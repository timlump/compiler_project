#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>

#include "token.h"

namespace lox {
    class scanner
    {
        public:
            scanner(std::string source);
            std::vector<token> scan_tokens();

        private:
            bool is_at_end();
            char advance();
            char peek();
            char peek_next();
            bool match(char expected);

            void add_token(token_type type, object value); 

            void add_token(token_type type);

            void scan_token();
            void scan_string();
            void scan_number();
            void scan_identifier();

            bool is_digit(char c);
            bool is_alpha(char c);
            bool is_alphanumeric(char c);

            std::string m_source;
            std::vector<token> m_tokens;
            
            size_t m_start = 0;
            size_t m_current = 0;
            size_t m_line = 1;

            static const std::map<std::string, token_type> KEYWORDS;
    };
}