#include <algorithm>
#include <fstream>
#include <set>
#include <string>
#include <vector>

typedef std::pair<short, short> Coords;
typedef std::set<Coords> FlashCache;
typedef std::vector<Coords> FlashList;

//#define PATH "./test_input.txt"
#define PATH "./input.txt"

#define STEPS 200
#define DIM 10
static short jellies[DIM][DIM];

void debugJellies()
{
    printf("---------------\n");
    for (int y = 0; y < DIM; y++)
    {
        for (int x = 0; x < DIM; x++)
        {
            printf(" %i ", jellies[y][x]);
        }
        printf("\n");
    }
}

std::vector<Coords> generateNeighbors(Coords c)
{
    std::vector<Coords> coords;

    short y = std::get<0>(c);
    short x = std::get<1>(c);

    for (short i = -1; i <= 1; i++)
    {
        for (short j = -1; j <= 1; j++)
        {
            if (i != 0 || j != 0)
            {
                short newY = y + i;
                short newX = x + j;

                if (newY < 10 && newY >= 0 && newX < 10 && newX >= 0)
                    coords.push_back(std::make_pair(newY, newX));
            }
        }
    }

    return coords;
}

FlashList incrementJellies()
{
    std::vector<Coords> updateList;

    for (int y = 0; y < DIM; y++)
    {
        for (int x = 0; x < DIM; x++)
        {
            jellies[y][x]++;
            if (jellies[y][x] == 10) updateList.push_back(std::make_pair(y, x));
        }
    }

    return updateList;
}

void handleFlashes(FlashList& toFlash, size_t idx, FlashCache& cache, size_t& count)
{
    const Coords c = toFlash[idx];
    const short y = std::get<0>(c);
    const short x = std::get<1>(c);

    //printf("[FLASH] Handling idx = %ld; (%i, %i); %i\n", idx, y, x, jellies[y][x]);

    if (jellies[y][x] > 9 && cache.find(c) == cache.end())
    {
        cache.insert(c);
        count++;

        for (const Coords& n : generateNeighbors(c))
        {
            const short ny = std::get<0>(n);
            const short nx = std::get<1>(n);
            jellies[ny][nx]++;

            toFlash.push_back(n);
        }
    }
}

int main(int argc, char** argv)
{
    auto file = std::ifstream(PATH);

    std::string line;

    short y = 0;
    while (file >> line)
    {
        for (int x = 0; x < DIM; x++)
        {
            jellies[y][x] = line[x] - '0';
        }
        y++;
    }

    size_t count = 0;
    debugJellies();
    bool allFlashed = false;
    int i = 0;
    while (!allFlashed)
    {
        FlashCache cache;
        FlashList toFlash = incrementJellies();
        //printf("With %ld to update...\n", toFlash.size());

        for (size_t i = 0; i < toFlash.size(); i++)
        {
            handleFlashes(toFlash, i, cache, count);
        }

        for (size_t i = 0; i < toFlash.size(); i++)
        {
            const Coords& c = toFlash[i];

            const short y = std::get<0>(c);
            const short x = std::get<1>(c);

            if (jellies[y][x] > 9) jellies[y][x] = 0;
        }

        allFlashed = cache.size() == DIM * DIM;
        printf("All flashed ? %s\n", (allFlashed) ? "YAYAA" : "WHAT NO");
        i++;
    }
    debugJellies();

    printf("Flash count = %ld, Part 2 = %i\n", count, i);

    return 0;
}
