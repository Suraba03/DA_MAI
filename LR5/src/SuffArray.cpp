#include "SuffArray.h"

std::vector<int>& countSort(std::vector<int>& arr) {
    int max = *max_element(arr.begin(), arr.end());
    int min = *min_element(arr.begin(), arr.end());
    int range = max - min + 1;
  
    std::vector<int> count(range), output(arr.size());
    for (int i = 0; i < arr.size(); i++)
        count[arr[i] - min]++;
  
    for (int i = 1; i < count.size(); i++)
        count[i] += count[i - 1];
  
    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[arr[i] - min] - 1] = arr[i];
        count[arr[i] - min]--;
    }
    return arr = std::move(output);

}

bool IsPrefix(std::string pattern, std::string text, int indexSuff) {
    if (pattern.size() > text.size() - indexSuff)
        return false;
    for (auto a : pattern) {
        if (a != text[indexSuff])
            return false;
        indexSuff++;
    }
    return true;
}

std::vector<int> lcpInit(const std::string& text, const std::vector<int>& suffA)
{
    std::vector<int> lcp(suffA.size(), 0);
	int n = text.length();
	std::vector<int> rank(n);
	for (int i = 0; i < n; ++i) rank[suffA[i]] = i;
	for (int i = 0, k = 0; i < n; ++i)
	{
		if (k > 0) k--;
		if (rank[i] == n - 1) { 
            lcp[n - 1] = -1, k = 0; 
            continue;
        }
		int j = suffA[rank[i] + 1];
		while (std::max(i + k, j + k) < text.length() && text[i + k] == text[j + k]) k++;
		lcp[rank[i]] = k;
	}
    return lcp;
}

std::vector<int> suffix_array(std::vector<int> &s) {

    s.push_back(0);  
    int n = (int) s.size(),
        cnt = 0,
        cls = 0;
    std::vector<int> c(n), p(n);
    
    std::map<int, std::vector<int>> t;
    for (int i = 0; i < n; i++)
        t[s[i]].push_back(i);
    
    
    for (auto &x : t) {
        for (int u : x.second)
            c[u] = cls, p[cnt++] = u;
        cls++;
    }
    
    for (int l = 1; cls < n; l++) {
        std::vector<std::vector<int>> a(cls);
        std::vector<int> _c(n);
        int d = (1 << l) / 2;
        int _cls = cnt = 0;
        
        for (int i = 0; i < n; i++) {
            int k = (p[i] - d + n) % n;
            a[c[k]].push_back(k);
        }
        
        for (int i = 0; i < cls; i++) {
            for (size_t j = 0; j < a[i].size(); j++) {
                if (j == 0 || c[(a[i][j] + d) % n] != c[(a[i][j - 1] + d) % n])
                    _cls++;
                _c[a[i][j]] = _cls - 1;
                p[cnt++] = a[i][j];
            }
        }
        
        c = _c;
        cls = _cls;
    }
    return std::vector<int>(p.begin() + 1, p.end());
}

std::vector<int> Search::search(std::string& pattern)
{
    int L = 0;
    std::vector<int> enterings;
    int R = suffArray.size();
    while (L < R) {
        int m = (L + R) / 2;
        int begin = suffArray[m] - 1;
        int index = 0;
        while (index + begin < text.size() &&
               index < pattern.size() &&
               text[index + begin] == pattern[index]) ++index;

        if (index == pattern.size() ||
           (index == pattern.size() &&
            index + begin == text.size())) {

            enterings.push_back(suffArray[m]);
            int it = m - 1;
            while (it >= L) {
                begin = suffArray[it] - 1;
                index = 0;
                while (index + begin < text.size() &&
                       index < pattern.size() &&
                       text[index + begin] == pattern[index]) ++index;
                if (index == pattern.size() ||
                   (index == pattern.size() &&
                    index + begin == text.size())) enterings.push_back(suffArray[it]);
                --it;
                if (it == -1) break;
            }
            it = m + 1;
            while (it <= R) {
                if (it == text.size()) break;
                begin = suffArray[it] - 1;
                index = 0;
                while (index + begin < text.size() && index < pattern.size() && text[index + begin] == pattern[index]) {
                    ++index;
                }
                if (index == pattern.size() ||
                   (index == pattern.size() &&
                    index + begin == text.size()))
                    enterings.push_back(suffArray[it]);
                ++it;
            }
            break;
        } else if (index + begin == text.size()) {
            L = m + 1;
        } else {
            if (pattern[index] < text[index + begin]) {
                R = m - 1;
            } else {
                L = m + 1;
            }
        }
    }
    if (L == R && L < text.size()) {
        int index = 0;
        int begin = suffArray[L] - 1;
        while (index + begin < text.size() &&
               index < pattern.size() &&
               text[index + begin] == pattern[index]) ++index;
        if (index == pattern.size() ||
           (index == pattern.size() &&
            index + begin == text.size()))
            enterings.push_back(suffArray[L]);
    }
    return enterings;
}