#pragma once
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Simple global inline function to load reliability sequence
inline std::vector<uint16_t> load_reliability_sequence(int GF, int N, float SNR)
{
    // Build the file path: ./matrices/GF64/N64/mat_N64_GF64_SNR1.000.txt
    std::ostringstream fname;
    fname << "./matrices/GF" << GF
          << "/N" << N
          << "/mat_N" << N
          << "_GF" << GF
          << "_SNR" << std::fixed << std::setprecision(3) << SNR
          << ".txt";

    std::string   filename = fname.str();
    std::ifstream file(filename);

    if (!file)
    {
        std::cerr << "❌ Cannot open reliability file: " << filename << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::vector<uint16_t> reliab;
    int                   tmp;
    while (file >> tmp)
        reliab.push_back(static_cast<uint16_t>(tmp));

    if (reliab.empty())
    {
        std::cerr << "❌ Reliability file is empty: " << filename << std::endl;
        std::exit(EXIT_FAILURE);
    }

    std::cout << "✅ Loaded " << reliab.size()
              << " reliability values from " << filename << std::endl;

    return reliab;
}
