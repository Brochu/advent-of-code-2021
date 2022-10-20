#include <fstream>
#include <string>
#include <vector>

typedef std::pair<long, long> Point;

#define PATH "./input.txt"
//#define PATH "./test_input.txt"

struct Target
{
    Point xrange;
    Point yrange;

    Target(std::ifstream&& file)
    {
        long lowx, highx;
        file.ignore(15);
        file >> lowx;
        file.ignore(2);
        file >> highx;
        xrange = std::make_pair(lowx, highx);

        long lowy, highy;
        file.ignore(4);
        file >> lowy;
        file.ignore(2);
        file >> highy;
        yrange = std::make_pair(lowy, highy);
    }

    void print() const
    {
        printf("(%ld,%ld) : (%ld,%ld)\n", xrange.first, xrange.second, yrange.first, yrange.second);
    }

    long ycmp(long y)
    {
        if (y >= yrange.first && y <= yrange.second) return 0;
        else if (y < yrange.first) return -1;
        else return 1;
    }

    long xcmp(long x)
    {
        if (x >= xrange.first && x <= xrange.second) return 0;
        else if (x < xrange.first) return -1;
        else return 1;
    }

    bool intersect(const Point& p)
    {
        long xRes = xcmp(p.first);
        long yRes = ycmp(p.second);

        return xRes == 0 && yRes == 0;
    }
};

void step(Point& pos, Point& vel)
{
    pos.first += vel.first;
    pos.second += vel.second;

    if (vel.first > 0) vel.first--;
    if (vel.first < 0) vel.first++;
    vel.second--;
}

int main(int argc, char** argv)
{
    Target target(std::ifstream(PATH));
    target.print();

    // PART 1
    //long yvel = 67;
    //long ypos = 0;
    //while (target.ycmp(ypos) >= 0)
    //{
    //    printf("[Y] pos = %ld; vel = %ld; cmp = %ld\n", ypos, yvel, target.ycmp(ypos));

    //    ypos += yvel;
    //    yvel--;
    //}
    //printf("[Y] pos = %ld; vel = %ld; cmp = %ld\n", ypos, yvel, target.ycmp(ypos));

    // PART 2
    unsigned long count = 0;
    for (int x = 0; x < 500; x++)
    {
        for (int y = -500; y < 500; y++)
        {
            if (x == 0 && y == 0) continue;

            Point vel = std::make_pair(x, y);
            Point pos = std::make_pair(0, 0);

            for (int k = 0; k < 1000; k++)
            {
                step(pos, vel);

                if (target.intersect(pos))
                {
                    count++;
                    break;
                }
            }
        }
    }

    printf("\nResult = %ld\n", count);
    return 0;
}
