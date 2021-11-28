#pragma once
#include <string>
#include <map>
#include <memory>
#include "token.h"

namespace lox
{
    class environment
    {
        public:
            environment();
            environment(std::shared_ptr<environment> enclosing);

            void define(std::string name, object value);
            object get(token name);
            void assign(token name, object value);

        private:
            std::map<std::string, object> m_values;
            std::shared_ptr<environment> m_enclosing;
    };
}