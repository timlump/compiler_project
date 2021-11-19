#pragma once
#include <string>
#include "token.h"

namespace lox {    
    class tree_walk {
        public:
            tree_walk() = delete;
            static void run(std::string source);

            static void run_prompt();

            static void run_file(std::string path);

            static void error(int line, std::string message);
            static void error(token token, std::string message);

            static bool had_error;

        private:
            static void report(int line, std::string where, std::string message);
    };
}