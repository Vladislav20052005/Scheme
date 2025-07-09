#include <huffman.h>

struct Node {
    uint8_t value_;
    std::unique_ptr<Node> left_;
    std::unique_ptr<Node> right_;
    bool IsLeaf() {
        return left_ == nullptr && right_ == nullptr;
    }
};

class HuffmanTree::Impl {
public:
    Impl() {
        tree = std::make_unique<Node>();
        move_ptr_ = tree.get();
    }
    void Build(const std::vector<uint8_t> &code_lengths) {
        std::vector<Node*> current_layer;
        current_layer.reserve(256);
        current_layer.push_back(tree.get());
        for (auto& leaves_cnt : code_lengths) {
            for (auto& parent_node : current_layer) {
                parent_node->left_ = std::make_unique<Node>();
                parent_node->right_ = std::make_unique<Node>();
            }
            std::vector<Node*> new_layer;
            new_layer.reserve(256);
            if (leaves_cnt % 2 == 1) {
                new_layer.push_back(current_layer[leaves_cnt / 2 - 1]->right_.get());
            }
            for (size_t index = leaves_cnt / 2; index < current_layer.size(); ++index) {
                new_layer.push_back(current_layer[index]->left_.get());
                new_layer.push_back(current_layer[index]->right_.get());
            }
            current_layer = std::move(new_layer);
        }
    }

    void Fill(Node* node, const std::vector<uint8_t> &values, size_t& index) {
        if (node->IsLeaf()) {
            node->value_ = values[index];
            ++index;
        } else {
            Fill(node->left_.get(), values, index);
            Fill(node->right_.get(), values, index);
        }
    }

    void Fill(const std::vector<uint8_t> &values, size_t& index) {
        Fill(tree.get(), values, index);
    }

    bool Move(bool bit, int &value) {
        Node* save = move_ptr_;
        move_ptr_ = bit ? move_ptr_->right_.get() : move_ptr_->left_.get();
        if (move_ptr_->IsLeaf()) {
            value = move_ptr_->value_;
            move_ptr_ = save;
            return true;
        }
        return false;
    }

private:
    std::unique_ptr<Node> tree;
    Node* move_ptr_;
};

HuffmanTree::HuffmanTree() = default;

void HuffmanTree::Build(const std::vector<uint8_t> &code_lengths,
                        const std::vector<uint8_t> &values) {
    impl_->Build(code_lengths);
    size_t index = 0;
    impl_->Fill(values, index);
}

bool HuffmanTree::Move(bool bit, int &value) {
    return impl_->Move(bit, value);
}

HuffmanTree::HuffmanTree(HuffmanTree &&) = default;

HuffmanTree &HuffmanTree::operator=(HuffmanTree &&) = default;

HuffmanTree::~HuffmanTree() = default;
