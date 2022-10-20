#include <algorithm>
#include <climits>
#include <fstream>
#include <string>
#include <vector>
#include <numeric>

int calcFuel(int pos, int target)
{
    int n = abs(target - pos);

    return (n * (n + 1)) / 2;
}

static std::string testPath = "./test_input.txt";
static std::string path = "./input.txt";
int main(int argc, char** argv)
{
    //auto file = std::ifstream(testPath);
    auto file = std::ifstream(path);

    std::vector<int> values;
    int value = 0;
    int minVal = INT_MAX;
    int maxVal = INT_MIN;
    while (file >> value)
    {
        file.ignore(1);
        values.push_back(value);

        if (value < minVal) minVal = value;
        if (value > maxVal) maxVal = value;

        printf("%i\n", value);
    }

    printf("min = %i; max = %i\n", minVal, maxVal);
    int minSum = INT_MAX;
    for (int target = minVal; target <= maxVal; target++)
    {
        int sum = 0;
        for (const int& i : values) sum += calcFuel(i, target);

        if (sum < minSum) minSum = sum;
    }

    printf("Result = %i\n", minSum);
    return 0;
}
