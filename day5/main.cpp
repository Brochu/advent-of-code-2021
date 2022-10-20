#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct Line
{
    int x0, x1;
    int y0, y1;

    Line(int xstart, int ystart, int xend, int yend)
        :x0(xstart), y0(ystart), x1(xend), y1(yend)
    { }

    bool isDiag() const
    {
        return abs(x0 - x1) == abs(y0 - y1);
    }
};

Line parseLine(std::string rawLine, int& minX, int& minY, int& maxX, int& maxY)
{
    int x0, x1 = 0;
    int y0, y1 = 0;
    std::sscanf(rawLine.c_str(), "%i,%i -> %i,%i", &x0, &y0, &x1, &y1);

    if (x0 < minX) minX = x0;
    if (x0 > maxX) maxX = x0;
    if (x1 < minX) minX = x1;
    if (x1 > maxX) maxX = x1;

    if (y0 < minY) minY = y0;
    if (y0 > maxY) maxY = y0;
    if (y1 < minY) minY = y1;
    if (y1 > maxY) maxY = y1;

    //printf("[parse line] (%i, %i) -> (%i, %i)\n", x0, y0, x1, y1);
    return Line(x0, y0, x1, y1);
}

static std::string testPath = "./test_input.txt";
static std::string path = "./input.txt";
int main(int argc, char** argv)
{
    //auto file = std::ifstream(testPath);
    auto file = std::ifstream(path);

    std::string line;
    std::vector<Line> lines;

    int minX, minY = INT32_MAX;
    int maxX, maxY = INT32_MIN;

    while(std::getline(file, line))
    {
        lines.push_back(parseLine(line, minX, minY, maxX, maxY));
    }
    printf("Parsed %ld lines\n", lines.size());
    //printf("min/max\n");
    //printf("x: (%i, %i)\n", minX, maxX);
    //printf("y: (%i, %i)\n", minY, maxY);
    //printf("\n");

    //for(const Line& l : lines)
    //{
    //    printf("line horizontal or vertical : %s\n", l.isDiag() ? "DIAG" : "NO");
    //}

    //TODO: Find intersections between all lines that are not diagonal
    const int Xcount = ((maxX - minX) + 1);
    const int Ycount = ((maxY - minY) + 1);
    int status[Xcount * Ycount];
    std::fill_n(status, Xcount * Ycount, 0);

    for(const Line& l : lines)
    {
        if (l.x0 == l.x1)
        {
            // Line is horizontal
            for(int y = std::min(l.y0, l.y1); y <= std::max(l.y0, l.y1); y++)
            {
                int idx = y * Ycount + l.x0;
                status[idx]++;
                //printf("(%i) ", idx);
            }
        }
        else if (l.y0 == l.y1)
        {
            // Line is vertical
            for(int x = std::min(l.x0, l.x1); x <= std::max(l.x0, l.x1); x++)
            {
                int idx = l.y0 * Ycount + x;
                status[idx]++;
                //printf("(%i) ", idx);
            }
        }
        else if (l.isDiag())
        {
            // Line is perfect diagonal
            int x = l.x0;
            int y = l.y0;

            while(x != l.x1)
            {
                printf("(%i, %i) ", x, y);
                int idx = y * Ycount + x;
                status[idx]++;

                if (x < l.x1) x++;
                else x--;

                if (y < l.y1) y++;
                else y--;
            }
            printf("(%i, %i) ", x, y);
            int idx = y * Ycount + x;
            status[idx]++;
        }
        else
        {
            // Strange case, no handle
        }
        printf("\n");
    }

    int result = 0;
    for(int i = 0; i < Xcount * Ycount; i++)
    {
        if (i % Xcount == 0) printf("\n");
        printf("%i ", status[i]);
        if (status[i] > 1) result++;
    }
    printf("\n");

    printf("Result = %i\n", result);
    return 0;
}
