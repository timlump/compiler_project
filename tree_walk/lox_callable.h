#pragma once
#include "token.h"
#include "interpreter.h"
#include <vector>

namespace lox
{
    class lox_callable
    {
        public:
            virtual int arity() = 0;
            virtual object call(interpreter* interpreter, std::vector<object> arguments) = 0; 
    };
}