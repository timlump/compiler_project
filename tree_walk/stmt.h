#pragma once
#include <memory>
#include "token.h"
#include "expr.h"

namespace lox
{
    class stmt_visitor
    {
        public:
            virtual void visit_expression(expression_stmt*) = 0;
            virtual void visit_print(print_stmt*) = 0;
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
}