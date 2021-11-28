#pragma once
#include <memory>
#include <vector>
#include "token.h"
#include "expr.h"

namespace lox
{
    class expression_stmt;
    class print_stmt;
    class var_stmt;
    class block_stmt;
    class if_stmt;
    class while_stmt;

    class stmt_visitor
    {
        public:
            virtual void visit_expression(expression_stmt*) = 0;
            virtual void visit_print(print_stmt*) = 0;
            virtual void visit_var(var_stmt*) = 0;
            virtual void visit_block(block_stmt*) = 0;
            virtual void visit_if(if_stmt*) = 0;
            virtual void visit_while(while_stmt*) = 0;
    };

    class stmt
    {
        public:
            virtual void accept(stmt_visitor*) = 0;
    };

    class expression_stmt : public stmt
    {
        public:
            expression_stmt(std::shared_ptr<expr> expression) {
                m_expression = expression;
            }

            void accept(stmt_visitor* visitor) override
            {
                visitor->visit_expression(this);
            }

            std::shared_ptr<expr> m_expression;
    };

    class print_stmt : public stmt
    {
        public:
            print_stmt(std::shared_ptr<expr> expression) {
                m_expression = expression;
            }

            void accept(stmt_visitor* visitor) override
            {
                visitor->visit_print(this);
            }

            std::shared_ptr<expr> m_expression;
    };

    class var_stmt : public stmt
    {
        public:
            var_stmt(token token, std::shared_ptr<expr> initializer)
            {
                m_name = token;
                m_initializer = initializer;
            }

            void accept(stmt_visitor* visitor) override
            {
                visitor->visit_var(this);
            }

            token m_name;
            std::shared_ptr<expr> m_initializer;
    };

    class block_stmt : public stmt
    {
        public:
            block_stmt(std::vector<std::shared_ptr<stmt>> statements)
            {
                m_statements = statements;
            }

            void accept(stmt_visitor* visitor) override
            {
                visitor->visit_block(this);
            }

            std::vector<std::shared_ptr<stmt>> m_statements;
    };

    class if_stmt : public stmt
    {
        public:
            if_stmt(std::shared_ptr<expr> condition,
                    std::shared_ptr<stmt> then_branch,
                    std::shared_ptr<stmt> else_branch) {
                        m_condition = condition;
                        m_then_branch = then_branch;
                        m_else_branch = else_branch;
                    }

            void accept(stmt_visitor* visitor) override
            {
                visitor->visit_if(this);
            }

            std::shared_ptr<expr> m_condition;
            std::shared_ptr<stmt> m_then_branch;
            std::shared_ptr<stmt> m_else_branch;
    };

    class while_stmt : public stmt
    {
        public:
            while_stmt(std::shared_ptr<expr> condition, std::shared_ptr<stmt> body)
            {
                m_condition = condition;
                m_body = body;
            }

            void accept(stmt_visitor* visitor) override
            {
                visitor->visit_while(this);
            }

            std::shared_ptr<expr> m_condition;
            std::shared_ptr<stmt> m_body;
    };
}