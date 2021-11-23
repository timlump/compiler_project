#include "token.h"
#include <iostream>
#include <sstream>

namespace lox
{
    const std::string MISMATCH_MSG = "Operand type mismatch.";
    const std::string MUST_BE_NUMBERS_MSG = "Operands must be numbers.";
    const std::string MUST_BE_NUMBERS_OR_STRINGS_MSG = "Operands must be both numbers or both strings.";
    const std::string UNSUPPORTED_MSG = "Unknown operand error";

    object::object(std::nullptr_t)
    {
        m_type = object_type::nil;
    }

    object::object(bool value)
    {
        m_type = object_type::boolean;
        m_boolean_value = value;
    }

    object::object(double value)
    {
        m_type = object_type::number;
        m_number_value = value;
    }

    object::object(std::string value)
    {
        m_type = object_type::text;
        m_text_value = value;
    }

    object object::operator!()
    {
        bool result = true;
        switch (m_type) {
            case object_type::nil:
            {
                result = false;
            } break;
            case object_type::boolean:
            {
                result = m_boolean_value;
            } break;
            default:
                result = true;
        }

        return object(!result);
    }

    object object::operator-()
    {
        switch (m_type) {
            case object_type::number:
                return object(-m_number_value);
            default:
                throw std::logic_error(MUST_BE_NUMBERS_MSG);
        }
    }

    object object::operator>(const object& right)
    {
        if (m_type != right.m_type) {
            throw std::logic_error(MISMATCH_MSG);
        }

        switch (m_type) {
            case object_type::number:
                return object(m_number_value > right.m_number_value);
            default:
                throw std::logic_error(MUST_BE_NUMBERS_MSG);
        }
    }

    object object::operator>=(const object& right)
    {
        if (m_type != right.m_type) {
            throw std::logic_error(MISMATCH_MSG);
        }

        switch (m_type) {
            case object_type::number:
                return object(m_number_value >= right.m_number_value);
            default:
                throw std::logic_error(MUST_BE_NUMBERS_MSG);
        }
    }

    object object::operator<(const object& right)
    {
        if (m_type != right.m_type) {
            throw std::logic_error(MISMATCH_MSG);
        }

        switch (m_type) {
            case object_type::number:
                return object(m_number_value < right.m_number_value);
            default:
                throw std::logic_error(MUST_BE_NUMBERS_MSG);
        }
    }

    object object::operator<=(const object& right)
    {
        if (m_type != right.m_type) {
            throw std::logic_error(MISMATCH_MSG);
        }

        switch (m_type) {
            case object_type::number:
                return object(m_number_value <= right.m_number_value);
            default:
                throw std::logic_error(MUST_BE_NUMBERS_MSG);
        }
    }

    object object::operator==(const object& right)
    {
        if (m_type != right.m_type) {
            return object(false);
        }

        switch (m_type) {
            case object_type::number:
                return object(m_number_value == right.m_number_value);
            case object_type::boolean:
                return object(m_boolean_value == right.m_boolean_value);
            case object_type::text:
                return object(m_text_value == right.m_text_value);
            case object_type::nil:
                return object(true);
            // should be unreachable
            default:
                throw std::logic_error(UNSUPPORTED_MSG);
        }
    }

    object object::operator!=(const object& right)
    {
        return !((*this)==right);
    }

    object object::operator-(const object& right)
    {
        if (m_type != right.m_type) {
            throw std::logic_error(MISMATCH_MSG);
        }

        switch (m_type) {
            case object_type::number:
                return object(m_number_value - right.m_number_value);
            default:
                throw std::logic_error(MUST_BE_NUMBERS_MSG);
        }
    }

    object object::operator+(const object& right)
    {
        if (m_type != right.m_type) {
            throw std::logic_error(MISMATCH_MSG);
        }

        switch (m_type) {
            case object_type::number:
                return object(m_number_value + right.m_number_value);
            case object_type::text:
                return object(m_text_value + right.m_text_value);
            default:
                throw std::logic_error(MUST_BE_NUMBERS_OR_STRINGS_MSG);
        }
    }

    object object::operator*(const object& right)
    {
        if (m_type != right.m_type) {
            throw std::logic_error(MISMATCH_MSG);
        }

        switch (m_type) {
            case object_type::number:
                return object(m_number_value * right.m_number_value);
            default:
                throw std::logic_error(MUST_BE_NUMBERS_MSG);
        }
    }

    object object::operator/(const object& right)
    {
        if (m_type != right.m_type) {
            throw std::logic_error(MISMATCH_MSG);
        }

        switch (m_type) {
            case object_type::number:
                return object(m_number_value / right.m_number_value);
            default:
                throw std::logic_error(MUST_BE_NUMBERS_MSG);
        }
    }

    std::string object::to_string() 
    {
        switch (m_type)
        {
            case object_type::boolean:
                return m_boolean_value ? "true" : "false";
            case object_type::number:
            {
                std::stringstream stream;
                stream << m_number_value;
                return stream.str();
            } break;
            case object_type::text:
                return m_text_value;
            default:
                return "nil";
        }
    }
}