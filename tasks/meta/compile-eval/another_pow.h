#pragma once

constexpr int pow(int a, int b) {
    return (b == 0) ? 1 : pow(a, b - 1) * a;
}
