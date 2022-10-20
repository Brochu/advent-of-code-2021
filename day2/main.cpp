#include <string>
#include <sstream>
#include <fstream>
#include <vector>

static std::string test = "forward 5\ndown 5\nforward 8\nup 3\ndown 8\nforward 2\n";
static std::string path = "./input.txt";
int main(int argc, char** argv)
{
    //auto file = std::stringstream(test);
    auto file = std::ifstream(path);

    // Status variables
    int h = 0;
    int d = 0;
    int a = 0;

    std::string dir;
    int num;
    while(file >> dir >> num)
    {
        if (dir == "forward")
        {
            h += num;
            d += (a * num);
        }
        else if (dir == "down")
        {
            a += num;
        }
        else if (dir == "up")
        {
            a -= num;
        }
    }

    printf("------\nHorizontal: %i, Depth: %i\n", h, d);
    printf("Result: %i\n", h * d);

    return 0;
}
