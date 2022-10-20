#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

typedef unsigned long ulong;

#define PATH "./input.txt"

static std::unordered_map<char, short> lutHex
{
    { '0', 0 },
    { '1', 1 },
    { '2', 2 },
    { '3', 3 },
    { '4', 4 },
    { '5', 5 },
    { '6', 6 },
    { '7', 7 },
    { '8', 8 },
    { '9', 9 },
    { 'A', 10 },
    { 'B', 11 },
    { 'C', 12 },
    { 'D', 13 },
    { 'E', 14 },
    { 'F', 15 },
};

std::string parseFile(std::ifstream&& file)
{
    std::string line;
    getline(file, line);
    return line;
}

void debugBits(const std::vector<bool>& bits, ulong start, ulong length)
{
    printf("[DEBUG] ");
    for (ulong i = start; i < (start + length); i++)
    {
        if ((i % 4) == 0 && i != 0)
            printf("%s ", bits[i] ? "1" : "0");
        else
            printf("%s", bits[i] ? "1" : "0");
    }
    printf("\n");
}

std::vector<bool> parseBits(const std::string& line)
{
    std::vector<bool> result;

    for (const char& c : line)
    {
        short value = lutHex[c];
        //printf("[BITS] character = %c; value = %i >> ", c, value);
        for (int i = 3; i >= 0; i--)
        {
            bool b = ((value >> i) % 2) != 0;
            //printf("%s", b ? "1" : "0");

            result.push_back(b);
        }
        //printf("\n");
    }

    //debugBits(result);
    return result;
}

ulong getValue(const std::vector<bool>& bits, ulong& start, ulong length)
{
    ulong value = 0;
    for (ulong i = 0; i < length; i++)
    {
        value <<= 1;
        value += bits[start++];
    }
    return value;
}

struct Header
{
    short version = 0;
    short type = 0;
};
Header fetchHeader(const std::vector<bool>& bits, ulong& start)
{
    short ver = 0;
    for (int i = 0; i < 3; i++)
    {
        ver <<= 1;
        ver += bits[start++];
    }

    short type = 0;
    for (int i = 0; i < 3; i++)
    {
        type <<= 1;
        type += bits[start++];
    }

    return { ver, type };
}

struct LiteralPart
{
    bool isEnd = false;
    ulong value = 0;
};
LiteralPart fetchNextPart(const std::vector<bool>& bits, ulong& start)
{
    bool isEnd = bits[start++];
    ulong value = getValue(bits, start, 4);

    return { !isEnd, value };
}

ulong parsePacket(const std::vector<bool>& packet, ulong& start, ulong& totalVersions)
{
    // Get Header
    Header h = fetchHeader(packet, start);
    //printf("[PACKET] Header: version = %i; type = %i\n", h.version, h.type);
    totalVersions += h.version;

    if (h.type == 4)
    {
        LiteralPart part;
        ulong finalVal = 0;
        while (!part.isEnd)
        {
            part = fetchNextPart(packet, start);
            //printf("[PACKET] Part: isEnd = %s; value = %ld\n", part.isEnd ? "TRUE" : "FALSE", part.value);

            finalVal <<= 4;
            finalVal |= part.value;
        }

        printf("[PACKET] Final literal = %ld\n", finalVal);
        return finalVal;
    }
    else
    {
        bool parseType = packet[start++];
        std::vector<ulong> results;

        if (parseType)
        {
            ulong count = getValue(packet, start, 11);
            //printf("[PACKET] Sub Packets Count = %ld\n", count);

            for (ulong i = 0; i < count; i++)
            {
                results.push_back(parsePacket(packet, start, totalVersions));
            }
        }
        else
        {
            ulong length = getValue(packet, start, 15);
            //printf("[PACKET] Bit Length = %ld\n", length);

            ulong target = start + length;
            while (start < target)
            {
                results.push_back(parsePacket(packet, start, totalVersions));
            }
        }

        printf("[OP] : \n");
        for (const ulong& v : results)
        {
            printf("%ld; ", v);
        }
        printf("\n");

        ulong final = -2;
        switch (h.type)
        {
        case 0:
            printf("SUM");
            final = 0;
            for (const ulong& l : results)
            {
                final += l;
            }
            break;
        case 1:
            printf("PROD");
            final = 1;
            for (const ulong& l : results)
            {
                final *= l;
            }
            break;
        case 2:
            printf("MIN");
            final = *std::min_element(results.cbegin(), results.cend());
            break;
        case 3:
            printf("MAX");
            final = *std::max_element(results.cbegin(), results.cend());
            break;
        case 5:
            printf("GREATER THAN");
            final = results[0] > results[1] ? 1 : 0;
            break;
        case 6:
            printf("LESSER THAN");
            final = results[0] < results[1] ? 1 : 0;
            break;
        case 7:
            printf("EQUAL");
            final = results[0] == results[1] ? 1 : 0;
            break;
        }

        printf(" : %ld\n", final);
        return final;
    }

    // This should not happen...
    return -1;
}

int main(int argc, char** argv)
{
    // Get the string in the file, or hardcode one for debug
    std::ifstream file = std::ifstream(PATH);
    std::string line;
    getline(file, line);

    //std::string line = "880086C3E88112";

    const std::vector<bool> bits = parseBits(line);
    ulong start = 0;
    ulong totalVersions = 0;
    ulong result = parsePacket(bits, start, totalVersions);

    printf("\nResult(part 1) = %ld\n", totalVersions);
    printf("\nResult(part 2) = %ld\n", result);

    return 0;
}
