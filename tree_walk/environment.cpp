#include "environment.h"

namespace lox
{
    environment::environment()
    {
        m_enclosing = nullptr;
    }
    
    environment::environment(std::shared_ptr<environment> enclosing)
    {
        m_enclosing = enclosing;
    }

    void environment::define(std::string name, object value)
    {
        m_values[name] = value;
    }

    object environment::get(token name)
    {
        auto find_iter = m_values.find(name.lexeme);
        if (find_iter != m_values.end()) {
            return find_iter->second;
        }

        if (m_enclosing != nullptr) {
            return m_enclosing->get(name);
        }

        throw lox_runtime_exception(name, 
            "Undefined variable '" + name.lexeme + "'.");
    }

    void environment::assign(token name, object value)
    {
        auto find_iter = m_values.find(name.lexeme);
        if (find_iter != m_values.end()) {
            m_values[name.lexeme] = value;
            return;
        }

        if (m_enclosing != nullptr) {
            m_enclosing->assign(name, value);
            return;
        }

        throw lox_runtime_exception(name,
            "Undefined variable '" + name.lexeme + "'.");
    }
}