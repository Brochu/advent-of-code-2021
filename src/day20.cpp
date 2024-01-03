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
#define FOUND(l, n) (std::find(std::begin(l), std::end(l), n) != std::end(l))

struct Pos { i32 x; i32 y; };
bool operator==(const Pos &left, const Pos &right) {
    return left.x == right.x && left.y == right.y;
}

struct Img {
    std::vector<Pos> pos;
    Pos min { INT32_MAX, INT32_MAX };
    Pos max { INT32_MIN, INT32_MIN };
};
void debug_img(Img &img) {
    const Pos test { 0, 0 };
    auto it = std::find(img.pos.begin(), img.pos.end(), test);
    for (i32 i = img.min.y - 5; i <= img.max.y + 5; i++) {
        for (i32 j = img.min.x - 5; j <= img.max.x + 5; j++) {
            const Pos p { j, i };
            if (FOUND(img.pos, p)) printf("#");
            else printf(".");
        }
        printf("\n");
    }
}
usize calc_offset(Img &img, Pos p) {
    //debug_img(img);
    Pos kern[9] {
        { p.x-1, p.y-1 }, { p.x+0, p.y-1 }, { p.x+1, p.y-1 },
        { p.x-1, p.y+0 }, { p.x+0, p.y+0 }, { p.x+1, p.y+0 },
        { p.x-1, p.y+1 }, { p.x+0, p.y+1 }, { p.x+1, p.y+1 },
    };
    usize offset = 0;
    for (Pos &k : kern) {
        offset <<= 1;
        if (FOUND(img.pos, k)) offset++;
    }
    return offset;
}
Img init_img(char *imgstr) {
    Img img;
    std::vector<char*> lines = Parse::split_char(imgstr, "\n");
    for (i32 i = 0; i < lines.size(); i++) {
        for (i32 j = 0; j < strlen(lines[i]); j++) {
            if (lines[i][j] == '#') {
                img.pos.push_back({ j, i });
                if (j < img.min.x) img.min.x = j;
                if (j > img.max.x) img.max.x = j;
                if (i < img.min.y) img.min.y = i;
                if (i > img.max.y) img.max.y = i;
            }
        }
    }
    return img;
}
void enhance_img(Img &img, std::span<char> algo) {
    std::vector<Pos> newimg;
    Pos min { INT32_MAX, INT32_MAX };
    Pos max { INT32_MIN, INT32_MIN };

    for (i32 i = img.min.y - 1; i <= img.max.y + 1; i++) {
        for (i32 j = img.min.x - 1; j <= img.max.x + 1; j++) {
            const Pos p { j, i };
            const usize idx = calc_offset(img, p);
            if (algo[idx] == '#') {
                newimg.push_back(p);
                if (j < min.x) min.x = j;
                if (j > max.x) max.x = j;
                if (i < min.y) min.y = i;
                if (i > max.y) max.y = i;
            }
        }
    }
    img.pos = newimg;
    img.min = min;
    img.max = max;
}

i32 part1(std::span<char> algo, Img &img) {
    //printf("[ALGO (%lld)]\n", algo.size());
    //for (char &c : algo) {
    //    printf("%c", c);
    //}
    //printf("\n\n");

    debug_img(img);
    enhance_img(img, algo);
    printf("-----------------\n");
    debug_img(img);
    enhance_img(img, algo);
    printf("-----------------\n");
    debug_img(img);

    return img.pos.size();
}

i32 part2() {
    i32 count = 0;
    return count;
}

i32 run(std::string *part1_out, std::string *part2_out) {
    std::string in = INCLUDE_STR(FILE_PATH);
    char *algostr = nullptr;
    char *imgstr = nullptr;
    Parse::split_once(in.data(), "\n\n", &algostr, &imgstr);

    std::span<char> algo { algostr, strlen(algostr) };
    Img img = init_img(imgstr);
    *part1_out = std::to_string(part1(algo, img));
    *part2_out = std::to_string(part2());

    return 0;
}

}
