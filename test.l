%{
    #include <iostream>
    #include <fstream>
    #include <vector>
    #include "classes.hh"
    using namespace std;
    vector<Token> tokens;
    void a() {

    }
%}

/* %option noyywrap */
%option c++

/* comment [//].*(\S.*)?$ */

string  \"[^\n"]+\"

ws      [ \t]+

alpha   [A-Za-z]
dig     [0-9]
name    ({alpha}|{dig}|\$)({alpha}|{dig}|[_.\-/$])*
num1    [-+]?{dig}+\.?([eE][-+]?{dig}+)?
num2    [-+]?{dig}*\.{dig}+([eE][-+]?{dig}+)?
number  {num1}|{num2}
int     [-]?{dig}+
float   [-]?{dig}+\.{dig}+
left_paren [\(]
right_paren [\)]
comma [,]
assign [=]
type_name (int|str|float|bool)
bool (true|false)
mathop [\+\-\/\*]
comparision (<|>|==|!=)
%%

{ws}    /* skip blanks and tabs */
{int} {
    Token t("int", YYText());
    tokens.push_back(t);
}
{float} {
    Token t("float", YYText());
    tokens.push_back(t);
}
{mathop} {
    Token t("mathop", YYText());
    tokens.push_back(t);
}
{comparision} {
    Token t("cmp", YYText());
    tokens.push_back(t);
}
{bool}  {
    Token t("bool", YYText());
    tokens.push_back(t);
};
{type_name}  {
    Token t("type_name", YYText());
    tokens.push_back(t);
};
\n {}
;        {
    Token t("line_end", "uwu");
    tokens.push_back(t);
};
{name}    {
    Token t("identifier", YYText());
    tokens.push_back(t);
};
{string}  {
    Token t("string", YYText());
    tokens.push_back(t);
};
{left_paren}  {
    Token t("left_paren", YYText());
    tokens.push_back(t);
};
{right_paren}  {
    Token t("right_paren", YYText());
    tokens.push_back(t);
};
{comma}   {
    Token t("comma", YYText());
    tokens.push_back(t);
};
{assign}   {
    Token t("assign", YYText());
    tokens.push_back(t);
};
%%

int yyFlexLexer::yywrap()
{
    return true;
}

int main(int argc, char** argv) {
    ifstream input_file(argv[1]);
    FILE* lexOut = fopen("lexOutTemp", "w");
    fprintf(lexOut, "");
    fclose(lexOut);
    lexOut = fopen("lexOutTemp", "a");

    yyFlexLexer lex(&input_file);
    while (lex.yylex())
    {
    }

    // tokens.push_back(Token("line_end", ""));

    for (Token t : tokens) {
        fprintf(lexOut, "%s\n", t.save().c_str());
    }
    
    fclose(lexOut);
    input_file.close();
    return 0;
}

// flex test.l && g++ ./lex.yy.cc -O3 -o lexTest

// screw this below!

// reflex --flex --noyywrap --yy test.l
// g++ -I/home/joemamapc/Documents/codering/c-c++/RE-flex-3.5.1/include /home/joemamapc/Documents/codering/c-c++/RE-flex-3.5.1/lib/libreflex.so lex.yy.cpp