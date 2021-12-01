#pragma once
#include "expr.h"
#include "stmt.h"
#include "environment.h"
#include <vector>

namespace lox
{
    class interpreter : public lox::expr_visitor, lox::stmt_visitor
    {
        public:
            interpreter();

            object visit_assign(assign_expr* exp) override;
            object visit_binary(binary_expr* exp) override;
            object visit_grouping(grouping_expr* exp) override;
            object visit_literal(literal_expr* exp) override;
            object visit_variable(variable_expr* exp) override;
            object visit_unary(unary_expr* exp) override;
            object visit_logical(logical_expr* exp) override;
            object visit_call(call_expr* exp) override;

            void visit_print(print_stmt* statement);
            void visit_expression(expression_stmt* statement) override;
            void visit_var(var_stmt* statement) override;
            void visit_block(block_stmt* statement) override;
            void visit_if(if_stmt* statement) override;
            void visit_while(while_stmt* statement) override;
            void visit_function(function_stmt* statement) override;

            void interpret(std::vector<std::shared_ptr<stmt>> statements);

        private:
            std::shared_ptr<environment> m_globals = nullptr;
            std::shared_ptr<environment> m_environment = nullptr;
            object evaluate(expr* expr);
            void execute(stmt* statement);
            void execute_block(std::vector<std::shared_ptr<stmt>> statements,
                               std::shared_ptr<environment> local_environment);
    };
}