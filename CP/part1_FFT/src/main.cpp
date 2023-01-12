#include <iostream>
#include <fstream>
#include <vector>
#include <complex>
#include <algorithm>
#include <iomanip>
#include <limits>
//#include <chrono>

#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"
#include "minimp3_ex.h"

using cf = std::complex<double>;
const double PI = acos(-1);
const uint16_t BLOCK = 4096;
const uint16_t STEP = 1024;
const uint16_t NUMBER_OF_WINDOWS = 1;

std::vector<short> decoder();
void fft(std::vector<cf> &a);
std::vector<cf> hann(const std::vector<short> &tempv);

int main(int argc, char const *argv[])
{
    //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    std::vector<short> pcm = decoder();

    std::vector<short> tempv;
    std::vector<double> maxs_for_each_block;

    unsigned l = 0, r = BLOCK;
    while (r <= pcm.size())
    {
        for (int i = l; i < r; ++i)
        {
            tempv.push_back(pcm[i]);
        }
        std::vector<cf> complex_hanned_pcm = hann(tempv);
        fft(complex_hanned_pcm);
        cf max_real = *max_element(complex_hanned_pcm.begin(), complex_hanned_pcm.end(), [](cf a, cf b)
                                   { return std::abs(a) < std::abs(b); });
        double x = std::abs(max_real);
        maxs_for_each_block.push_back(x);
        tempv.clear();
        l += STEP;
        r += STEP;
    }


    //std::cout << maxs_for_each_block.size() << std::endl;
    for (int cnt = 0; cnt < maxs_for_each_block.size(); ++cnt)
        std::cout << std::fixed << std::setprecision(20) << maxs_for_each_block[cnt] << "\n";

    //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //std::cout << "\nTime difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[µs]" << std::endl;
    return 0;
}

std::vector<short> decoder()
{
	mp3dec_t mp3d;
	mp3dec_file_info_t info;
	if (mp3dec_load(&mp3d, "input.mp3", &info, NULL, NULL))
	{
		std::cout << "Error" << std::endl;
	}
	std::vector<short> res(info.buffer, info.buffer + info.samples);
	return res;
}

void fft(std::vector<cf> &a)
{
    int n = a.size();

    for (int i = 1, j = 0; i < n; i++)
    {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1)
            j ^= bit;
        j ^= bit;

        if (i < j)
            std::swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1)
    {
        double ang = 2 * PI / len;
        cf wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len)
        {
            cf w(1);
            for (int j = 0; j < len / 2; j++)
            {
                cf u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
}

std::vector<cf> hann(const std::vector<short> &tempv)
{
    double multiplier;
    std::vector<double> hanned_pcm;
    for (unsigned elem = 0; elem < BLOCK; ++elem)
    {
        multiplier = 0.5 * (1 - cos(2 * M_PI * elem / (BLOCK - 1)));
        hanned_pcm.push_back(tempv[elem] * multiplier);
    }
    std::vector<cf> complex_hanned_pcm(hanned_pcm.begin(), hanned_pcm.end());
    return std::move(complex_hanned_pcm);
}

/*
[0, 4096)
[1024, 5120)
[2048, 6144)
[3072, 7168)
[4096, 8192)
...
2648448
*/
