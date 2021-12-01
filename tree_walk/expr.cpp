#include "expr.h"

namespace lox
{
    
    assign_expr::assign_expr(token name, std::shared_ptr<expr> value)
    {
        m_name = name;
        m_value = value;
    }

    object assign_expr::accept(expr_visitor* visitor)
    {
        return visitor->visit_assign(this);
    }
    
    binary_expr::binary_expr(std::shared_ptr<expr> left, token op, std::shared_ptr<expr> right)
    {
        m_left = left;
        m_op = op;
        m_right = right;
    }

    object binary_expr::accept(expr_visitor* visitor)
    {
        return visitor->visit_binary(this);
    }

    grouping_expr::grouping_expr(std::shared_ptr<expr> expression) {
        m_expression = expression;
    }

    object grouping_expr::accept(expr_visitor* visitor)
    {
        return visitor->visit_grouping(this);
    }

    
    literal_expr::literal_expr(object value) 
    {
        m_value = value;
    }

    object literal_expr::accept(expr_visitor* visitor)
    {
        return visitor->visit_literal(this);
    }

    
    variable_expr::variable_expr(token token)
    {
        m_name = token;
    }

    object variable_expr::accept(expr_visitor* visitor)
    {
        return visitor->visit_variable(this);
    }

    unary_expr::unary_expr(token op, std::shared_ptr<expr> right)
    {
        m_op = op;
        m_right = right;
    }

    object unary_expr::accept(expr_visitor* visitor)
    {
        return visitor->visit_unary(this);
    }

    logical_expr::logical_expr(std::shared_ptr<expr> left, token op, std::shared_ptr<expr> right)
    {
        m_left = left;
        m_op = op;
        m_right = right;
    }

    object logical_expr::accept(expr_visitor* visitor)
    {
        return visitor->visit_logical(this);
    }

    call_expr::call_expr(std::shared_ptr<expr> callee, token paren,
                std::vector<std::shared_ptr<expr>> arguments)
    {
        m_callee = callee;
        m_paren = paren;
        m_arguments = arguments;
    }

    object call_expr::accept(expr_visitor* visitor)
    {
        return visitor->visit_call(this);
    }
}