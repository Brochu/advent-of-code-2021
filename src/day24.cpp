#include "day.h"
#include "parsing.h"

#include <span>

namespace Solution {

#define DEMO 1
#if DEMO == 1 // ------------------------------------
#define FILE_PATH ".\\inputs\\day24_demo3.txt"
#else // ------------------------------------
#define FILE_PATH ".\\inputs\\day24.txt"
#endif // ------------------------------------

struct Instruction {
    char* op;
    u8 a;
    i64 b;
    bool litteral = false;
};

struct Instance {
    i64 registers[4]; // W, X, Y, Z
    std::span<Instruction> prog;
    i64 pc;
    std::span<const char*> inputs;
    i64 ic;
};
void debug_registers(Instance &inst) {
    printf("[REGISTERS]\n");
    printf("  w=%lld\n", inst.registers[0]);
    printf("  x=%lld\n", inst.registers[1]);
    printf("  y=%lld\n", inst.registers[2]);
    printf("  z=%lld\n", inst.registers[3]);
}
bool is_done(Instance &inst) {
    return inst.pc >= inst.prog.size();
}
void step(Instance &inst) {
    Instruction &instr = inst.prog[inst.pc++];
    //printf("[%s] %i %lld (%c)\n", instr.op, instr.a, instr.b, instr.litteral ? 'Y' : 'N');
    i64 val = instr.litteral ? instr.b : inst.registers[instr.b];

    if (strcmp(instr.op, "inp") == 0) {
        i64 in = std::stoll(inst.inputs[inst.ic++]);
        inst.registers[instr.a] = in;
    }
    else if (strcmp(instr.op, "add") == 0) {
        inst.registers[instr.a] += val;
    }
    else if (strcmp(instr.op, "mul") == 0) {
        inst.registers[instr.a] *= val;
    }
    else if (strcmp(instr.op, "div") == 0) {
        inst.registers[instr.a] /= val;
    }
    else if (strcmp(instr.op, "mod") == 0) {
        inst.registers[instr.a] %= val;
    }
    else if (strcmp(instr.op, "eql") == 0) {
        inst.registers[instr.a] = (inst.registers[instr.a] == val) ? 1 : 0;
    }
}

i32 part1(std::span<Instruction> prog) {
    std::vector<const char*> inputs { "1", "3", "5", "7", "9", "2", "4", "6", "8", "9", "9", "9", "9", "9" };
    printf("[INPUTS] count=%lld: [", inputs.size());
    for (const char *in : inputs) {
        printf("'%s' ", in);
    }
    printf("]\n");

    Instance inst { {0, 0, 0, 0}, prog, 0, inputs, 0, };
    while(!is_done(inst)) {
        step(inst);
    }

    debug_registers(inst);
    return 0;
}

i32 part2() {
    i32 count = 0;
    return count;
}

i32 run(std::string *part1_out, std::string *part2_out) {
    std::string in = INCLUDE_STR(FILE_PATH);

    std::vector<Instruction> prog;
    Parse::enum_str(std::move(in), "\n", [&prog](char *token){
        char *opcode = nullptr;
        char *args = nullptr;
        Parse::split_once(token, " ", &opcode, &args);
        char *arg0 = nullptr;
        char *arg1 = nullptr;
        Parse::split_once(args, " ", &arg0, &arg1);

        u8 idx = 0;
        if (arg0[0] == 'w') idx = 0;
        else if (arg0[0] == 'x') idx = 1;
        else if (arg0[0] == 'y') idx = 2;
        else if (arg0[0] == 'z') idx = 3;

        if (strcmp(opcode, "inp") == 0) {
            prog.push_back({ opcode, idx, -1, false });
        }
        else {
            i64 val = 0;
            bool lit = false;
            if ((arg1[0] >= '0' && arg1[0] <= '9') || arg1[0] == '-') {
                val = std::stoi(arg1);
                lit = true;
            }
            else {
                if (arg1[0] == 'w') val = 0;
                else if (arg1[0] == 'x') val = 1;
                else if (arg1[0] == 'y') val = 2;
                else if (arg1[0] == 'z') val = 3;
            }
            prog.push_back({ opcode, idx, val, lit });
        }
    });

    *part1_out = std::to_string(part1(prog));
    *part2_out = std::to_string(part2());

    return 0;
}

}
