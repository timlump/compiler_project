#include "expr.h"
#include "ast_printer.h"

int main()
{
    lox::token minus_token;
    {
        minus_token.type = lox::token_type::MINUS;
        minus_token.lexeme = "-";
        minus_token.line = 1;
    }

    lox::token star_token;
    {
        star_token.type = lox::token_type::STAR;
        star_token.lexeme = "*";
        star_token.line = 1;
    }

    std::shared_ptr<lox::expr> expression = std::make_shared<lox::binary_expr>(
        // -123
        std::make_shared<lox::unary_expr>(minus_token,
            std::make_shared<lox::literal_expr>(lox::object(123.0))
        ),
        //*
        star_token,
        //(45.67)
        std::make_shared<lox::grouping_expr>(
            std::make_shared<lox::literal_expr>(lox::object(45.67))
        )
    );

    ast_printer printer;
    auto result = printer.print(expression.get());

    std::cout << result << std::endl;

    return 0;
}