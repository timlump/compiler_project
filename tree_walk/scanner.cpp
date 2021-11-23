#include "scanner.h"
#include "tree_walk.h"

namespace lox
{
    scanner::scanner(std::string source) :
        m_source(source)
    {
    }

    std::vector<token> scanner::scan_tokens()
    {
        while (not is_at_end()) {
            m_start = m_current;
            scan_token();
        }

        add_token(token_type::END_OF_FIELD);
        return m_tokens;
    }

    bool scanner::is_at_end()
    {
        return m_current >= m_source.size();
    }

    char scanner::advance()
    {
        return m_source[m_current++];
    }

    char scanner::peek()
    {
        if (is_at_end()) {
            return '\0';
        }
        return m_source[m_current];
    }

    char scanner::peek_next()
    {
        if (m_current + 1 >= m_source.size()) {
            return '\0';
        }

        return m_source[m_current+1];
    }

    bool scanner::match(char expected) {
        if (is_at_end()) {
            return false;
        }

        if (m_source[m_current] != expected) {
            return false;
        }

        m_current++;
        return true;
    }

    void scanner::add_token(token_type type, object value) {
                token token;
                token.type = type;
                token.lexeme = type != token_type::END_OF_FIELD ? 
                    m_source.substr(m_start,m_current-m_start) : "";
                token.line =  m_line;
                token.value = value;

                m_tokens.push_back(token);
            }

    void scanner::add_token(token_type type)
    {
        add_token(type, object(nullptr));
    }

    void scanner::scan_token()
    {
        char c = advance();
        switch (c) {
            // single character operators
            case '(':
                add_token(token_type::LEFT_PAREN);
                break;
            case ')':
                add_token(token_type::RIGHT_PAREN);
                break;
            case '{':
                add_token(token_type::LEFT_BRACE);
                break;
            case '}':
                add_token(token_type::RIGHT_BRACE);
                break;
            case ',':
                add_token(token_type::COMMA);
                break;
            case '.':
                add_token(token_type::DOT);
                break;
            case '-':
                add_token(token_type::MINUS);
                break;
            case '+':
                add_token(token_type::PLUS);
                break;
            case ';':
                add_token(token_type::SEMICOLON);
                break;
            case '*':
                add_token(token_type::STAR);
                break;

            // possibly two character operators
            case '!':
                add_token(match('=') ? token_type::BANG_EQUAL : token_type::BANG);
                break;

            case '=':
                add_token(match('=') ? token_type::EQUAL_EQUAL : token_type::EQUAL);
                break;

            case '<':
                add_token(match('=') ? token_type::LESS_EQUAL : token_type::LESS);
                break;

            case '>':
                add_token(match('=') ? token_type::GREATER_EQUAL : token_type::GREATER);
                break;

            // longer lexemes
            case '/':
                if (match('/')) {
                    while(peek() != '\n' && not is_at_end()) {
                        advance();
                    }
                } else {
                    add_token(token_type::SLASH);
                }
                break;

            // string literals
            case '"':
                scan_string();
                break;

            // whitespace
            case ' ':
            case '\r':
            case '\t':
                break;

            case '\n':
                m_line++;
                break;

            default:
                if (is_digit(c)) {
                    scan_number();
                }
                else if (is_alpha(c)) {
                    scan_identifier();
                }
                else {
                    tree_walk::error(m_line, "Unexpected character.");
                }

                break;
        }
    }

    void scanner::scan_string() {
        while (peek() != '"' && not is_at_end()) {
            if (peek() == '\n') {
                m_line++;
            }
            advance();
        }

        if (is_at_end()) {
            tree_walk::error(m_line, "Unterminated string.");
            return;
        }

        // the closing ".
        advance();

        // trim the surrounding quotes
        int start = m_start + 1;
        int length = m_current - start;
        
        object value(m_source.substr(start, length - 1));
        add_token(token_type::STRING, value);
    }

    void scanner::scan_number() {
        while(is_digit(peek())) {
            advance();
        }

        // look for the fractional part
        if (peek() == '.' && is_digit(peek_next())) {
            // consume the '.'
            advance();

            while (is_digit(peek())) {
                advance();
            }
        }

        int length = m_current - m_start;
        double value = std::stod(m_source.substr(m_start, length));
        add_token(token_type::NUMBER, object(value));
    }

    void scanner::scan_identifier()
    {
        while(is_alphanumeric(peek())) {
            advance();
        }

        token_type type = token_type::IDENTIFIER;
        int length = m_current - m_start;
        std::string text = m_source.substr(m_start, length);
        auto find_iter = KEYWORDS.find(text);
        if (find_iter != KEYWORDS.end()) {
            type = find_iter->second;
        }

        add_token(type);
    }

    bool scanner::is_alpha(char c)
    {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    bool scanner::is_digit(char c)
    {
        return c >= '0' && c <= '9';
    }

    bool scanner::is_alphanumeric(char c)
    {
        return is_alpha(c) || is_digit(c);
    }

    const std::map<std::string, token_type> scanner::KEYWORDS = {
        {"and", token_type::AND},
        {"class", token_type::CLASS},
        {"else", token_type::ELSE},
        {"false", token_type::FALSE},
        {"for", token_type::FOR},
        {"fun", token_type::FUN},
        {"if", token_type::IF},
        {"nil", token_type::NIL},
        {"or", token_type::OR},
        {"print", token_type::PRINT},
        {"return", token_type::RETURN},
        {"super", token_type::SUPER},
        {"this", token_type::THIS},
        {"true", token_type::TRUE},
        {"var", token_type::VAR},
        {"while", token_type::WHILE}
    };
}