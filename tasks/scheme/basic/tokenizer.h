#pragma once

#include <variant>
#include <optional>
#include <istream>
#include <iostream>

inline bool IsNumber(char c) {
    return ('0' <= c) && (c <= '9');
}

inline bool IsStartSymbol(char c) {
    return (('a' <= c) && (c <= 'z')) || (('A' <= c) && (c <= 'Z')) || (c == '<') || (c == '=') ||
           (c == '>') || (c == '*') || (c == '/') || (c == '#');
}

inline bool IsMidSymbol(char c) {
    return IsStartSymbol(c) || IsNumber(c) || (c == '?') || (c == '!') || (c == '-');
}

inline bool IsPlusOrMinus(char c) {
    return (c == '+') || (c == '-');
}

struct SymbolToken {
    std::string name;

    bool operator==(const SymbolToken& other) const {
        return name == other.name;
    }
    template <typename T>
    bool operator==(const T&) const {
        return false;
    }
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const {
        return true;
    }
    template <typename T>
    bool operator==(const T&) const {
        return false;
    }
};

struct DotToken {
    bool operator==(const DotToken&) const {
        return true;
    }
    template <typename T>
    bool operator==(const T&) const {
        return false;
    }
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int value;

    bool operator==(const ConstantToken& other) const {
        return value == other.value;
    }
    template <typename T>
    bool operator==(const T&) const {
        return false;
    }
};

using Token = std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken>;

class Tokenizer {
public:
    Tokenizer(std::istream* in) : stream_(in){};
    bool IsEnd(); /* {
         while (stream_->peek() == ' ' || stream_->peek() == '\n') {
             stream_->get();
         }
         return stream_->peek() == -1;
     }*/

    void Next(); /* {
         if (got_token_) {
         } else {
             GetToken();
         }
         got_token_ = false;
     }*/

    Token GetToken(); /* {
         if (got_token_) {
             return current_token_;
         } else {
             while (stream_->peek() == ' ' || stream_->peek() == '\n') {
                 stream_->get();
             }
             auto c = stream_->get();
             Token ret;
             if (c == 39) {
                 ret = QuoteToken();
             } else if (c == '.') {
                 ret = DotToken();
             } else if (c == '(') {
                 ret = BracketToken::OPEN;
             } else if (c == ')') {
                 ret = BracketToken::CLOSE;
             } else if (IsNumber(c) || IsPlusOrMinus(c)) {
                 std::string ret_string(1, static_cast<char>(c));
                 if (IsPlusOrMinus(c) && !IsNumber(stream_->peek())) {
                     ret = SymbolToken{ret_string};
                 } else {
                     while (IsNumber(stream_->peek())) {
                         ret_string += stream_->get();
                     }
                     ret = ConstantToken{std::stoi(ret_string)};
                 }
             } else if (IsStartSymbol(c)) {
                 std::string sym = "0";
                 sym[0] = c;
                 while (IsMidSymbol(stream_->peek())) {
                     sym += stream_->get();
                 }
                 ret = SymbolToken{sym};
             }
             current_token_ = ret;
             got_token_ = true;
             return ret;
         }
     }*/

private:
    std::istream* stream_;
    Token current_token_;
    bool got_token_ = false;
};