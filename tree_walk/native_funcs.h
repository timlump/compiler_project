#pragma once
#include "lox_callable.h"
#include <chrono>

namespace lox
{
    // in crafting interpreters this was an anonymouse class
    class clock : public lox_callable
    {
        public:
            int arity() override {
                return 0;
            }

            object call(interpreter* interpreter, std::vector<object> arguments) override
            {
                auto now = std::chrono::system_clock::now();
                auto ms_since_epoch = std::chrono::duration_cast<std::chrono::milliseconds>
                    (now.time_since_epoch()).count();
                return object(static_cast<double>(ms_since_epoch));
            }
    };
}