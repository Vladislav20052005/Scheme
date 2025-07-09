#pragma once

#include <map>
#include <vector>
#include <string>
#include <memory>

class FullMatch {
public:
    FullMatch(std::string name) : name_(name) {
    }
    bool operator()(std::string name) {
        return name == name_;
    }

private:
    std::string name_;
};

class Substr {
public:
    Substr(std::string substr) : substr_(substr) {
    }
    bool operator()(std::string name) {
        return name.find(substr_) != std::string::npos;
    }

private:
    std::string substr_;
};

class AbstractTest {
public:
    virtual void SetUp() = 0;
    virtual void TearDown() = 0;
    virtual void Run() = 0;
    virtual ~AbstractTest() {
    }
};

class AbstractFabric {
public:
    virtual AbstractTest* Produce() = 0;
    virtual ~AbstractFabric() {
    }
};

template <typename Test>
class Fabric : public AbstractFabric {
public:
    virtual AbstractTest* Produce() override {
        return new Test();
    }
};

class TestRegistry {
public:
    static TestRegistry& Instance() {
        static TestRegistry inst;

        return inst;
    }

    template <class TestClass>
    void RegisterClass(const std::string& class_name) {
        registry_[class_name] = std::make_shared<Fabric<TestClass>>();
    }

    std::unique_ptr<AbstractTest> CreateTest(const std::string& class_name) {
        if (registry_.count(class_name) == 0) {
            throw std::out_of_range("asd");
        }
        return std::unique_ptr<AbstractTest>(registry_[class_name]->Produce());
    }

    void RunTest(const std::string& test_name) {
        auto test = CreateTest(test_name);

        test->SetUp();
        try {
            test->Run();
        } catch (...) {
            test->TearDown();
            throw;
        }
        test->TearDown();
    }

    template <class Predicate>
    std::vector<std::string> ShowTests(Predicate callback) const {
        std::vector<std::string> ret;
        for (auto [key, obj] : registry_) {
            if (callback(key)) {
                ret.push_back(key);
            }
        }
        return ret;
    }

    std::vector<std::string> ShowAllTests() const {
        std::vector<std::string> ret;
        for (auto [key, obj] : registry_) {
            ret.push_back(key);
        }
        return ret;
    }

    template <class Predicate>
    void RunTests(Predicate callback) {
        for (auto [key, obj] : registry_) {
            if (callback(key)) {
                RunTest(key);
            }
        }
    }

    void Clear() {
        registry_.clear();
    }

private:
    std::map<std::string, std::shared_ptr<AbstractFabric>> registry_;
};
