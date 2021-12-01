#pragma once
#include <memory>
#include <string>
#include <exception>

namespace lox {
    class lox_callable;

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
            object();
            object(std::nullptr_t);
            object(bool value);
            object(double value);
            object(std::string value);
            object(std::shared_ptr<lox_callable> callable);

            enum class object_type {nil, boolean, number, text, callable} m_type = object_type::nil;

            bool m_boolean_value;
            double m_number_value;
            std::string m_text_value;
            std::shared_ptr<lox_callable> m_callable;

            operator bool() const;

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