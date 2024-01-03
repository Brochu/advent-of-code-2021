#include "day.h"
#include "parsing.h"

namespace Solution {

#define DEMO 1
#if DEMO == 1 // ------------------------------------
#define FILE_PATH ".\\inputs\\day24_demo1.txt"
#else // ------------------------------------
#define FILE_PATH ".\\inputs\\day24.txt"
#endif // ------------------------------------

i32 part1() {
    i32 count = 0;
    return count;
}

i32 part2() {
    i32 count = 0;
    return count;
}

i32 run(std::string *part1_out, std::string *part2_out) {
    std::string in = INCLUDE_STR(FILE_PATH);
    printf("%s", in.c_str());

    *part1_out = std::to_string(part1());
    *part2_out = std::to_string(part2());

    return 0;
}

}
