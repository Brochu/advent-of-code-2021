#include "day.h"
#include "parsing.h"

#include <span>

namespace Solution {

#define DEMO 1
#if DEMO == 1 // ------------------------------------
#define FILE_PATH ".\\inputs\\day20_demo1.txt"
#define IMG_SIZE 5
#else // ------------------------------------
#define FILE_PATH ".\\inputs\\day20.txt"
#define IMG_SIZE 100
#endif // ------------------------------------

struct Pos {
    i32 x;
    i32 y;
};
char find_at(std::span<Pos> array, i32 x, i32 y) {
    for (Pos &elem : array) {
        if (elem.x == x && elem.y == y) {
            return '#';
        }
    }
    return '.';
}

void debug_space(std::span<Pos> space) {
    for (i32 i = -5; i <= 9; i++) {
        for (i32 j = -5; j <= 9; j++) {
            printf("%c", find_at(space, j, i));
        }
        printf("\n");
    }
}

void find_kernel(i32 x, i32 y, Pos out[9]) {
    out[0] = { x - 1, y - 1 };
    out[1] = { x - 0, y - 1 };
    out[2] = { x + 1, y - 1 };
    out[3] = { x - 1, y - 0 };
    out[4] = { x - 0, y - 0 };
    out[5] = { x + 1, y - 0 };
    out[6] = { x - 1, y + 1 };
    out[7] = { x - 0, y + 1 };
    out[8] = { x + 1, y + 1 };
}

std::string part1(char *algo, std::span<Pos> space) {
    usize count = 0;
    debug_space(space);

    Pos k[9];
    find_kernel(2, 2, k);
    usize offset = 0;
    for (i32 i = 0; i < 9; i++) {
        offset <<= 1;
        if (find_at(space, k[i].x, k[i].y) == '#') {
            offset++;
        }
    }
    printf("OFFSET = %lld\n", offset);
    return std::to_string(count);
}

std::string part2(char *algo, std::span<Pos> space) {
    usize count = 0;
    return std::to_string(count);
}

i32 run(std::string *part1_out, std::string *part2_out) {
    std::string in = INCLUDE_STR(FILE_PATH);
    std::vector<char*> inputs = Parse::split_str(std::move(in), "\n\n");

    std::vector<char*> imgData = Parse::split_char(inputs[1], "\n");
    std::vector<Pos> space;
    for (i32 i = 0; i < imgData.size(); i++) {
        for (i32 j = 0; j < strlen(imgData[i]); j++) {
            if (imgData[i][j] == '#') { space.push_back({ j, i }); }
        }
    }

    *part1_out = part1(inputs[0], space);
    *part2_out = part2(inputs[0], space);

    return 0;
}

}
