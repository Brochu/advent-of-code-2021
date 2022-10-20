#include <algorithm>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

static std::unordered_map<char, char> lut
{
    { '(' , ')' },
    { '[' , ']' },
    { '{' , '}' },
    { '<' , '>' }
};

static std::unordered_map<char, int> lutScore
{
    { ')', 3 },
    { ']', 57 },
    { '}', 1197 },
    { '>', 25137 }
};

static std::unordered_map<char, int> lutScorePart2
{
    { ')', 1 },
    { ']', 2 },
    { '}', 3 },
    { '>', 4 }
};

bool checkCorrupt(const std::string& raw, int& outScore, std::vector<char>& outStack)
{
    for (const char& c : raw)
    {
        if (c == '(' || c == '[' || c == '{' || c == '<')
        {
            outStack.push_back(c);
        }
        else if (c == ')' || c == ']' || c == '}' || c == '>')
        {
            if (lut[outStack.back()] != c)
            {
                //printf("[CORRUPT] checking for %s\n", raw.c_str());
                //printf("Expected: %c and got %c!!\n", lut[outStack.back()], c);
                outScore = lutScore[c];
                return true;
            }
            outStack.pop_back();
        }
    }

    return false;
}

//#define PATH "./test_input.txt"
#define PATH "./input.txt"

int main(int argc, char** argv)
{
    auto file = std::ifstream(PATH);

    std::string line;

    int finalScore = 0;
    std::vector<unsigned long> scoresP2;
    while (file >> line)
    {
        int score = 0;
        std::vector<char> stack;
        if (checkCorrupt(line, score, stack))
        {
            finalScore += score;
        }
        else
        {
            unsigned long score = 0;
            std::reverse(stack.begin(), stack.end());
            for (const char& c : stack)
            {
                score *= 5;
                score += lutScorePart2[lut[c]];
                printf(" %c ", lut[c]);
            }
            printf(" : score = %ld\n ", score);
            scoresP2.push_back(score);
        }
    }

    printf("Final Score = %i\n", finalScore);

    std::sort(scoresP2.begin(), scoresP2.end());
    size_t idx = ((scoresP2.size()) / 2);

    printf("P2 = %ld, %ld, %ld\n", scoresP2[idx], scoresP2.size(), idx);
    return 0;
}
