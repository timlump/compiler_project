#pragma once
#include "expr.h"

namespace lox
{
    class interpreter : public lox::visitor
    {
        public:
            std::shared_ptr<object_base> visit_binary(binary_expr*) override;
            std::shared_ptr<object_base> visit_grouping(grouping_expr*) override;
            std::shared_ptr<object_base> visit_literal(literal_expr*) override;
            std::shared_ptr<object_base> visit_unary(unary_expr*) override;

        private:
            std::shared_ptr<object_base> evaluate(expr* expr);
            bool is_truthy(std::shared_ptr<object_base> obj);
    };
}