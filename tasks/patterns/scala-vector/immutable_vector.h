#pragma once

#include <memory>
#include <iostream>
#include <vector>

const size_t max_depth = 5;
const size_t branching = 32;


template <class T>
class Node {
public:
    Node() : nodes_(std::make_shared<std::vector<Node<T>*>>(32, nullptr)) {}
    ~Node(){}

    

    Node* MoveDown(size_t index){
        return nodes_->at(index);
    }
    void Create(size_t index){
        nodes_->at(index) = (new Node<T>());
    }

    bool CreateRecursive(const T& const_value, size_t depth, size_t target_size, size_t& cnt) {
        std::cout <<123 << nodes_->at(0);
        //this->Create(0);
        if(depth == max_depth){
            value_ = const_value;
            cnt++;
            if(cnt == target_size){
                return true;
            }
            return false;
        }
        for(size_t i = 0; i < branching; i++){
            std::cout << 123 <<"<-";
            //nodes_[0].reset(new Node<T>);
            this->Create(i);
            std::cout <<456;

            auto next = MoveDown(i);
            if(next->CreateRecursive(const_value, depth + 1, target_size, cnt)){
                return true;
            }
        }
        return false;
    }

    const T& Get(size_t index, size_t depth){
        if(depth == max_depth){
            return value_;
        }
        index >>= (5 - depth) * 5;
    }
private:
    std::shared_ptr<std::vector<Node<T>*>> nodes_;
    T value_;
};



template <class T>
class ImmutableVector {
public:
    ImmutableVector() : size_(0) {
        size_t cnt = 0;
        root_->CreateRecursive(T(), 1, 1, cnt);
    }

    explicit ImmutableVector(size_t count, const T& value = T()) : size_(count) {

    }

    template <typename Iterator>
    ImmutableVector(Iterator first, Iterator last) {
    }

    ImmutableVector(std::initializer_list<T> l) {
    }

    ImmutableVector Set(size_t index, const T& value) {
    }

    const T& Get(size_t index) const {
        static T dummy_value;
        return dummy_value;
    }

    ImmutableVector PushBack(const T& value) {
    }

    ImmutableVector PopBack() {
    }

    size_t Size() const {
        return size_;
    }
private:
    std::shared_ptr<Node<T>> root_;
    size_t size_;


};
