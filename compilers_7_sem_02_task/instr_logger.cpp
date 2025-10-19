#include <cstdio>
#include <cstring>

extern "C" void recordInstructionOpcode(const char *instructionOpcode) 
{
    printf("%s\n", instructionOpcode);
    fflush(stdout);
}