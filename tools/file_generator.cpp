#include <iostream>
#include <fstream>
#include <random>
#include <bitset>

#define BITS 10240

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(0, (1ULL << 64) - 1);

    // Generate 2048 random bits
    std::bitset<BITS> random_bits;
    int times = BITS >> 5;
    for (int i = 0; i < times; i++)
    {
        uint64_t rand_num = dis(gen);
        random_bits = random_bits<<32;
        random_bits |= std::bitset<BITS>(rand_num);
        
    }

    // Write random bits to file
    std::ofstream outfile1("random_bits.txt", std::ios::binary);
    if (!outfile1.is_open())
    {
        std::cout << "Failed to open random_bits.txt\n";
        return 1;
    }
    outfile1.write(reinterpret_cast<const char*>(&random_bits), sizeof(random_bits));
    outfile1.close();

    // Generate all ones
    std::bitset<BITS> ones_bits;
    ones_bits.set();

    // Write all ones to file
    std::ofstream outfile2("ones_bits.txt", std::ios::binary);
    if (!outfile2.is_open())
    {
        std::cout << "Failed to open ones_bits.txt\n";
        return 1;
    }
    outfile2.write(reinterpret_cast<const char*>(&ones_bits), sizeof(ones_bits));
    outfile2.close();

    // Generate all zeros
    std::bitset<BITS> zero_bits;

    // Write all zeros to file
    std::ofstream outfile3("zero_bits.txt", std::ios::binary);
    if (!outfile3.is_open())
    {
        std::cout << "Failed to open zero_bits.txt\n";
        return 1;
    }
    outfile3.write(reinterpret_cast<const char*>(&zero_bits), sizeof(zero_bits));
    outfile3.close();

    return 0;
}
