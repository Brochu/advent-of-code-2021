#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

typedef std::unordered_map<int, std::vector<int>> CacheMap;
typedef std::unordered_map<int, int> CacheVals;
static constexpr int BOARD_SIZE = 5;

std::vector<int> initDrawList(const std::string& rawList)
{
    std::vector<int> result;
    printf("Got the draw list: %s\n", rawList.c_str());

    std::stringstream ss(rawList);
    std::string val;
    while(getline(ss, val, ','))
    {
        result.push_back(atoi(val.c_str()));
    }

    return result;
}

void parseBoard(std::vector<std::string>& rawBoard, CacheMap& lut, CacheVals& lutVals, std::vector<bool>& boards, int& count)
{
    for(int i = 0; i < rawBoard.size(); i++)
    {
        //printf("%s\n", rawBoard[i].c_str());
        std::stringstream ss(rawBoard[i]);
        int num;
        while(ss >> num)
        {
            lutVals[count] = num;
            lut[num].push_back(count++);
            boards.push_back(false);
        }
    }
    //printf("--------------------\n");
}

void debugBoard(std::vector<bool>& boards)
{
    for (int i = 0; i < boards.size(); i++)
    {
        if (i % 25 == 0) printf("\n\nBOARD %i", i / 25);
        if (i % 5 == 0) printf("\n");
        printf("%s ", boards[i] ? "X" : "0");
    }

    printf("\n");
}

void updateBoard(CacheMap& lut, std::vector<bool>& boards, int drawn)
{
    printf("Updating the board, with drawn no %i:\n", drawn);
    for(const int& idx : lut[drawn])
    {
        //printf("[%i], idx: %i\n", drawn, idx);
        boards[idx] = true;
    }
}

void isWin(std::vector<bool>& boards, int boardCount, std::unordered_set<int>& winners, int& winIdx)
{
    // Check rows
    winIdx = -1;
    for (int i = 0; i < boards.size(); i += 5)
    {
        bool win = true;
        for (int j = i; j < i + BOARD_SIZE; j++)
        {
            win = win && boards[j];
        }

        int temp = i / 25;
        if (win && winners.find(temp) == winners.end())
        {
            winners.insert(temp);
            winIdx = temp;
        }
    }

    // Check cols
    for (int i = 0; i < boardCount; i++)
    {
        for (int k = 0; k < BOARD_SIZE; k++)
        {
            bool win = true;
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                win = win && boards[(i * BOARD_SIZE * BOARD_SIZE) + (j * BOARD_SIZE) + k];
            }

            if (win && winners.find(i) == winners.end())
            {
                winners.insert(i);
                winIdx = i;
            }
        }
    }
}

int calcScore(CacheVals& lutVals, std::vector<bool>& boards, int winIdx, int lastDraw)
{
    int sum = 0;
    int offset = winIdx * BOARD_SIZE * BOARD_SIZE;
    for(int i = offset; i < offset + (BOARD_SIZE * BOARD_SIZE); i++)
    {
        if (i % 5 == 0) printf("\n");
        printf("%i ", lutVals[i]);

        if (!boards[i]) sum += lutVals[i];
    }
    printf("\n");

    printf("sum = %i\n", sum);
    return sum * lastDraw;
}

static std::string testPath = "./test_input.txt";
static std::string path = "./input.txt";
int main(int argc, char** argv)
{
    //auto file = std::ifstream(testPath);
    auto file = std::ifstream(path);

    // Parsing list of numbers drawn
    std::string rawDraws;
    file >> rawDraws;
    std::vector<int> drawList = initDrawList(rawDraws);
    printf("DrawList initiated with %ld elements\n", drawList.size());

    std::vector<bool> boards;
    CacheMap lutPos;
    CacheVals lutVals;
    int boardCount = 0;
    int count = 0;
    std::vector<std::string> tempBoard;
    std::string line;
    while(getline(file, line))
    {
        if ((line.size() == 1 || line.size() == 0) && tempBoard.size() > 0)
        {
            parseBoard(tempBoard, lutPos, lutVals, boards, count);

            boardCount++;
            tempBoard.clear();
        }
        else
        {
            tempBoard.push_back(line);
        }
    }
    parseBoard(tempBoard, lutPos, lutVals, boards, count);

    // Update the board as we pick numbers (save the last pick)
    // Check if we reached a win (save which board won)
    std::unordered_set<int> winners;
    int drawIdx = 0;
    int winIdx = -1;
    while(winners.size() != boardCount && drawIdx < drawList.size())
    {
        updateBoard(lutPos, boards, drawList[drawIdx++]);
        isWin(boards, boardCount, winners, winIdx);
    }
    //debugBoard(boards);
    printf("winners count = %ld\n", winners.size());
    printf("last board = %i\n", winIdx);

    // Sum unmarked numbers on the winning board
    // multiply sum with last picked number for win
    int result = calcScore(lutVals, boards, winIdx, drawList[--drawIdx]);
    printf("Result = %i\n", result);

    return 0;
}
