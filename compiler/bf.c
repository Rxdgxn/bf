#include <stdio.h>

#define CELLS        2048
#define PROGRAM_SIZE 4096
#define STACK_SIZE   512

#define END       0
#define INC_VAL   1
#define DEC_VAL   2
#define INC_PTR   3
#define DEC_PTR   4
#define OUT       5
#define IN        6
#define FWD       7
#define BCK       8

struct instruction {
    unsigned short op;
    unsigned int pos;
};
unsigned int P = 0;
#define PUSH(X) (STACK[P++] = X)
#define POP()   (STACK[--P])

struct instruction PROGRAM[PROGRAM_SIZE];
unsigned short STACK[STACK_SIZE];
unsigned short DATA[CELLS];

void compile(FILE *fp) {
    unsigned int p = 0, jmp_p, c;
    while ((c = getc(fp)) != EOF && p < PROGRAM_SIZE) {
        switch (c) {
            case '>': PROGRAM[p].op = INC_PTR; break;
            case '<': PROGRAM[p].op = DEC_PTR; break;
            case '+': PROGRAM[p].op = INC_VAL; break;
            case '-': PROGRAM[p].op = DEC_VAL; break;
            case '.': PROGRAM[p].op = OUT; break;
            case ',': PROGRAM[p].op = IN; break;
            case '[':
                PROGRAM[p].op = FWD;
                PUSH(p);
                break;
            case ']':
                jmp_p = POP();
                PROGRAM[p].op = BCK;
                PROGRAM[p].pos = jmp_p;
                PROGRAM[jmp_p].pos = p;
                break;
            default: p--; break;
        }
        p++;
    }
    PROGRAM[p].op = END;
}

void execute() {
    unsigned int ptr = CELLS, p = 0;
    while (--ptr) DATA[ptr] = 0;
    while (PROGRAM[p].op != END && ptr < CELLS) {
        switch (PROGRAM[p].op) {
            case INC_VAL: DATA[ptr]++; break;
            case DEC_VAL: DATA[ptr]--; break;
            case INC_PTR: ptr++; break;
            case DEC_PTR: ptr--; break;
            case OUT: putchar(DATA[ptr]); break;
            case IN: DATA[ptr] = (unsigned int) getchar(); break;
            case FWD: if (!DATA[ptr]) p = PROGRAM[p].pos; break;
            case BCK: if (DATA[ptr]) p = PROGRAM[p].pos; break;
        }
        p++;
    }
}

int main(int argc, const char * argv[]) {
    FILE* fp = fopen(argv[1], "r");
    compile(fp);
    execute();
}