#include <huffman.h>

struct Node {
    uint8_t value_;
    std::unique_ptr<Node> left_;
    std::unique_ptr<Node> right_;
    bool is_leaf_ = false;
    bool IsTerminal() {
        return is_leaf_;
    }
};

class HuffmanTree::Impl {
public:
    Impl() : tree_(), move_ptr_() {
    }

    void Build(const std::vector<uint8_t> &code_lengths, const std::vector<uint8_t> &values) {
        if (code_lengths.size() > 16) {
            throw std::invalid_argument("code lengths size is more than 16");
        }
        size_t sum = 0;
        for (auto &cl : code_lengths) {
            sum += cl;
        }
        if (sum != values.size()) {
            throw std::invalid_argument("incorrect values cnt");
        }
        tree_ = std::make_unique<Node>();
        move_ptr_ = tree_.get();
        std::vector<uint8_t> set(code_lengths.size());
        set.resize(code_lengths.size());
        for (const uint8_t &value : values) {
            if (!Try(tree_.get(), value, code_lengths, set, -1)) {
                throw std::invalid_argument("123");
            }
        }
        for (size_t i = 0; i < code_lengths.size(); i++) {
            if (set[i] != code_lengths[i]) {
                throw std::invalid_argument("no");
            }
        }
    }

    bool Try(Node *curr, uint8_t value, const std::vector<uint8_t> &cl, std::vector<uint8_t> &set,
             int depth) {
        if (curr->IsTerminal()) {
            return false;
        }
        if (depth >= 0 && cl[depth] != set[depth]) {
            curr->value_ = value;
            set[depth]++;
            curr->is_leaf_ = true;
            return true;
        }
        if (!curr->left_) {
            curr->left_ = std::make_unique<Node>();
        }
        if (Try(curr->left_.get(), value, cl, set, depth + 1)) {
            return true;
        }
        if (!curr->right_) {
            curr->right_ = std::make_unique<Node>();
        }
        if (Try(curr->right_.get(), value, cl, set, depth + 1)) {
            return true;
        }

        return false;
    }

    bool Move(bool bit, int &value) {
        if (move_ptr_ == nullptr) {
            throw std::invalid_argument("");
        }
        move_ptr_ = bit ? move_ptr_->right_.get() : move_ptr_->left_.get();
        if (move_ptr_ == nullptr) {
            throw std::invalid_argument("");
        }
        if (move_ptr_->IsTerminal()) {
            value = move_ptr_->value_;
            move_ptr_ = tree_.get();
            return true;
        }
        return false;
    }

private:
    std::unique_ptr<Node> tree_;
    Node *move_ptr_;
};

HuffmanTree::HuffmanTree() : impl_(std::make_unique<Impl>()) {
}

void HuffmanTree::Build(const std::vector<uint8_t> &code_lengths,
                        const std::vector<uint8_t> &values) {
    impl_->Build(code_lengths, values);
}

bool HuffmanTree::Move(bool bit, int &value) {
    return impl_->Move(bit, value);
}

HuffmanTree::HuffmanTree(HuffmanTree &&) = default;

HuffmanTree &HuffmanTree::operator=(HuffmanTree &&) = default;

HuffmanTree::~HuffmanTree() = default;
