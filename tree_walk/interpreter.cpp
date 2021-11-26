#include "interpeter.h"
#include "tree_walk.h"
#include <iostream>

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
                case token_type::PLUS:
                    return left + right;
                case token_type::MINUS:
                    return left - right;
                case token_type::SLASH:
                    return left / right;
                case token_type::STAR:
                    return left * right;
                default:
                    // not supported yet
                    break;
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
        return evaluate(expr->m_expression.get());
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

            default:
                return object(nullptr);
        }
    }

    void interpreter::visit_print(print_stmt* statement)
    {
        evaluate(statement->m_expression.get());
    }

    void interpreter::visit_expression(expression_stmt* statement)
    {
        auto value = evaluate(statement->m_expression.get());
        std::cout << value.to_string() << std::endl;
    }

    void interpreter::interpret(std::vector<std::shared_ptr<stmt>> statements)
    {
        try
        {
            for (auto statement : statements) {
                execute(statement.get());
            }
        }
        catch(const lox_runtime_exception& e)
        {
            tree_walk::runtime_error(e);
        }
    }

    object interpreter::evaluate(expr* expr)
    {
        return expr->accept(this);
    }

    void interpreter::execute(stmt* statement)
    {
        statement->accept(this);
    }
}