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
    bool IsEnd();

    void Next();

    Token GetToken();

private:
    std::istream* stream_;
    Token current_token_;
    bool got_token_ = false;
};