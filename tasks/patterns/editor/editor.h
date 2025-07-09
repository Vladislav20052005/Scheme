#pragma once

#include <string>
#include <vector>
#include <memory>

class AbstractCommand {
public:
    virtual void Do() = 0;
    virtual void Undo() = 0;

    virtual ~AbstractCommand() = default;
};

class TypeCommand : public AbstractCommand {
public:
    TypeCommand(char c, std::string& buffer, size_t& pos) : c_(c), buffer_(buffer), pos_(pos) {
    }

    void Do() {
        buffer_.insert(pos_, 1, c_);
        ++pos_;
    }

    void Undo() {
        buffer_.erase(pos_ - 1, 1);
        --pos_;
    }

private:
    char c_;
    std::string& buffer_;
    std::size_t& pos_;
};

class BackspaceCommand : public AbstractCommand {
public:
    BackspaceCommand(std::string& buffer, size_t& pos) : buffer_(buffer), pos_(pos) {
    }

    void Do() {
        if (pos_ > 0) {
            c_ = buffer_[pos_ - 1];
            buffer_.erase(pos_ - 1, 1);
            --pos_;
        }
    }

    void Undo() {
        buffer_.insert(pos_, 1, c_);
        ++pos_;
    }

private:
    char c_;
    std::string& buffer_;
    std::size_t& pos_;
};

class ShiftCommand : public AbstractCommand {
public:
    ShiftCommand(char direction, std::string& buffer, size_t& pos)
        : direction_(direction), buffer_(buffer), pos_(pos) {
    }

    void Do() {
        if (direction_ == '<') {
            if (pos_ != 0) {
                --pos_;
            }
        } else {
            if (pos_ != buffer_.size()) {
                ++pos_;
            }
        }
    }

    void Undo() {
        if (direction_ == '<') {
            ++pos_;
        } else {
            --pos_;
        }
    }

private:
    char direction_;
    std::string& buffer_;
    std::size_t& pos_;
};

class Editor {
    friend class TypeCommand;

public:
    const std::string& GetText() const {
        return buffer_;
    }

    void Type(char c) {
        relog_.clear();
        log_.push_back(std::make_unique<TypeCommand>(c, buffer_, pos_));
        log_.back()->Do();
    }

    void ShiftLeft() {
        if (pos_ != 0) {
            relog_.clear();
            log_.push_back(std::make_unique<ShiftCommand>('<', buffer_, pos_));
            log_.back()->Do();
        }
    }

    void ShiftRight() {
        if (pos_ != buffer_.size()) {
            relog_.clear();
            log_.push_back(std::make_unique<ShiftCommand>('>', buffer_, pos_));
            log_.back()->Do();
        }
    }

    void Backspace() {
        if (pos_ != 0) {
            relog_.clear();
            log_.push_back(std::make_unique<BackspaceCommand>(buffer_, pos_));
            log_.back()->Do();
        }
    }

    void Undo() {
        if (!log_.empty()) {
            log_.back()->Undo();
            auto p = log_.back().release();
            log_.pop_back();

            relog_.push_back(std::unique_ptr<AbstractCommand>(p));
        }
    }

    void Redo() {
        if (!relog_.empty()) {
            relog_.back()->Do();
            relog_.pop_back();
        }
    }

private:
    std::string buffer_;
    size_t pos_ = 0;
    std::vector<std::unique_ptr<AbstractCommand>> log_;
    std::vector<std::unique_ptr<AbstractCommand>> relog_;
};
