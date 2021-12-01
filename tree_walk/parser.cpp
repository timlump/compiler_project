#include "parser.h"
#include "tree_walk.h"
#include <iostream>

namespace lox
{
    parser::parser(std::vector<token> tokens) {
        m_tokens = tokens;
    }

    std::vector<std::shared_ptr<stmt>> parser::parse()
    {
        std::vector<std::shared_ptr<stmt>> statements;
        while (not is_at_end()) {
            statements.push_back(declaration());
        }
        return statements;
    }

    std::shared_ptr<stmt> parser::declaration()
    {
        try {
            if (match({token_type::FUN})) {
                return function("function");
            }
            if (match({token_type::VAR})) {
                return var_declaration();
            }
            return statement();
        }
        catch (const std::runtime_error& e){
            synchronize();
            return nullptr;
        }
    }

    std::shared_ptr<stmt> parser::var_declaration()
    {
        auto name = consume(token_type::IDENTIFIER, "Expect variable name.");
        std::shared_ptr<expr> initializer = nullptr;
        if (match({token_type::EQUAL})) {
            initializer = expression();
        }

        consume(token_type::SEMICOLON, "Expect ';' after variable declaration.");
        return std::make_shared<var_stmt>(name, initializer);
    }

    std::shared_ptr<stmt> parser::function(std::string kind) {
        auto name = consume(token_type::IDENTIFIER, "Expect " + kind + " name.");

        consume(token_type::LEFT_PAREN, "Expect '(' after " + kind + " name.");
        std::vector<token> parameters;
        if (not check(token_type::RIGHT_PAREN)) {
            do {
                if (parameters.size() >= 255) {
                    error(peek(), "Can't have more than 255 parameters");
                }
                parameters.push_back(
                    consume(token_type::IDENTIFIER, "Expect parameter name")
                );
            }
            while(match({token_type::COMMA}));
        }
        consume(token_type::RIGHT_PAREN, "Expect ')' after parameters.");
        consume(token_type::LEFT_BRACE, "Expect '{' before " + kind + " body.");
        auto body = block();
        return std::make_shared<function_stmt>(name, parameters, body);
    }

    std::shared_ptr<stmt> parser::statement()
    {
        if (match({token_type::FOR})) {
            return for_statement();
        }

        if (match({token_type::IF})) {
            return if_statement();
        }

        if (match({token_type::PRINT})) {
            return print_statement();
        }

        if (match({token_type::WHILE})) {
            return while_statement();
        }

        if (match({token_type::LEFT_BRACE})) {
            return std::make_shared<block_stmt>(block());
        }

        return expr_statement();
    }

    std::vector<std::shared_ptr<stmt>> parser::block()
    {
        std::vector<std::shared_ptr<stmt>> statements;

        while (not check(token_type::RIGHT_BRACE) && not is_at_end()) {
            statements.push_back(declaration());
        }

        consume(token_type::RIGHT_BRACE, "Expect '}' after block.");
        return statements;
    }
    
    std::shared_ptr<stmt> parser::if_statement()
    {
        consume(token_type::LEFT_PAREN, "Expect '(' after 'if'.");
        auto condition = expression();
        consume(token_type::RIGHT_PAREN, "Expect ')' after if condition.");

        auto then_branch = statement();
        std::shared_ptr<stmt> else_branch = nullptr;
        if (match({token_type::ELSE})) {
            else_branch = statement();
        }

        return std::make_shared<if_stmt>(condition, then_branch, else_branch);
    }

    std::shared_ptr<stmt> parser::expr_statement()
    {
        auto exp = expression();
        consume(token_type::SEMICOLON, "Expect ';' after expression.");
        return std::make_shared<expression_stmt>(exp);
    }
    
    std::shared_ptr<stmt> parser::print_statement()
    {
        auto exp = expression();
        consume(token_type::SEMICOLON, "Expect ';' after expression.");
        return std::make_shared<print_stmt>(exp);
    }

    std::shared_ptr<stmt> parser::while_statement()
    {
        consume(token_type::LEFT_PAREN, "Expect '(' adter 'while'.");
        auto condition = expression();
        consume(token_type::RIGHT_PAREN, "Expect ')' after condition.");
        auto body = statement();

        return std::make_shared<while_stmt>(condition, body);
    }

    std::shared_ptr<stmt> parser::for_statement()
    {
        consume(token_type::LEFT_PAREN, "Expect '(' after 'for'.");

        std::shared_ptr<stmt> initializer;
        if (match({token_type::SEMICOLON})) {
            initializer = nullptr;
        }
        else if (match({token_type::VAR})) {
            initializer = var_declaration();
        }
        else {
            initializer = expr_statement();
        }

        std::shared_ptr<expr> condition = nullptr;
        if (not check(token_type::SEMICOLON)) {
            condition = expression();
        }
        consume(token_type::SEMICOLON, "Expect ';' after loop condition.");

        std::shared_ptr<expr> increment = nullptr;
        if (not check(token_type::RIGHT_PAREN)) {
            increment = expression();
        }

        consume(token_type::RIGHT_PAREN, "Expect ')' after for clauses.");

        auto body = statement();

        // now we synthesize a for loop out a while loop - aka desugaring

        if (increment) {
            std::vector<std::shared_ptr<stmt>> statements;
            statements.push_back(body);
            statements.push_back(std::make_shared<expression_stmt>(increment));
            body = std::make_shared<block_stmt>(statements);
        }

        if (condition == nullptr) {
            condition = std::make_shared<literal_expr>(true);
        }
        body = std::make_shared<while_stmt>(condition, body);

        if (initializer) {
            std::vector<std::shared_ptr<stmt>> statements;

            statements.push_back(initializer);
            statements.push_back(body);
            body = std::make_shared<block_stmt>(statements);
        }

        return body;
    }

    std::shared_ptr<expr> parser::expression()
    {
        return assignment();
    }

    std::shared_ptr<expr> parser::assignment()
    {
        auto exp = logic_or();

        if (match({token_type::EQUAL})) {
            auto equals = previous();
            auto value = assignment();

            variable_expr* var_exp = dynamic_cast<variable_expr*>(exp.get());
            if (var_exp){
                auto name = var_exp->m_name;
                return std::make_shared<assign_expr>(name, value);
            }

            error(equals, "Invalid assignment target.");
        }

        return exp;
    }

    std::shared_ptr<expr> parser::logic_or()
    {
        auto expr = logic_and();

        while (match({token_type::OR})) {
            auto op = previous();
            auto right = logic_and();
            expr = std::make_shared<logical_expr>(expr, op, right);
        }

        return expr;
    }

    std::shared_ptr<expr> parser::logic_and()
    {
        auto expr = equality();

        while (match({token_type::AND})) {
            auto op = previous();
            auto right = equality();
            expr = std::make_shared<logical_expr>(expr, op, right);
        }

        return expr;
    }

    std::shared_ptr<expr> parser::equality()
    {
        auto expr = comparison();
        while (match({token_type::BANG_EQUAL, token_type::EQUAL_EQUAL})) {
            token op = previous();
            auto right = comparison();
            expr = std::make_shared<binary_expr>(expr, op, right);
        }
        return expr;
    }

    std::shared_ptr<expr> parser::comparison()
    {
        auto expr = term();
        while (match({token_type::GREATER, token_type::GREATER_EQUAL, token_type::LESS, token_type::LESS_EQUAL})) {
            token op = previous();
            auto right = term();
            expr = std::make_shared<binary_expr>(expr, op, right);
        }
        return expr;
    }

    std::shared_ptr<expr> parser::term()
    {
        auto expr = factor();
        while (match({token_type::MINUS, token_type::PLUS})) {
            token op = previous();
            auto right = factor();
            expr = std::make_shared<binary_expr>(expr, op, right);
        }
        return expr;
    }

    std::shared_ptr<expr> parser::factor()
    {
        auto expr = unary();
        while (match({token_type::SLASH, token_type::STAR})) {
            token op = previous();
            auto right = unary();
            expr = std::make_shared<binary_expr>(expr, op, right);
        }
        return expr;
    }

    std::shared_ptr<expr> parser::unary()
    {
        if (match({token_type::BANG, token_type::MINUS})){
            token op = previous();
            auto right = unary();
            return std::make_shared<unary_expr>(op,right);
        }

        return call();
    }

    std::shared_ptr<expr> parser::call()
    {
        auto expr = primary();

        while (true) {
            if (match({token_type::LEFT_PAREN})) {
                expr = finish_call(expr);
            } 
            else {
                break;
            }
        }

        return expr;
    }

    std::shared_ptr<expr> parser::finish_call(std::shared_ptr<expr> callee)
    {
        std::vector<std::shared_ptr<expr>> arguments;
        if (not check(token_type::RIGHT_PAREN)) {
            do {
                if (arguments.size() >= 255) {
                    error(peek(), "Can't have more than 255 arguments.");        
                }
                arguments.push_back(expression());
            }
            while (match({token_type::COMMA}));
        }

        auto paren = consume(token_type::RIGHT_PAREN,
                            "Expect ')' after arguments.");

        return std::make_shared<call_expr>(callee, paren, arguments);
    }

    std::shared_ptr<expr> parser::primary()
    {
        if (match({token_type::FALSE}))
        {
            return std::make_shared<literal_expr>(object(false));
        }

        if (match({token_type::TRUE}))
        {
            return std::make_shared<literal_expr>(object(true));
        }

        if (match({token_type::NIL}))
        {
            return std::make_shared<literal_expr>(object(nullptr));
        }

        if (match({token_type::NUMBER, token_type::STRING})) {
            return std::make_shared<literal_expr>(previous().value);
        }

        if (match({token_type::IDENTIFIER})) {
            return std::make_shared<variable_expr>(previous());
        }

        if (match({token_type::LEFT_PAREN})) {
            auto expr = expression();
            consume(token_type::RIGHT_PAREN, "Expect ')' after expression.");
            return std::make_shared<grouping_expr>(expr);
        }

        throw error(peek(), "Expect expression.");
    }

    bool parser::match(std::vector<token_type> types)
    {
        for (auto type : types) {
            if (check(type)) {
                advance();
                return true;
            }
        }
        return false;
    }

    bool parser::check(token_type type)
    {
        if (is_at_end()){
            return false;
        }
        return peek().type == type;
    }

    token parser::advance()
    {
        if (not is_at_end()) {
            m_current++;
        }
        return previous();
    }

    token parser::peek()
    {
        return m_tokens[m_current];
    }

    token parser::previous()
    {
        return m_tokens[m_current - 1];
    }

    bool parser::is_at_end()
    {
        return peek().type == token_type::END_OF_FILE;
    }

    // consume all tokens until we hit the end of a statement
    // to hopefully recover from error
    void parser::synchronize()
    {
        advance();
        while(not is_at_end()) {
            if (previous().type == token_type::SEMICOLON) {
                return;
            }

            switch(peek().type) {
                case token_type::CLASS:
                case token_type::FOR:
                case token_type::FUN:
                case token_type::IF:
                case token_type::PRINT:
                case token_type::RETURN:
                case token_type::VAR:
                case token_type::WHILE:
                    return;
                default:
                    advance();
                    break;
            }
        }
    }

    token parser::consume(token_type type, std::string message)
    {
        if (check(type)) {
            return advance();
        }

        throw error(peek(), message);
    }

    std::runtime_error parser::error(token token, std::string message)
    {
        tree_walk::error(token, message);
        return std::runtime_error(message);
    }
}