#pragma once

template <class Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end) : begin_(begin), end_(end) {
    }

    Iterator begin() const {  // NOLINT
        return begin_;
    }

    Iterator end() const {  // NOLINT
        return end_;
    }

private:
    Iterator begin_, end_;
};

template <class T>
class Rng {
public:
    Rng(const T value, const T step) : value_{value}, step_{step} {};
    void operator++() {
        value_ += step_;
    }

    bool operator!=(const Rng iter) {
        return iter.value_ > value_;
    }

    T operator*() {
        return value_;
    }

private:
    T value_, step_;
};

template <class T>
IteratorRange<Rng<T>> Range(T start, T edge, T step) {
    return IteratorRange<Rng<T>>(Rng<T>(start, step), Rng<T>(edge, step));
}

template <class T>
IteratorRange<Rng<T>> Range(T start, T edge) {
    return IteratorRange<Rng<T>>(Rng<T>(start, 1), Rng<T>(edge, 1));
}

template <class T>
IteratorRange<Rng<T>> Range(T edge) {
    return IteratorRange<Rng<T>>(Rng<T>(0, 1), Rng<T>(edge, 1));
}

template <class I1, class I2>
class Zp {
public:
    Zp() = default;
    Zp(I1 iter1, I2 iter2) : iter1_{iter1}, iter2_{iter2} {};
    void operator++() {
        ++iter1_;
        ++iter2_;
    }
    bool operator!=(const Zp oth) {
        return iter1_ != oth.iter1_ && iter2_ != oth.iter2_;
    }
    auto operator*() {
        return std::make_pair(*iter1_, *iter2_);
    }

private:
    I1 iter1_;
    I2 iter2_;
};

template <class I1, class I2>
IteratorRange<Zp<I1, I2>> MakeZip(I1 I1b, I2 I2b, I1 I1e, I2 I2e) {
    return IteratorRange<Zp<I1, I2>>(Zp(I1b, I2b), Zp(I1e, I2e));
}

template <class S1, class S2>
auto Zip(const S1& seq1, const S2& seq2) {
    return MakeZip(seq1.begin(), seq2.begin(), seq1.end(), seq2.end());
}

template <class I>
class SubSeq {
public:
    SubSeq() = default;
    SubSeq(I iter) : iter_{iter} {};
    void operator++() {
        ++iter_;
    }
    bool operator!=(const SubSeq oth) {
        return iter_ != oth.iter_;
    }
    auto operator*() {
        return *iter_;
    }

private:
    I iter_;
};

template <class I>
class Grp {
public:
    Grp() = default;
    Grp(I iter) : iter_{iter} {};
    void operator++() {
        I iter_next = iter_;
        ++iter_next;
        while (*iter_next == *iter_) {
            ++iter_;
            ++iter_next;
        }
        ++iter_;
    }
    bool operator!=(const Grp oth) {
        return iter_ != oth.iter_;
    }
    auto operator*() {
        I iter_last = iter_;
        while (*iter_last == *iter_) {
            ++iter_last;
        }
        return IteratorRange<SubSeq<I>>(iter_, iter_last);
    }

private:
    I iter_;
};

template <class I>
IteratorRange<Grp<I>> MakeGroup(I Ib, I Ie) {
    return IteratorRange<Grp<I>>(Ib, Ie);
}

template <class S>
auto Group(const S& seq) {
    return MakeGroup(seq.begin(), seq.end());
}