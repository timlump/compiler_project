#pragma once

#include <string>
#include <exception>

namespace lox {
    enum class token_type {
        // single-character tokens
        LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
        COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

        // one or two-character tokens
        BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL,
        GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,

        // literals
        IDENTIFIER, STRING, NUMBER,

        // keywords
        AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
        PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

        END_OF_FILE
    };

    class object
    {
        public:
            object(std::nullptr_t);
            object(bool value);
            object(double value);
            object(std::string value);

            enum class object_type {nil, boolean, number, text} m_type = object_type::nil;

            bool m_boolean_value;
            double m_number_value;
            std::string m_text_value;

            object operator!();

            object operator-();

            object operator>(const object& right);

            object operator>=(const object& right);

            object operator<(const object& right);

            object operator<=(const object& right);

            object operator==(const object& right);

            object operator!=(const object& right);

            object operator-(const object& right);

            object operator+(const object& right);

            object operator*(const object& right);

            object operator/(const object& right);

            std::string to_string();
    };

    struct token {
        token_type type;
        std::string lexeme;
        object value = object(nullptr);
        int line;
    };

    class lox_runtime_exception : public std::exception
    {
        public:
            lox_runtime_exception(token token, std::string message)
            {
                m_token = token;
                m_message = message;
            }

            token m_token;
            std::string m_message;
    };
}