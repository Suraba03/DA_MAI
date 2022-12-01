#include <iostream>
#include <vector>

int min(int a, int b) {
    if (a > b) {
        return b;
    } else {
        return a;
    }
}

std::vector<int> ZBlockFunc(std::string s) {	
	int n = (int) s.length();
	std::vector<int> z(n);
	for (int i = 1, l = 0, r = 0; i < n; ++i) {
		if (i <= r) {
			z[i] = min(r - i + 1, z[i - l]);
        }
		while (i + z[i] < n && s[z[i]] == s[i + z[i]])
			++z[i];
		if (i + z[i] - 1 > r)
			l = i;
            r = i + z[i] - 1;
	}
	return z;
}

std::vector<int> FindEntries(std::string pattern, std::string text) {
	std::vector<int> entries;	
	std::string str = pattern + '@' + text;
	std::vector<int> zfuncVector = ZBlockFunc(str);

	for (int i = pattern.length() + 1; i < zfuncVector.size(); ++i) {
		if (zfuncVector[i] == pattern.length())
			entries.push_back(i - pattern.length() - 1);
	}
	return entries;
}

int main(int argc, char const *argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

	std::string pattern, text;
	std::cin >> text >> pattern;

	std::vector<int> entries = FindEntries(pattern, text);
	for (const auto a : entries)
		std::cout << a << '\n';

    return 0;
}
