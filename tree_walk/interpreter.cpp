#include "interpeter.h"

namespace lox
{
    object interpreter::visit_binary(binary_expr* expr)
    {
        try {
            auto left = evaluate(expr->m_left.get());
            auto right = evaluate(expr->m_right.get());
            
            switch(expr->m_op.type) {
                case token_type::GREATER:
                    return left > right;
                case token_type::GREATER_EQUAL:
                    return left >= right;
                case token_type::LESS:
                    return left < right;
                case token_type::LESS_EQUAL:
                    return left <= right;
                case token_type::BANG_EQUAL:
                    return left != right;
                case token_type::EQUAL_EQUAL:
                    return left == right;
                case token_type::MINUS:
                    return left - right;
                case token_type::SLASH:
                    return left / right;
                case token_type::STAR:
                    return left * right;
            }

            return object(nullptr);
        }
        catch (std::logic_error& e)
        {
            throw lox_runtime_exception(expr->m_op, e.what());
        }
    }

    object interpreter::visit_grouping(grouping_expr* expr)
    {
        return evaluate(expr);
    }

    object interpreter::visit_literal(literal_expr* expr)
    {
        return expr->m_value;
    }

    object interpreter::visit_unary(unary_expr* expr)
    {
        auto right = evaluate(expr->m_right.get());
        switch(expr->m_op.type) {
            case token_type::BANG:
                return object(!right);

            case token_type::MINUS:
                return -right;
        }

        return object(nullptr);
        
    }

    object interpreter::evaluate(expr* expr)
    {
        return expr->accept(this);
    }
}