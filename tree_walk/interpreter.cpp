#include "interpeter.h"
#include "tree_walk.h"
#include <iostream>

namespace lox
{
    interpreter::interpreter()
    {
        m_environment = std::make_shared<environment>();
    }

    object interpreter::visit_assign(assign_expr* expr)
    {
        auto value = evaluate(expr->m_value.get());
        m_environment->assign(expr->m_name, value);
        return value;
    }

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

    object interpreter::visit_variable(variable_expr* expr)
    {
        return m_environment->get(expr->m_name);
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

    object interpreter::visit_logical(logical_expr* expr)
    {
        auto left = evaluate(expr->m_left.get());

        if (expr->m_op.type == token_type::OR) {
            // short circuit
            if (left) {
                return left;
            }
        }
        else {
            if (not left) {
                return left;
            }
        }

        return evaluate(expr->m_right.get());
    }

    void interpreter::visit_print(print_stmt* statement)
    {
        auto value = evaluate(statement->m_expression.get());
        std::cout << value.to_string() << std::endl;
    }

    void interpreter::visit_expression(expression_stmt* statement)
    {
        evaluate(statement->m_expression.get());
    }

    void interpreter::visit_var(var_stmt* statement)
    {
        object value;
        if (statement->m_initializer) {
            value = evaluate(statement->m_initializer.get());
        }
        m_environment->define(statement->m_name.lexeme, value);
    }

    void interpreter::visit_block(block_stmt* statement)
    {
        // local scope
        auto block_environment = std::make_shared<environment>(m_environment);
        execute_block(statement->m_statements, block_environment);
    }

    void interpreter::visit_if(if_stmt* statement)
    {
        auto condition = evaluate(statement->m_condition.get());
        // this is invoking the overloaded bool() operator in lox::object
        if (condition) {
            execute(statement->m_then_branch.get());
        }
        else if (statement->m_else_branch) {
            execute(statement->m_else_branch.get());
        }
    }

    void interpreter::visit_while(while_stmt* statement)
    {
        while (evaluate(statement->m_condition.get())) {
            execute(statement->m_body.get());
        }
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

    void interpreter::execute_block(std::vector<std::shared_ptr<stmt>> statements,
     std::shared_ptr<environment> local_environment) {
         auto previous_environment = m_environment;
         try {
            m_environment = local_environment;

            for (auto statement : statements) {
                execute(statement.get());
            }
         }
         // in Crafting Intrepeters this was a finally block which c++ doesn't have
         catch (const std::exception& e) {
             m_environment = previous_environment;
            throw e;
         }
         
         m_environment = previous_environment;
     }
}