#pragma once
#include <memory>
#include "token.h"

namespace lox {

    class binary_expr;
    class grouping_expr;
    class literal_expr;
    class unary_expr;

    class visitor
    {
        public:
            virtual std::shared_ptr<object_base> visit_binary(binary_expr*) = 0;
            virtual std::shared_ptr<object_base> visit_grouping(grouping_expr*) = 0;
            virtual std::shared_ptr<object_base> visit_literal(literal_expr*) = 0;
            virtual std::shared_ptr<object_base> visit_unary(unary_expr*) = 0;
    };

    class expr
    {
        public:
            virtual std::shared_ptr<object_base> accept(visitor*) = 0;
    };


    class binary_expr : public expr
    {
        public:
            binary_expr(std::shared_ptr<expr> left, token op, std::shared_ptr<expr> right)
            {
                m_left = left;
                m_op = op;
                m_right = right;
            }

            std::shared_ptr<object_base> accept(visitor* visitor) override
            {
                return visitor->visit_binary(this);
            }

            std::shared_ptr<expr> m_left;
            token m_op;
            std::shared_ptr<expr> m_right;
    };

    class grouping_expr : public expr
    {
        public:
            grouping_expr(std::shared_ptr<expr> expression) {
                m_expression = expression;
            }

            std::shared_ptr<object_base>  accept(visitor* visitor) override
            {
                return visitor->visit_grouping(this);
            }

            std::shared_ptr<expr> m_expression;
    };

    class literal_expr : public expr
    {
        public:
            literal_expr(std::shared_ptr<object_base> value) 
            {
                m_value = value;
            }

            std::shared_ptr<object_base> accept(visitor* visitor) override
            {
                return visitor->visit_literal(this);
            }

            std::shared_ptr<object_base> m_value;
    };

    class unary_expr : public expr
    {
        public:
            unary_expr(token op, std::shared_ptr<expr> right)
            {
                m_op = op;
                m_right = right;
            }

            std::shared_ptr<object_base> accept(visitor* visitor) override
            {
                return visitor->visit_unary(this);
            }

            token m_op;
            std::shared_ptr<expr> m_right;
    };
}