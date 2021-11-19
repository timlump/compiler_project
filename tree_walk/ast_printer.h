#pragma once
#include "expr.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

class ast_printer : public lox::visitor
{
    public:
        std::string print(lox::expr* expr)
        {
            return expr->accept(this)->to_string();
        }

        std::shared_ptr<lox::object_base> visit_binary(lox::binary_expr* expr) override
        {
            return parenthesize(expr->m_op.lexeme, {expr->m_left.get(), expr->m_right.get()});
        }

        std::shared_ptr<lox::object_base>visit_grouping(lox::grouping_expr* expr) override
        {
            return parenthesize("group", {expr->m_expression.get()});
        }

        std::shared_ptr<lox::object_base> visit_literal(lox::literal_expr* expr) override
        {
            return expr->m_value;
        }

        std::shared_ptr<lox::object_base> visit_unary(lox::unary_expr* expr) override
        {
            return parenthesize(expr->m_op.lexeme, {expr->m_right.get()});
        }

    private:
        std::shared_ptr<lox::object_base> parenthesize(std::string name, std::vector<lox::expr*> exprs)
        {
            std::stringstream stream;
            stream << "(" << name;

            for (lox::expr* expr : exprs) {
                stream << " " << expr->accept(this)->to_string();
            }
            stream << ")";

            auto result = std::make_shared<lox::object<std::string>>();
            result->m_value = stream.str();
            return result;
        }
};