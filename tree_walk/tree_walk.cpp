#include "tree_walk.h"
#include "parser.h"
#include "ast_printer.h"
#include "expr.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <exception>

#include "scanner.h"

namespace lox {    
    bool tree_walk::had_error = false;

    void tree_walk::run(std::string source) {
        scanner scanner(source);
        auto tokens = scanner.scan_tokens();
        parser psr(tokens);
        std::shared_ptr<expr> exp = psr.parse();

        if (tree_walk::had_error){
            return;
        }
        
        ast_printer printer;
        auto result = printer.print(exp.get());

        std::cout << result << std::endl;
    }

    void tree_walk::run_prompt() {
        while(true) {
            std::cout << "> ";
            std::string line;
            std::getline(std::cin, line);
            if (not line.empty()) {
                tree_walk::run(line);
                // we don't want to kill the interactive prompt if there is an error
                tree_walk::had_error = false;
            }
        }
    }

    void tree_walk::run_file(std::string path) {
        std::ifstream file;
        file.open(path, std::ifstream::in);
        if (file.is_open()) {
            file.seekg(0, file.end);
            int length = file.tellg();
            file.seekg(0, file.beg);

            std::vector<char> data(length);
            file.read(data.data(), length);
            file.close();

            std::string script(data.begin(), data.end());

            tree_walk::run(script);

            if (had_error) {
                // kill the script - we don't want a script full of errors to proceed
                throw std::runtime_error("Fatal Error");
            }
        }
    }

    void tree_walk::error(int line, std::string message) {
        tree_walk::report(line, "", message);
    }

    void tree_walk::error(token token, std::string message)
    {
        if (token.type == token_type::END_OF_FIELD) {
            tree_walk::report(token.line, " at end", message);
        }
        else {
            tree_walk::report(token.line, " at '" + token.lexeme + "'", message);
        }
    }

    void tree_walk::report(int line, std::string where, std::string message) {
        std::cerr << "[line " << line << "] Error" << where << ": "<< message << std::endl;
        tree_walk::had_error = true;
    }
}