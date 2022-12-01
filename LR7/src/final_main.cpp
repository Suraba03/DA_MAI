#include <cmath>
#include <iostream>

long long length(long long  n) {
    long long l = 0;
    while (n) {
        n /= 10;
        l++;
    }
    return l;
}

int main()
{
    long long n, m, ans;
    std::cin >> n >> m;
    if (n % m == 0) ans = -1;
    else ans = 0;
    while (n != 0) {
        if (length(n) != length(n - 1))
            break;
        long long p10 = pow(10, length(n) - 1);
        if (p10 % m != 0 || p10 == 0) {
            p10 = p10;
        } else {
            p10--;
        }
        ans += n/m - p10/m;
        n /= 10;
    }
    if (ans < 0) std::cout << "0\n";
    else std::cout << ans << "\n";
    return 0;
}