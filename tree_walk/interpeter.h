#pragma once
#include "expr.h"

namespace lox
{
    class interpreter : public lox::visitor
    {
        public:
            object visit_binary(binary_expr*) override;
            object visit_grouping(grouping_expr*) override;
            object visit_literal(literal_expr*) override;
            object visit_unary(unary_expr*) override;

        private:
            object evaluate(expr* expr);
    };
}