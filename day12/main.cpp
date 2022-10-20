#include <algorithm>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

typedef std::unordered_map<std::string, std::vector<std::string>> LinkMap;
typedef std::unordered_map<std::string, size_t> IndexMap;

//#define PATH "./test_input.txt"
#define PATH "./input.txt"
#define MAX_NODES 20
#define START_NAME "start"
#define END_NAME "end"

struct TrieNode
{
    TrieNode() : isEnd(false)
    {
        for (int i = 0; i < MAX_NODES; i++)
            this->next[i] = nullptr;
    }

    TrieNode* next[MAX_NODES];
    bool isEnd;
};

void buildTrieImpl(
        const LinkMap& linkDB,
        const IndexMap& lutIndex,
        TrieNode* n,
        std::string name,
        std::map<std::string, int> cache,
        std::set<std::string> twice,
        int& count)
{
    // Check if we reached end node, if yes return
    if (name == "end")
    {
        count++;
        return;
    }

    // Get all possible next's
    std::vector<std::string> possible = linkDB.at(name);

    // Filter the possible node based on the cache
    auto filtered = std::remove_if(possible.begin(), possible.end(), [&cache, &twice](const std::string& e)
        {
            auto anyTwice = std::any_of(cache.begin(), cache.end(), [](const std::pair<std::string, int>& e)
                {
                    return islower(e.first[0]) && e.second > 1;
                });

            if (anyTwice)
                return islower(e[0]) && cache[e] > 0;
            else
                return islower(e[0]) && cache[e] > 1;
        });

    printf("[TRIE] Continue with all valid options: \n");
    for (auto it = possible.begin(); it != filtered; it++)
    {
        const size_t idx = lutIndex.at(*it);
        const bool isLower = islower((*it)[0]);
        printf("%s[%ld][%s]\n", it->c_str(), idx, isLower ? "lower" : "upper");

        // Create a new node at the right index of the array
        n->next[idx] = new TrieNode();

        cache[*it]++;
        // Call Impl with the new node
        buildTrieImpl(linkDB, lutIndex, n->next[idx], *it, cache, twice, count);
        cache[*it]--;
    }
}

TrieNode buildTrie(const LinkMap& linkDB, const IndexMap& lutIndex, int& count)
{
    TrieNode root;

    std::map<std::string, int> cache;
    std::set<std::string> twice;
    // Recurse through to build trie
    buildTrieImpl(linkDB, lutIndex, &root, START_NAME, cache, twice, count);

    return root;
}

int main(int argc, char** argv)
{
    auto file = std::ifstream(PATH);
    LinkMap linkDB;
    size_t index = 0;
    IndexMap lutIndex;

    int count = 0;
    std::set<std::string> nodes;

    std::string line;
    while (file >> line)
    {
        std::string from, to;
        const size_t idx = line.find('-');
        from = line.substr(0, idx);
        to = line.substr(idx + 1);
        //printf("* %s -> %s\n", from.c_str(), to.c_str());

        if (from != START_NAME && from != END_NAME && to != START_NAME && to != END_NAME)
        {
            linkDB[from].push_back(to);
            linkDB[to].push_back(from);
        }
        else if (to == START_NAME || from == END_NAME)
            linkDB[to].push_back(from);
        else
            linkDB[from].push_back(to);

        if (nodes.find(from) == nodes.end())
        {
            nodes.insert(from);
            lutIndex[from] = index++;
        }
        if (nodes.find(to) == nodes.end())
        {
            nodes.insert(to);
            lutIndex[to] = index++;
        }
    }

    printf("\nNode count = %ld\n", nodes.size());
    for (const std::string& n : nodes)
    {
        printf("[%ld] Node %s :", lutIndex[n], n.c_str());
        for (const std::string& link : linkDB[n])
        {
            printf("%s, ", link.c_str());
        }
        printf("\n");
    }
    printf("\n");

    TrieNode root = buildTrie(linkDB, lutIndex, count);
    //for (int i = 0; i < MAX_NODES; i++)
    //{
    //    printf("-> %s\n", root.next[i] == nullptr ? "NULL" : "NOT NULL");
    //}

    printf("Result = %i\n", count);
    return 0;
}
