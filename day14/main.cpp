#include <algorithm>
#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>


//#define PATH "./test_input.txt"
#define PATH "./input.txt"

#define STEPS 40
#define WINDOW 2

//TODO: Remember this, what is needed to use custom type as key for unordered containers
struct Pair
{
    char l;
    char r;

    bool operator==(const Pair& rhs) const
    {
        return this->l == rhs.l &&  this->r == rhs.r;
    }
};
template<> struct std::hash<Pair>
{
    std::size_t operator() (const Pair &s) const noexcept
    {
        std::size_t h1 = std::hash<char>{}(s.l);
        std::size_t h2 = std::hash<char>{}(s.r);

        return h1 ^ (h2 << 1);
    }
};
typedef std::unordered_map<Pair, char> RuleMap;

void debugPairFr(const std::unordered_map<Pair, size_t>& fr)
{
    for (auto [pair, count] : fr)
    {
        printf("[PFR] '%c%c' >> %ld\n", pair.l, pair.r, count);
    }
}
void debugCharFr(const std::vector<uint64_t>& fr)
{
    for (int i = 0; i < 26; i++)
    {
        printf("[CFR] '%c' >> %ld\n", i + 'A', fr[i]);
    }
}
void debugRules(const RuleMap& rules)
{
    for (auto [pair, c] : rules)
    {
        printf("[RULE] '%c%c' >> %c\n", pair.l, pair.r, c);
    }
}

void step(const RuleMap& rules, std::unordered_map<Pair, uint64_t>& pairs, std::vector<uint64_t>& chars)
{
    printf("[STEPPING]\n");
    auto newPairs = pairs;
    auto newChars = chars;

    for (auto [pair, count] : pairs)
    {
        if (rules.count(pair) == 0) continue;
        char c = rules.at(pair);
        //printf("Add '%c'\n", c);

        newChars[c - 'A'] += count;
        newPairs[pair] -= count;

        newPairs[Pair{pair.l, c}] += count;
        newPairs[Pair{c, pair.r}] += count;
    }

    chars = std::move(newChars);
    pairs = std::move(newPairs);
}

int main(int argc, char** argv)
{
    auto file = std::ifstream(PATH);
    RuleMap rules;

    std::vector<uint64_t> char_fr(26, 0);
    std::unordered_map<Pair, uint64_t> pair_fr;

    std::string line;
    while (getline(file, line))
    {
        if (line.size() <= 1) continue;

        auto idx = line.find('-');
        if (idx != std::string::npos)
        {
            std::string in = line.substr(0, idx-1);
            rules.emplace(Pair{in[0], in[1]}, line.substr(idx+3)[0]);
        }
        else
        {
            std::string start = line;
            for (int i = 0; i + WINDOW <= start.size(); i++)
            {
                //printf("[%i] '%c%c'\n", i, start[i], start[i+1]);
                pair_fr[Pair{start[i], start[i+1]}]++;
                char_fr[start[i] - 'A']++;
            }
            char_fr[start[start.size()-1] - 'A']++;
        }
    }

    debugPairFr(pair_fr);
    printf("\n");
    debugCharFr(char_fr);

    for (int i = 0; i < STEPS; i++)
    {
        step(rules, pair_fr, char_fr);
    }
    debugCharFr(char_fr);

    auto end = std::remove_if(char_fr.begin(), char_fr.end(), [](const uint64_t& e){ return e == 0; });
    //for (auto it = char_fr.begin(); it != end; it++)
    //{
    //    printf("%ld\n", *it);
    //}
    auto res = std::minmax_element(char_fr.begin(), end);

    printf("min = %ld, max = %ld\n", *res.first, *res.second);
    printf("Result = %ld\n", *res.second - *res.first);
    return 0;
}
