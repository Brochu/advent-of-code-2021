#include <array>
#include <cstdint>
#include <fstream>
#include <queue>
#include <set>
#include <string>

//#define PATH "./test_input.txt"
#define PATH "./input.txt"
#define DIM 100
#define TILES 5

typedef std::array<std::array<short, DIM>, DIM> Grid;
typedef std::pair<int, int> Coord;

Grid parseGrid(std::ifstream& file)
{
    Grid g;

    std::string line;
    short y = 0;
    while (getline(file, line))
    {
        for (short x = 0; x < line.length(); x++)
            g[y][x] = line[x] - '0';

        y++;
    }

    return g;
}

short fetchCoord(const Grid& g, int y, int x)
{
    //printf("[FETCH] (%i, %i), (%i, %i), (%i, %i)\n", x, y, x % DIM, y % DIM, x / DIM, y / DIM);
    if (y >= DIM * TILES || y < 0)
        return 10;
    else if (x >= DIM * TILES || x < 0)
        return 10;

    int rx = x % DIM;
    int ry = y % DIM;

    int add = x / DIM;
    add += y / DIM;

    short val = g[ry][rx] + add;
    while (val > 9) val -= 9;

    return val;
}

struct Node
{
    uint pos_x;
    uint pos_y;

    uint64_t dist;

    Coord getCoord() { return std::make_pair(pos_x, pos_y);  }
};

std::vector<Node> GenerateNeighbours(const Node& n)
{
    std::vector<Node> res;
    if (n.pos_x < (DIM * TILES) - 1)
        res.push_back(Node{ n.pos_x + 1, n.pos_y });

    if (n.pos_y < (DIM * TILES) - 1)
        res.push_back(Node{ n.pos_x, n.pos_y + 1 });

    if (n.pos_x > 0)
        res.push_back(Node{ n.pos_x - 1, n.pos_y });

    if (n.pos_y > 0)
        res.push_back(Node{ n.pos_x, n.pos_y - 1 });

    return res;
}

uint64_t dijkstra(const Grid& g, uint start, uint end)
{
    //printf("(%i, %i) to (%i, %i)\n", start, start, end, end);
    auto cmp = [](const Node& l, const Node& r){ return l.dist > r.dist; };
    std::priority_queue<Node, std::vector<Node>, decltype(cmp)> Q(cmp);
    std::set<Coord> visited;

    std::array<std::array<uint64_t, DIM * TILES>, DIM * TILES> dist;
    for (int i = 0; i < DIM * TILES; i++)
    {
        for (int j = 0; j < DIM * TILES; j++)
        {
            dist[i][j] = INT64_MAX;
        }
    }
    dist[start][start] = 0;
    Q.push(Node{ start, start, 0 });

    while(!Q.empty())
    {
        auto q = Q.top();
        Q.pop();

        if (visited.find(q.getCoord()) == visited.end())
        {
            //printf("Checking (%i, %i) [%i] : ", q.pos_x, q.pos_y, fetchCoord(g, q.pos_y, q.pos_x));

            visited.insert(q.getCoord());
            for (Node n : GenerateNeighbours(q))
            {
                uint64_t newDist = dist[q.pos_y][q.pos_x] + fetchCoord(g, n.pos_y, n.pos_x);
                //printf("(%i, %i) [%ld][%ld] ; ", n.pos_x, n.pos_y, newDist, dist[n.pos_y][n.pos_x]);

                if (newDist < dist[n.pos_y][n.pos_x])
                {
                    dist[n.pos_y][n.pos_x] = newDist;

                    n.dist = newDist;
                    Q.push(n);
                }
            }
            printf("\n");
        }
    }

    //for (int i = 0; i < DIM * TILES; i++)
    //{
    //    for (int j = 0; j < DIM * TILES; j++)
    //    {
    //        printf("%ld ", dist[i][j]);
    //    }
    //    printf("\n");
    //}
    printf("%ld\n", dist[end][end]);

    return 0;
}

int main(int argc, char** argv)
{
    std::ifstream file = std::ifstream(PATH);
    Grid grid = parseGrid(file);

    //for (int i = 0; i < DIM * TILES; i++)
    //{
    //    for (int j = 0; j < DIM * TILES; j++)
    //    {
    //        printf("%i ", fetchCoord(grid, i, j));
    //    }
    //    printf("\n");
    //}

    uint64_t risk = dijkstra(grid, 0, (DIM * TILES) - 1);
    printf("\nResult = %ld\n", risk);
    return 0;
}
