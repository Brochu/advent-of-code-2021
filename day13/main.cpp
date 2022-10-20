#include <cstdint>
#include <fstream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

typedef std::pair<int, int> Coord;
typedef std::pair<char, int> Fold;

//#define PATH "./test_input.txt"
#define PATH "./input.txt"

Coord parseCoord(const std::string& line)
{
    //printf("[%ld][coords] * %s\n", line.size(), line.c_str());
    std::stringstream ss (line);
    int x, y;

    ss >> x;
    ss.ignore(1);
    ss >> y;

    return std::make_pair(x, y);
}
void debugCoord(const Coord& c)
{
    printf("Coord: (%i, %i)\n", c.first, c.second);
}

Fold parseFold(const std::string& line)
{
    //printf("[%ld][folds] * %s\n", line.size(), line.c_str());
    size_t idx = line.find('=');

    return std::make_pair(line[idx-1], atoi(line.substr(idx+1).c_str()));
}
void debugFold(const Fold& f)
{
    printf("Fold: (%c, %i)\n", f.first, f.second);
}

void foldX(std::vector<Coord>& coords, int max, int line)
{
    printf("[FOLD X at %i]\n", line);
    for (Coord& c : coords)
    {
        if (c.first < line) continue;
        c.first = (line - (c.first - line));
    }
}
void foldY(std::vector<Coord>& coords, int max, int line)
{
    printf("[FOLD Y at %i]\n", line);
    for (Coord& c : coords)
    {
        if (c.second < line) continue;
        c.second = (line - (c.second - line));
    }
}

int main(int argc, char** argv)
{
    auto file = std::ifstream(PATH);
    int xMax = -1;
    int yMax = -1;
    std::vector<Coord> coords;
    std::vector<Fold> folds;

    std::string line;
    bool swap = false;
    while (getline(file, line))
    {
        if (line.size() <= 1)
        {
            swap = true;
            continue;
        }

        if (!swap)
        {
            auto c = parseCoord(line);
            //debugCoord(c);
            if (c.first > xMax) xMax = c.first;
            if (c.second > yMax) yMax = c.second;

            coords.push_back(c);
        }
        else
        {
            folds.push_back(parseFold(line));
            //debugFold(f);
        }
    }
    printf("xMax = %i; yMax = %i\n", xMax, yMax);

    //Fold f = folds[0];
    for (const Fold& f : folds)
    {
        if (f.first == 'x') foldX(coords, xMax, f.second);
        else if (f.first == 'y') foldY(coords, yMax, f.second);
    }

    std::set<Coord> distinct;
    for (const Coord& c : coords)
    {
        distinct.insert(c);
    }

    for (const Coord& d : distinct)
    {
        printf("(%i, %i)\n", d.first, d.second);
    }

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            const auto c = std::make_pair(j, i);
            if (distinct.find(c) != distinct.end())
                printf("# ");
            else
                printf(". ");
        }
        printf("\n");
    }
    printf("\nResult = %ld\n", distinct.size());
    return 0;
}
