#include <algorithm>
#include <climits>
#include <fstream>
#include <math.h>
#include <numeric>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

static short segMap[] {
    1 << 0, //a
    1 << 1, //b
    1 << 2, //c
    1 << 3, //d
    1 << 4, //e
    1 << 5, //f
    1 << 6, //g
};

int countEasyNums(std::string raw)
{
    //printf("%s\n", raw.c_str());
    int count = 0;

    std::stringstream ss(raw);
    std::string current;
    while(std::getline(ss, current, ' '))
    {
        //printf("- %s[%ld]\n", current.c_str(), current.length());

        int len = current.length();
        if (len == 2 || len == 3 || len == 4 || len == 7) count++;
    }

    return count;
}

short segmentsToShort(const std::string& segments)
{
    short result = 0;

    for (const char& c : segments)
        result |= segMap[c - 'a'];

    return result;
}

int countSetBits(short num)
{
    int count = 0;

    while(num)
    {
        num &= (num-1);
        count++;
    }

    return count;
}

struct Entry
{
    std::unordered_map<int, std::vector<short>> hints;
    std::unordered_map<short, int> lutNums;
    std::vector<short> nums;
};
void parseHints(const std::string& raw, Entry& e)
{
    std::stringstream ss(raw);
    std::string current;

    while(std::getline(ss, current, ' '))
        e.hints[current.length()].push_back(segmentsToShort(current));
}
void parseList(const std::string& raw, Entry& e)
{
    std::stringstream ss(raw);
    std::string current;

    while(std::getline(ss, current, ' '))
        e.nums.push_back(segmentsToShort(current));
}
Entry parseEntry(std::string raw)
{
    Entry e;
    size_t idx = raw.find('|');

    parseHints(raw.substr(0, idx), e);
    parseList(raw.substr(idx + 2), e);

    return e;
}

short findSixSegStrategy(std::vector<short>& potentials, short cmp)
{
    short result = 0;

    auto it = potentials.begin();
    for (;it != potentials.end(); it++)
    {
        if ((*it & cmp) == cmp)
        {
            result = *it;
            break;
        }
    }
    potentials.erase(it);

    return result;
}

void findSixSegments(std::vector<short> potentials, short four, short seven, short& outSix, short& outNine, short& outZero)
{
    outNine = findSixSegStrategy(potentials, four);
    outZero = findSixSegStrategy(potentials, seven);
    outSix = potentials.front();

    //printf("Found nine = %i\n", outNine);
    //printf("Found zero = %i\n", outZero);
    //printf("Found six = %i\n", outSix);
}

short findFiveSegStrategy(std::vector<short>& potentials, short cmp)
{
    short result = 0;

    auto it = potentials.begin();
    for (;it != potentials.end(); it++)
    {
        if (countSetBits(*it & cmp) == 2)
        {
            result = *it;
            break;
        }
    }
    potentials.erase(it);

    return result;
}

void findFiveSegments(std::vector<short> potentials, short four, short seven, short& outTwo, short& outThree, short& outFive)
{
    outThree = findSixSegStrategy(potentials, seven);
    outTwo = findFiveSegStrategy(potentials, four);
    outFive = potentials.front();

    //printf("Found three = %i\n", outThree);
    //printf("Found two = %i\n", outTwo);
    //printf("Found five = %i\n", outFive);
}

void calcNums(Entry& e)
{
    //printf("[Calc] -> \n");
    //printf("hints: ");
    //for (int i = 2; i <= 7; i++)
    //{
    //    for (const short h : e.hints[i])
    //    {
    //        printf("%i , ", h);
    //    }
    //}
    //printf("\n");
    e.lutNums[e.hints[2].front()] = 1;
    e.lutNums[e.hints[3].front()] = 7;
    e.lutNums[e.hints[4].front()] = 4;
    e.lutNums[e.hints[7].front()] = 8;

    short six, nine, zero = 0;
    findSixSegments(e.hints[6], e.hints[4].front(), e.hints[3].front(), six, nine, zero);
    e.lutNums[six] = 6;
    e.lutNums[nine] = 9;
    e.lutNums[zero] = 0;

    //printf("\n");

    short two, three, five = 0;
    findFiveSegments(e.hints[5], e.hints[4].front(), e.hints[3].front(), two, three, five);
    e.lutNums[two] = 2;
    e.lutNums[three] = 3;
    e.lutNums[five] = 5;
}

static std::string testPath = "./test_input.txt";
static std::string path = "./input.txt";
int main(int argc, char** argv)
{
    //auto file = std::ifstream(testPath);
    auto file = std::ifstream(path);

    int final = 0;
    std::string line;
    int quantity = 0;
    while (std::getline(file, line))
    {
        std::string raw = line.substr(line.find('|') + 2);
        quantity += countEasyNums(raw);

        Entry e = parseEntry(line);
        calcNums(e);

        int i = 0;
        int total = 0;
        for (auto it = e.nums.crbegin(); it != e.nums.crend(); it++)
        {
            total += (int)(e.lutNums[*it] * pow(10.0, (float)i));
            i++;
        }
        printf("\nResult = %i\n", total);
        final += total;
    }

    printf("\nFINAL = %i\n", final);
    return 0;
}
