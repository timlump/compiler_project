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
    bool tree_walk::had_runtime_error = false;
    interpreter* tree_walk::m_interpreter = nullptr;

    void tree_walk::run(std::string source) {
        scanner scanner(source);
        auto tokens = scanner.scan_tokens();
        parser psr(tokens);
        std::shared_ptr<expr> exp = psr.parse();

        if (tree_walk::had_error){
            return;
        }
        
        if (m_interpreter == nullptr) {
            m_interpreter = new interpreter();
        }
        m_interpreter->interpret(exp.get());
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

            if (had_error || had_runtime_error) {
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

    void tree_walk::runtime_error(const lox_runtime_exception& e)
    {
        std::cerr << e.m_message << std::endl << "[line " << e.m_token.line << "]" << std::endl;
        tree_walk::had_runtime_error = true; 
    }

    void tree_walk::report(int line, std::string where, std::string message) {
        std::cerr << "[line " << line << "] Error" << where << ": "<< message << std::endl;
        tree_walk::had_error = true;
    }
}