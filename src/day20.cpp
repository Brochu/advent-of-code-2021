#include "day.h"
#include "parsing.h"

#include <span>

namespace Solution {

#define DEMO 1
#if DEMO == 1 // ------------------------------------
#define FILE_PATH ".\\inputs\\day20_demo1.txt"
#else // ------------------------------------
#define FILE_PATH ".\\inputs\\day20.txt"
#endif // ------------------------------------

struct Img {
    char *data;
    i32 from;
    i32 to;
};
char get_at(Img &img, i32 x, i32 y) {
    if (x < 0 || y < 0 || x >= img.w || y >= img.h) {
        return '.';
    } else {
        return img.data[x + (y * IMG_SIZE)];
    }
}

struct Pos {
    i32 x;
    i32 y;
};

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

std::string part1(char *algo, Img &img) {
    usize count = 0;

    Pos kern[9];
    find_kernel(-1, -1, kern);
    usize offset = 0;
    for (i32 i = 0; i < 9; i++) {
        offset <<= 1;
        if (get_at(img, kern[i].x, kern[i].y) == '#') offset++;
    }
    printf("offset = %lld\n", offset);

    return std::to_string(count);
}

std::string part2(char *algo, Img &img) {
    usize count = 0;
    return std::to_string(count);
}

i32 run(std::string *part1_out, std::string *part2_out) {
    std::string in = INCLUDE_STR(FILE_PATH);
    std::vector<char*> inputs = Parse::split_str(std::move(in), "\n\n");

    std::string imgStr;
    Parse::enum_char(inputs[1], "\n", [&imgStr](char *token){
        imgStr.append(token);
    });
    Img img { imgStr.data(), 0, 0, IMG_SIZE, IMG_SIZE };

    *part1_out = part1(inputs[0], img);
    *part2_out = part2(inputs[0], img);

    return 0;
}

}
