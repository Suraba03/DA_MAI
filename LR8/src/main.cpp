#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cmath>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);

    size_t n;
    std::cin >> n;
    if (n < 3) {
        std::cout << 0 << std::endl;
        return 0;
    }
    std::vector<uint64_t> vec(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> vec[i];
    }
    std::sort(vec.begin(), vec.end());
    for (size_t i = vec.size() - 1; i > 1; --i) {
        uint64_t a = vec[i];
        uint64_t b = vec[i - 1];
        uint64_t c = vec[i - 2];
        // a >= b >= c
        if (b + c > a) {
            // Heron's formula
            long double p = (a + b + c) / 2.0;
            long double s = sqrt(p * (p - a) * (p - b) * (p - c));
            std::cout << std::fixed << std::setprecision(3) << s << "\n";
            std::cout << c << " " << b << " " << a << std::endl;
            return 0;
        }
    }
    std::cout << 0 << std::endl;
    return 0;
}
