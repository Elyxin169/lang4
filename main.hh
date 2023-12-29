#ifndef MAINHH
#define MAINHH

#include <cstdio>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <stack>
// #include <charconv> // required for ctpg
// #include <iostream> // also ctpg
#include "macros.hh"
#include "classes.hh"
#include "funcs.hh"
// #include "ctpg.hpp"

enum Errors
{
    compilerArgsError = 1,
    fileReadError,
    syntaxError,
    typeError
};

#endif