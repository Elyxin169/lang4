#ifndef funcshh
#define funcshh

#include <cstdio>
#include <string>
#include <cstring>

#include "macros.hh"

std::string readFile(const char *fileName)
{
    char *result;
    FILE *f = std::fopen(fileName, "r");
    fseek(f, 0L, SEEK_END);
    int size = std::ftell(f);
    result = (char *)std::malloc(size * sizeof(char));

    std::rewind(f);

    for (int i = 0; i < size; i++)
    {
        result[i] = std::fgetc(f);
    }

    std::fclose(f);

    return std::string(result);
}

namespace lang
{
    void skillIssue(const char *msg, int errorCode)
    {
        log("\n\n%s\n\n\n", msg);
        exit(errorCode);
    }
    // #define panic(msg, code) skillIssue(msg, code)
    //  so that i can use panic as well
}

#endif