#include <parser.h>
#include <error.h>

std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("No token");
    }
    auto next_token = tokenizer->GetToken();
    std::shared_ptr<Object> return_object;
    if (std::holds_alternative<ConstantToken>(next_token)) {
        return_object = std::make_shared<Number>(std::get<ConstantToken>(next_token).value);
    } else if (std::holds_alternative<SymbolToken>(next_token)) {
        return_object = std::make_shared<Symbol>(std::get<SymbolToken>(next_token).name);
    } else if (next_token == Token{BracketToken::OPEN}) {
        return_object = ReadList(tokenizer);
        if (tokenizer->GetToken() != Token{BracketToken::CLOSE}) {
            throw SyntaxError("No closing bracket ')'");
        }
    } else {
        throw SyntaxError("Unexpected token");
    }
    tokenizer->Next();
    return return_object;
}

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
    tokenizer->Next();
    if (tokenizer->GetToken() == Token{BracketToken::CLOSE}) {
        return nullptr;
    }
    std::shared_ptr<Cell> return_object(std::make_shared<Cell>());
    return_object->SetFirst(Read(tokenizer));
    std::shared_ptr<Cell> current_object(return_object);

    while (tokenizer->GetToken() != Token{BracketToken::CLOSE}) {
        if (tokenizer->GetToken() == Token{DotToken{}}) {
            tokenizer->Next();
            current_object->SetSecond(Read(tokenizer));
            return return_object;
        } else {
            current_object->SetSecond(std::make_shared<Cell>());
            current_object = dynamic_pointer_cast<Cell>(current_object->GetSecond());
            current_object->SetFirst(Read(tokenizer));
        }
    }
    return return_object;
}
