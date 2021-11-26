#pragma once
#include "expr.h"
#include "stmt.h"
#include <vector>

namespace lox
{
    class interpreter : public lox::expr_visitor, lox::stmt_visitor
    {
        public:
            object visit_binary(binary_expr* exp) override;
            object visit_grouping(grouping_expr* exp) override;
            object visit_literal(literal_expr* exp) override;
            object visit_unary(unary_expr* exp) override;

            void visit_print(print_stmt* statement);
            void visit_expression(expression_stmt* statement) override;

            void interpret(std::vector<std::shared_ptr<stmt>> statements);

        private:
            object evaluate(expr* expr);
            void execute(stmt* statement);
    };
}