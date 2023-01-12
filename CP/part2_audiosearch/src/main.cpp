#include <iostream>
#include <fstream>
#include <vector>
#include <complex>
#include <cmath>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <array>
#include <string>
#include <string.h>
#include <unordered_map>
#include <chrono>

#include <set>

#define MINIMP3_IMPLEMENTATION
#include "minimp3.h"
#include "minimp3_ex.h"

using cd = std::complex<double>;
const double PI = acos(-1);
const uint16_t BLOCK = 4096;
const uint16_t STEP = 4096;
const uint16_t NUMBER_OF_WINDOWS = 1;
const float EPS = std::numeric_limits<float>::epsilon();
const std::vector<int> RANGE = {40, 80, 120, 180, 300, 500, 1000};
const int SAMPLE_RATE = 44100;
const int FUZ_FACTOR = 2;

std::vector<long long> getHashes(std::string mp3file);
std::vector<short> decoder(std::string input_file);
void fft(std::vector<cd> &a);
std::vector<cd> hann(const std::vector<short> &tempv);
double taylor_ln(double x);
unsigned max_index(std::vector<cd> v);
int getIndex(int freq);
long long hash(const std::vector<short> &f);

int main(int argc, char const *argv[])
{
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);

    if (!strcmp(argv[1], "index"))
    {
        std::string input_file = argv[3];
        std::string output_file = argv[5];
        std::ifstream infile(input_file);
        std::ofstream outfile;
        outfile.open(output_file);
        std::string mp3file;
        int number_of_files;
        infile >> number_of_files;
        outfile << number_of_files << "\n";
        while (infile >> mp3file)
            outfile << mp3file << "\n";
        infile.clear();
        infile.seekg(0);
        infile >> number_of_files;
        while (infile >> mp3file)
        {
            std::cout << "Indexing " << mp3file << "...\n";
            std::vector<long long> signature = getHashes(mp3file);
            outfile << signature.size() << "\n";
            for (const auto &x : signature)
                outfile << x << " ";
            outfile << "\n";
            std::cout << "DONE\n\n";
        }
    }
    else if (!strcmp(argv[1], "search"))
    {
        std::string base_file = argv[3];
        std::string query_file = argv[5];
        std::string result_file = argv[7];

        int number_of_files;
        std::ifstream base{base_file};
        base >> number_of_files;
        std::vector<std::string> tracks_names; // something like dictionary for names
        std::string track_name;
        for (int i = 0; i < number_of_files; ++i)
        {
            base >> track_name;
            tracks_names.push_back(track_name);
        }

        std::vector<std::vector<long long>> hash_table;
        for (int cnt = 0; cnt < number_of_files; ++cnt)
        {
            unsigned n;
            base >> n;
            std::vector<long long> hashes;
            long long hash;
            for (int j = 0; j < n; ++j)
            {
                base >> hash;
                hashes.push_back(hash);
            }
            hash_table.push_back(hashes);
        }

        std::ifstream query{query_file};
        int number_of_files_query;
        query >> number_of_files_query;
        std::string mp3file;
        std::ofstream res;
        res.open(result_file);
        std::vector<std::string> ans;
        for (int cnt = 0; cnt < number_of_files_query; ++cnt)
        {
            query >> mp3file;
            std::cout << "\n\nFile " << mp3file << ":\n";
            std::cout << "Computing hashes for " << mp3file << std::endl;
            std::vector<long long> signature = getHashes(mp3file);
            std::cout << "DONE\nFinding track in base...\n";
            std::pair<std::pair<long long, unsigned>,
                      std::pair<long long, unsigned>>
                begin_end_time_hash;
            begin_end_time_hash = std::make_pair(std::make_pair(signature[0], 0),
                                                 std::make_pair(signature[signature.size() - 1], signature.size() - 1));
            
            std::vector<std::pair<int, int>> song_data1;
            std::vector<std::pair<int, int>> song_data2;
            for (int i = 0; i < hash_table.size(); ++i)
            {
                for (int j = 0; j < hash_table[i].size(); ++j)
                {
                    if (begin_end_time_hash.first.first == hash_table[i][j])
                    {
                        song_data1.push_back({i, j});
                    }
                    if (begin_end_time_hash.second.first == hash_table[i][j])
                    {
                        song_data2.push_back({i, j});
                    }
                }
            }
            std::cout << "Computing possible matches...\n";
            res << mp3file << std::endl;
/*             res << "start: ";
            for (int i = 0; i < song_data1.size(); ++i)
            {
                res << "(" << tracks_names[song_data1[i].first] << ", " << song_data1[i].second << "); ";
            }
            res << "\nend: ";
            for (int i = 0; i < song_data2.size(); ++i)
            {
                res << "(" << tracks_names[song_data2[i].first] << ", " << song_data2[i].second << "); ";

            } */
            int time_range;
            int sample_time_range = begin_end_time_hash.second.second - begin_end_time_hash.first.second;
            for (int i = 0; i < song_data1.size(); ++i)
            {
                for (int j = 0; j < song_data2.size(); ++j)
                {
                    if (song_data1[i].first == song_data2[j].first)
                    {
                        time_range = song_data2[j].second - song_data1[i].second; 
                        if (time_range == sample_time_range)
                        {
                            ans.push_back(tracks_names[song_data1[i].first]);
                        }
                    }
                }
            }           
            if (ans.size() == 0)
            {
                res << "! NOT FOUND\n\n";
            }
            else
            {
                res << "Matches: ";
                for (const auto &x : ans)
                    res << x << ", ";
                res << "\n\n";
            }
            ans.clear();
            std::cout << "DONE";
            //std::cout << "Found - " << result_mp3file << std::endl;   
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "[Error] Enter search or index after ./a.out\n";
    }

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "\nTime difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[µs]" << std::endl;
    return 0;
}

std::vector<long long> getHashes(std::string mp3file)
{
    std::vector<short> pcm = decoder(mp3file);

    std::vector<short> tempv;
    std::vector<double> maxs_for_each_block;
    std::vector<std::vector<cd>> mag_table;
    std::vector<double> maxs_for_each_block_log10;

    std::cout << pcm.size() << std::endl;
    unsigned l = 0, r = BLOCK;
    while (r <= pcm.size())
    {
        for (int i = l; i < r; ++i)
        {
            tempv.push_back(pcm[i]);
        }
        std::vector<cd> complex_hanned_pcm = hann(tempv);
        fft(complex_hanned_pcm);
        mag_table.push_back(complex_hanned_pcm);
        tempv.clear();
        l += STEP;
        r += STEP;
    }
    double mag;
    short freq;
    short index;
    std::vector<std::vector<short>> powerful_freqs_table;
    std::vector<long long> hashes;
    //
    std::set<long long> diff_hashes;
    //
    for (int i = 0; i < mag_table.size(); ++i)
    {
        std::vector<double> max_mags(7, 0);
        std::vector<short> powerful_freqs(7, 0);
        for (int j = 0; j <= 1000 / (SAMPLE_RATE / BLOCK); ++j)
        {
            mag = std::abs(mag_table[i][j]);
            freq = j * (SAMPLE_RATE / BLOCK);
            index = getIndex(freq);
            if (mag - max_mags[index] >= EPS)
            {
                powerful_freqs[index] = freq;
                max_mags[index] = mag;
            }
        }
        powerful_freqs_table.push_back(powerful_freqs);
        hashes.push_back(hash(powerful_freqs));
        //
        diff_hashes.insert(hash(powerful_freqs));
        //
    }
    return std::move(hashes);
}

std::vector<short> decoder(std::string input_file)
{
    mp3dec_t mp3d;
    mp3dec_file_info_t info;
    if (mp3dec_load(&mp3d, ("../data/" + input_file).c_str(), &info, NULL, NULL))
    {
        std::cout << "Error" << std::endl;
    }
    std::vector<short> res(info.buffer, info.buffer + info.samples);
    std::ofstream out;
    out.open("input.pcm");
    for (const auto &x : res)
        out << x << std::endl;
    return res;
}

void fft(std::vector<cd> &a)
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
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len)
        {
            cd w(1);
            for (int j = 0; j < len / 2; j++)
            {
                cd u = a[i + j], v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
}

std::vector<cd> hann(const std::vector<short> &tempv)
{
    double multiplier;
    std::vector<double> hanned_pcm;
    for (unsigned elem = 0; elem < BLOCK; ++elem)
    {
        multiplier = 0.5 * (1 - cos(2 * M_PI * elem / (BLOCK - 1)));
        hanned_pcm.push_back(tempv[elem] * multiplier);
    }
    std::vector<cd> complex_hanned_pcm(hanned_pcm.begin(), hanned_pcm.end());
    return std::move(complex_hanned_pcm);
}

double taylor_ln(double x)
{
    return (x - 1) - std::pow(x - 1, 2) / 2 + std::pow(x - 1, 3) / 3 - std::pow(x - 1, 4) / 4 +
           std::pow(x - 1, 5) / 5 - std::pow(x - 1, 6) / 6 + std::pow(x - 1, 7) / 7 -
           std::pow(x - 1, 8) / 8 + std::pow(x - 1, 9) / 9 - std::pow(x - 1, 10) / 10 +
           std::pow(x - 1, 11) / 11 - std::pow(x - 1, 12) / 12 + std::pow(x - 1, 13) / 13;
}

unsigned max_index(std::vector<cd> v)
{
    double max = 0.;
    unsigned max_ind = 0;
    for (unsigned i = 0; i < v.size(); ++i)
    {
        if (std::abs(v[i]) > max)
        {
            max = std::abs(v[i]);
            max_ind = i;
        }
    }
    return max_ind;
}

int getIndex(int freq)
{
    int i = 0;
    while (freq > RANGE[i])
        i++;
    return i;
}

long long hash(const std::vector<short> &f)
{
    long long out = 0;
    for (int i = 0; i < f.size(); ++i)
        out += ((f[i] - (f[i] % FUZ_FACTOR)) * std::pow(10, i));
    return out;
}
