#include "parser.h"
#include "tree_walk.h"

namespace lox
{
    parser::parser(std::vector<token> tokens) {
        m_tokens = tokens;
    }

    std::shared_ptr<expr> parser::parse()
    {
        try {
            return expression();
        }
        catch (std::runtime_error& e) {
            return nullptr;
        }
    }

    std::shared_ptr<expr> parser::expression()
    {
        return equality();
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

        return primary();
    }

    std::shared_ptr<expr> parser::primary()
    {
        if (match({token_type::FALSE}))
        {
            auto lit = std::make_shared<object<bool>>();
            lit->m_value = false;
            return std::make_shared<literal_expr>(lit);
        }

        if (match({token_type::TRUE}))
        {
            auto lit = std::make_shared<object<bool>>();
            lit->m_value = true;
            return std::make_shared<literal_expr>(lit);
        }

        if (match({token_type::NIL}))
        {
            auto lit = std::make_shared<object<std::nullptr_t>>();
            return std::make_shared<literal_expr>(lit);
        }

        if (match({token_type::NUMBER, token_type::STRING})) {
            return std::make_shared<literal_expr>(previous().value);
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
        return peek().type == token_type::END_OF_FIELD;
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