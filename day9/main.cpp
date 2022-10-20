#include <algorithm>
#include <climits>
#include <fstream>
#include <math.h>
#include <numeric>
#include <string>
#include <sstream>
#include <unordered_map>
#include <set>
#include <vector>

typedef std::vector<std::vector<char>> Field;
typedef std::set<std::pair<int, int>> Cache;

short getFieldValue(Field& field, int y, int x)
{
    if (y < 0 || y > field.size()-1) return 10;
    else if (x < 0 || x > field[0].size()-1) return 10;
    else return field[y][x] - '0';
}

void floodField(Field& field, Cache& cache, int& size, int y, int x, short cmp)
{
    if (cmp >= 9) return;

    auto pos = std::make_pair(y, x);
    if (cache.find(pos) != cache.end()) return;

    size++;
    cache.insert(pos);
    //printf("[FLOOD] at (%i, %i) = %i\n", y, x, cmp);

    floodField(field, cache, size, y+1, x, getFieldValue(field, y+1, x));
    floodField(field, cache, size, y-1, x, getFieldValue(field, y-1, x));
    floodField(field, cache, size, y, x-1, getFieldValue(field, y, x-1));
    floodField(field, cache, size, y, x+1, getFieldValue(field, y, x+1));
}

//#define PATH "./test_input.txt"
#define PATH "./input.txt"

int main(int argc, char** argv)
{
    auto file = std::ifstream(PATH);

    Field field;
    Cache cache;
    std::vector<std::vector<int>> preCheck;
    std::string line;

    int j = 0;
    while (file >> line)
    {
        for (int i = 0; i < line.length(); i++)
        {
            if (i == 0) field.push_back(std::vector<char>(line.length()));
            field[j][i] = line[i];

            if (i == 0) preCheck.push_back(std::vector<int>());

            short minone = (i == 0) ? 10 : line[i-1] - '0';
            short addone = (i == line.size() - 1) ? 10 : line[i+1] - '0';
            short val = line[i] - '0';
            //printf("%i : %i : %i\n", minone, val, addone);

            if (val < minone && val < addone) preCheck[j].push_back(i);
        }
        //printf("\n");

        j++;
    }

    for (const std::vector<char>& line : field)
    {
        for (const char e : line)
        {
            printf("%i ", e - '0');
        }
        printf("\n");
    }

    int sum = 0;
    std::vector<int> basins;
    for (int y = 0; y < field.size(); y++)
    {
        for (int x = 0; x < preCheck[y].size(); x++)
        {
            short minone = getFieldValue(field, y-1, preCheck[y][x]);
            short addone = getFieldValue(field, y+1, preCheck[y][x]);
            short val = getFieldValue(field, y, preCheck[y][x]);

            //printf("%i : %i : %i \n", minone, val, addone);
            if (val < minone && val < addone)
            {
                sum += (val+1);

                int size = 0;
                floodField(field, cache, size, y, preCheck[y][x], val);
                basins.push_back(size);
                //printf("\n");
            }
        }
        //printf("\n");
    }

    std::make_heap(basins.begin(), basins.end());
    int total = 1;
    for(int i = 0; i < 3; i++)
    {
        printf("basin size = %i\n", basins.front());
        total *= basins.front();

        std::pop_heap(basins.begin(), basins.end());
        basins.pop_back();
    }

    printf("Final = %i, Total = %i\n", sum, total);

    return 0;
}
