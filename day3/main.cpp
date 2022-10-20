#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

static std::string testPath = "./test_input.txt";
static std::string path = "./input.txt";
static constexpr int BIT_COUNT=12;

int main(int argc, char** argv)
{
    //auto file = std::ifstream(testPath);
    auto file = std::ifstream(path);

    std::string val;
    std::vector<std::string> values;
    //std::vector<int> oneCount(BIT_COUNT);
    while(file >> val)
    {
        //for (int i = 0; i < val.length(); i++)
        //{
        //    if (val[i] == '1')
        //        oneCount[i] += 1;
        //}

        values.push_back(val);
    }

    printf("read %ld entries\n", values.size());

    // PART 1
    //int gamma = 0;
    //int epsilon = 0;
    //for (int i = 0; i < oneCount.size(); i++)
    //{
    //    gamma = gamma << 1;
    //    epsilon = epsilon << 1;
    //
    //    printf("%i\n", oneCount[i]);
    //    if (oneCount[i] > values.size() - oneCount[i])
    //    {
    //        gamma++;
    //    }
    //    else
    //    {
    //        epsilon++;
    //    }
    //}

    //printf("gamma = %i; epsilon = %i\n", gamma, epsilon);
    //printf("result = %i\n", gamma * epsilon);

    // PART 2
    int pos = 0;

    int oxy = 0;
    std::vector<std::string> oxyVals = values;

    auto itEnd = oxyVals.end();
    while(std::distance(oxyVals.begin(), itEnd) > 1)
    {
        int oneCount = 0;
        for (auto it = oxyVals.begin(); it != itEnd; it++)
        {
            if ((*it)[pos] == '1') oneCount++;
        }
        //printf("[pos = %i] got oneCount = %i\n", pos, oneCount);

        itEnd = std::remove_if(oxyVals.begin(), itEnd, [&pos, &oneCount, &itEnd, &oxyVals](const std::string& elem)
        {
            if (oneCount >= std::distance(oxyVals.begin(), itEnd) - oneCount)
            {
                return elem[pos] == '0';
            }
            return elem[pos] == '1';
        });
        pos++;
    }
    printf("oxy = %s\n", oxyVals[0].c_str());

    pos = 0;
    int co2 = 0;
    std::vector<std::string> co2Vals = values;

    itEnd = co2Vals.end();
    while(std::distance(co2Vals.begin(), itEnd) > 1)
    {
        int oneCount = 0;
        for (auto it = co2Vals.begin(); it != itEnd; it++)
        {
            if ((*it)[pos] == '1') oneCount++;
        }
        //printf("[pos = %i] got oneCount = %i\n", pos, oneCount);

        itEnd = std::remove_if(co2Vals.begin(), itEnd, [&pos, &oneCount, &itEnd, &co2Vals](const std::string& elem)
        {
            if (oneCount >= std::distance(co2Vals.begin(), itEnd) - oneCount)
            {
                return elem[pos] == '1';
            }
            return elem[pos] == '0';
        });
        pos++;
    }
    printf("co2 = %s\n", co2Vals[0].c_str());

    int oxyVal = std::stoi(oxyVals[0], nullptr, 2);
    int co2Val = std::stoi(co2Vals[0], nullptr, 2);

    printf("oxy = %i; co2 = %i\n", oxyVal, co2Val);
    printf("result = %i\n", oxyVal * co2Val);

    return 0;
}
