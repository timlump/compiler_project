#include "interpeter.h"

namespace lox
{
    std::shared_ptr<object_base> interpreter::visit_binary(binary_expr* expr)
    {
        try {
            // TODO - turns out this can have more than just doubles as strings can also be concatenated
            auto left = *cast_to<double>(evaluate(expr->m_left.get()));
            auto right = *cast_to<double>(evaluate(expr->m_right.get()));

            switch(expr->m_op.type) {
                case token_type::MINUS:
                    return make<double>(left - right);
                case token_type::SLASH:
                    return make<double>(left / right);
                case token_type::STAR:
                    return make<double>(left * right);
            }

            return make<std::nullptr_t>();
        }
        catch (std::bad_cast& e)
        {
            return make<std::nullptr_t>();
        }
    }

    std::shared_ptr<object_base> interpreter::visit_grouping(grouping_expr* expr)
    {
        return evaluate(expr);
    }

    std::shared_ptr<object_base> interpreter::visit_literal(literal_expr* expr)
    {
        return expr->m_value;
    }

    std::shared_ptr<object_base> interpreter::visit_unary(unary_expr* expr)
    {
        auto right = evaluate(expr->m_right.get());
        switch(expr->m_op.type) {
            case token_type::BANG:
            {
                bool value = is_truthy(right);
                return make<bool>(!value);
            } break;

            case token_type::MINUS:
            {
                auto value = *cast_to<double>(right);
                return make<double>(-value);
            } break;
        }

        return make<std::nullptr_t>();
        
    }

    std::shared_ptr<object_base> interpreter::evaluate(expr* expr)
    {
        return expr->accept(this);
    }

    bool interpreter::is_truthy(std::shared_ptr<object_base> obj)
    {
        try {
            return cast_to<bool>(obj)->m_value;
        } catch(std::bad_cast& e) {
            return false;
        }
    }
}