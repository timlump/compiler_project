#pragma once
#include "expr.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

class ast_printer : public lox::expr_visitor
{
    public:
        std::string print(lox::expr* expr)
        {
            return expr->accept(this).to_string();
        }

        lox::object visit_binary(lox::binary_expr* expr) override
        {
            return parenthesize(expr->m_op.lexeme, {expr->m_left.get(), expr->m_right.get()});
        }

        lox::object visit_grouping(lox::grouping_expr* expr) override
        {
            return parenthesize("group", {expr->m_expression.get()});
        }

        lox::object visit_literal(lox::literal_expr* expr) override
        {
            return expr->m_value;
        }

        lox::object visit_unary(lox::unary_expr* expr) override
        {
            return parenthesize(expr->m_op.lexeme, {expr->m_right.get()});
        }

    private:
        lox::object parenthesize(std::string name, std::vector<lox::expr*> exprs)
        {
            std::stringstream stream;
            stream << "(" << name;

            for (lox::expr* expr : exprs) {
                stream << " " << expr->accept(this).to_string();
            }
            stream << ")";

            return lox::object(stream.str());
        }
};