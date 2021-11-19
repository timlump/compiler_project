#pragma once

#include <string>
#include <iostream>
#include <memory>

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

        END_OF_FIELD
    };

    class object_base
    {
        public:
            virtual std::string to_string() = 0;
    };

    template<class T>
    class object : public object_base
    {
        public:
            object() {};
            object(T value) : m_value(value) {};
            T m_value;
            std::string to_string() override {

                return std::to_string(m_value);
            }
    };

    template<>
    class object<std::string> : public object_base
    {
        public:
            object() {};
            object(std::string value) : m_value(value) {};
            std::string m_value;
            std::string to_string() override {
                return m_value;
            }
    };

    template<>
    class object<std::nullptr_t> : public object_base
    {
        public:
            std::string to_string() override {
                return "nil";
            }
    };

    template<>
    class object<bool> : public object_base
    {
        public:
            object() {};
            object(bool value) : m_value(value) {};
            bool m_value;
            std::string to_string() override {
                return m_value ? "true" : "false";
            }

            object<bool> operator!()
            {
                object<bool> result(!m_value);
                return result;
            }
    };

    template<>
    class object<double> : public object_base
    {
        public:
            object() {};
            object(double value) : m_value(value) {};
            double m_value;
            std::string to_string() override {
                return std::to_string(m_value);
            }

            object<double> operator-()
            {
                object<double> result(-m_value);
                return result;
            }

            object<double> operator+(const object<double>& right)
            {
                object<double> result(m_value + right.m_value);
                return result;
            }

            object<double> operator-(const object<double>& right)
            {
                object<double> result(m_value - right.m_value);
                return result;
            }

            object<double> operator*(const object<double>& right)
            {
                object<double> result(m_value * right.m_value);
                return result;
            }

            object<double> operator/(const object<double>& right)
            {
                object<double> result(m_value / right.m_value);
                return result;
            }


    };

    struct token {
        token_type type;
        std::string lexeme;
        std::shared_ptr<object_base> value;
        int line;
    };

    template <class T>
    static std::shared_ptr<object<T>> cast_to(std::shared_ptr<object_base> obj)
    {
        auto result = dynamic_cast<std::shared_ptr<object<T>>>(obj);
        if (result == nullptr) {
            throw std::bad_cast();
        }
        else {
            return std::shared_ptr<object<T>>(result->m_value);
        }
    }

    template <class T>
    static std::shared_ptr<object<T>> make()
    {
        return std::make_shared<object<T>>();
    }

    template <class T>
    static std::shared_ptr<object<T>> make(T value)
    {
        return std::make_shared<object<T>>(value);
    }

    template <class T>
    static std::shared_ptr<object<T>> make(object<T> value)
    {
        return make<T>(value->m_value);
    }
}