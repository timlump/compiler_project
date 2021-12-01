#pragma once
#include <string>
#include "token.h"
#include "interpreter.h"

namespace lox {
    class tree_walk {
        public:
            tree_walk() = delete;
            static void run(std::string source);

            static void run_prompt();

            static void run_file(std::string path);

            static void error(int line, std::string message);
            static void error(token token, std::string message);
            static void runtime_error(const lox_runtime_exception& e);

            static bool had_error;
            static bool had_runtime_error;

        private:
            static interpreter * m_interpreter;
            static void report(int line, std::string where, std::string message);
    };
}