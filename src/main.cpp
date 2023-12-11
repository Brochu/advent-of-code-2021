#include <stdio.h>
#include "day.h"

int main(int argc, char **argv) {
    std::string p1 = "";
    std::string p2 = "";
    int code = Solution::run(&p1, &p2);

    printf("[MAIN] Solutions for day %i (code = %i)\n", DAY, code);
    printf("[MAIN] Here the part_1 solution = %s\n", p1.c_str());
    printf("[MAIN] Here the part_2 solution = %s\n", p2.c_str());

    return 0;
}
