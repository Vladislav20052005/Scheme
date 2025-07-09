

template <bool IF, typename F, typename S>
struct Conditional {
    static const long long value = F::value;
};

template <typename F, typename S>
struct Conditional<false, F, S> {
    static const long long value = S::value;
};

template <long long N, long long l = 1, long long r = N, long long mid = (l + r) / 2>
struct Sqrt {
    static const long long value =
        Conditional<mid * mid >= N, Sqrt<N, l, mid>, Sqrt<N, mid + 1, r> >::value;
};

template <long long N, long long r>
struct Sqrt<N, r, r, r> {
    static const long long value = r;
};
