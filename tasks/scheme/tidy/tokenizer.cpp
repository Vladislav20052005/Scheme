#include <tokenizer.h>

bool Tokenizer::IsEnd() {
    while (stream_->peek() == ' ' || stream_->peek() == '\n') {
        stream_->get();
    }
    return stream_->peek() == -1;
}

void Tokenizer::Next() {
    if (got_token_) {
    } else {
        GetToken();
    }
    got_token_ = false;
}

Token Tokenizer::GetToken() {
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
}