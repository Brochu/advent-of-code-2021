#include <fstream>
#include <sstream>
#include <string>
#include <vector>

static std::string test = "199\n200\n208\n210\n200\n207\n240\n269\n260\n263\n";
static std::string path = "input.txt";
int main(int argc, char** argv)
{
    //auto file = std::stringstream(test);
    auto file = std::ifstream(path);

    std::vector<int> depths;
    int depth;
    while(file >> depth)
    {
        depths.push_back(depth);
        //printf("%i, ", depth);
    }
    printf("%ld\n", depths.size());

    //PART 1
    //auto l = depths.cbegin();
    //auto r = depths.cbegin();
    //r++;

    //int count = 0;
    //while(l != depths.cend())
    //{
    //    if (*l < *r) count++;
    //    l++;
    //    r++;
    //}


    int count = 0;
    int prev = -1;
    for (int i = 0; (i+2) < depths.size(); i++)
    {
        int sum = depths[i] + depths[i+1] + depths[i+2];
        if (prev < 0) prev = sum;
        else if (sum > prev)
        {
            count++;
        }
        prev = sum;

        //printf("%i, %i, %i\n", depths[i], depths[i+1], depths[i+2]);
    }

    printf("%i\n", count);

    return 0;
}
