#include <algorithm>
#include <assert.h>
#include <fstream>
#include <math.h>
#include <sstream>
#include <string>
#include <set>
#include <vector>

typedef unsigned long ulong;
typedef std::set<ulong> DistCache;

//#define PATH "./input.txt"
#define PATH "./test_input.txt"

#define MAX 30

struct Beacon
{
    long x;
    long y;
    long z;

    std::string toString() const
    {
        std::string out;
        sprintf(&out[0], "{%ld, %ld, %ld}", x, y, z);
        return out;
    }
};

struct Scanner
{
    std::vector<Beacon> beacons;
    ulong sqDistMatrix[MAX][MAX];
    DistCache sqDists;

    void debug() const
    {
        for (const auto& b : beacons)
        {
            printf("* %s\n", b.toString().c_str());
        }
        printf("\n");
    }

    void debugDist() const
    {
        for (int i = 0; i < beacons.size(); i++)
        {
            for (int j = i+1; j < beacons.size(); j++)
            {
                printf("%ld ", sqDistMatrix[i][j]);
            }
            printf("\n");
        }
    }

    ulong calcDistSq(int firstIdx, int secondIdx)
    {
        const Beacon& first = beacons[firstIdx];
        const Beacon& second = beacons[secondIdx];

        const ulong delX = std::pow(first.x - second.x, 2);
        const ulong delY = std::pow(first.y - second.y, 2);
        const ulong delZ = std::pow(first.z - second.z, 2);

        return delX + delY + delZ;
    }

    void buildDistMatrix()
    {
        for (int i = 0; i < beacons.size(); i++)
        {
            for (int j = i+1; j < beacons.size(); j++)
            {
                const ulong dist = calcDistSq(i, j);
                sqDistMatrix[i][j] = dist;
                sqDists.insert(dist);
            }
        }
    }

    int findBeacon(ulong dist) const
    {
        for (int i = 0; i < beacons.size(); i++)
        {
            for (int j = i+1; j < beacons.size(); j++)
            {
                if (sqDistMatrix[i][j] == dist)
                    return i;
            }
        }

        return {};
    }

    Scanner(){}
    Scanner(std::vector<std::string> lines)
    {
        for (int i = 1; i < lines.size(); i++)
        {
            std::stringstream ss(lines[i]);

            long x;
            ss >> x;
            ss.ignore(1);

            long y;
            ss >> y;
            ss.ignore(1);

            long z;
            ss >> z;

            beacons.push_back(Beacon { x, y, z });
        }

        // Make sure we can store the beacons distances in the matrix
        assert(beacons.size() < MAX);
        buildDistMatrix();
    }
};

struct Overlap
{
    int firstScan;
    std::vector<int> fBeaconIdx;

    int secondScan;
    std::vector<int> sBeaconIdx;

    Overlap(int first, int second)
        : firstScan(first), secondScan(second)
    {
        printf("[OVERLAP] Finding all beacons between scan %i and %i\n", firstScan, secondScan);
    }
};

std::vector<Scanner> parseFile(std::ifstream&& file)
{
    std::vector<Scanner> results;

    std::vector<std::string> lines;
    std::string line;
    while (getline(file, line))
    {
        if (line.size() > 0)
        {
            lines.push_back(line);
        }
        else if (line.size() <= 0)
        {
            results.push_back(Scanner(lines));
            lines.clear();
        }
    }

    results.push_back(Scanner(lines));
    lines.clear();

    return results;
}

std::vector<Overlap> findOverlaps(const std::vector<Scanner>& scans)
{
    std::vector<Overlap> overlaps;

    for (int i = 0; i < scans.size(); i++)
    {
        for (int j = i+1; j < scans.size(); j++)
        {
            const DistCache& first = scans[i].sqDists;
            const DistCache& second = scans[j].sqDists;

            std::vector<ulong> common(first.size() + second.size());
            auto it = std::set_intersection(first.begin(), first.end(), second.begin(), second.end(), common.begin());
            common.resize(it - common.begin());

            //printf("{%i, %i} have %ld in common\n", i, j, common.size());

            if (common.size() >= 66)
                overlaps.push_back(Overlap(i, j));
        }
    }

    return overlaps;
}

int main(int argc, char** argv)
{
    std::vector<Scanner> scans = parseFile(std::ifstream(PATH));
    //for (const Scanner& s : scans)
    //{
    //    printf("[%ld]\n", s.beacons.size());
    //    s.debug();
    //}

    //for (const Scanner& s : scans)
    //{
    //    s.debugDist();
    //}

    std::vector<Overlap> overlaps = findOverlaps(scans);
    for (const auto& p : overlaps)
    {
        printf("[OVERLAP] (%i, %i)\n", p.firstScan, p.secondScan);
    }

    return 0;
}
