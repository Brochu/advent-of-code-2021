#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>

static constexpr int STAGES = 9;
static constexpr int DAYS = 256;

static std::string testPath = "./test_input.txt";
static std::string path = "./input.txt";
int main(int argc, char** argv)
{
    //auto file = std::ifstream(testPath);
    auto file = std::ifstream(path);

    int num = 0;
    std::vector<long> lives(STAGES);
    while (file >> num)
    {
        file.ignore(1);

        lives[num]++;
    }

    for (int i = 0; i < DAYS; i++)
    {
        const long newL = lives[0];
        lives[7] += newL;
        lives[0] = 0;
        std::rotate(lives.begin(), lives.begin() + 1, lives.end());
        lives[8] += newL;

    }

    //for (int i = 0; i < STAGES; i++)
    //    printf("%i\n", lives[i]);
    //printf("------\n");

    auto result = std::accumulate(lives.cbegin(), lives.cend(), (unsigned long)0);
    printf("Result = %ld\n", result);
    return 0;
}
