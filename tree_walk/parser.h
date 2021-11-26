#pragma once
#include <vector>
#include <memory>
#include <exception>
#include "token.h"
#include "expr.h"
#include "stmt.h"

namespace lox
{
    class parser
    {
        public:
            parser(std::vector<token> tokens);
            std::vector<std::shared_ptr<stmt>> parse();

        private:

            // statement -> expr_stmt | print_stmt
            std::shared_ptr<stmt> statement();
            // expr_statement -> expression ";"
            std::shared_ptr<stmt> expr_statement();
            // print_statement -> "print" expression ";"
            std::shared_ptr<stmt> print_statement();

            // expression -> equality
            std::shared_ptr<expr> expression();
            // equality -> comparison ( ("!=" | "==") comparison )*
            std::shared_ptr<expr> equality();
            // comparison -> term ( ( ">" | ">= " | "<" | "<=" ) term )*
            std::shared_ptr<expr> comparison();
            // term -> factor ( ( "-" | "+" ) factor )*
            std::shared_ptr<expr> term();
            // factor -> unary ( ( "/" | "*" ) unary )*
            std::shared_ptr<expr> factor();
            // unary -> ( "!" | "-" ) unary | primary
            std::shared_ptr<expr> unary();
            // primary -> NUMBER | STRING | "true" | "false" | "nil" | "(" expression ")"
            std::shared_ptr<expr> primary();

            bool match(std::vector<token_type> types);
            bool check(token_type type);
            token advance();
            token peek();
            token previous();
            token consume(token_type type, std::string message);
            bool is_at_end();
            void synchronize();

            std::runtime_error error(token token, std::string message);

            std::vector<token> m_tokens;
            int m_current = 0;
    };
}