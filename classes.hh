#ifndef classeshh
#define classeshh
#include "string"
#include "macros.hh"
// #include "format"

class Token
{
public:
    std::string name, value;
    Token(std::string name, std::string value)
        : name(name), value(value)
    {
    }
    ~Token()
    {
    }
    std::string save()
    {
        return "T:" + this->name + ";V:" + this->value + ">";
    }
    static Token readToken(std::string input)
    {
        int pos = input.find(';');
        std::string name = input.substr(0, pos);
        std::string value = input.substr(pos, input.length() - name.length());
        // printf("%s, %s\n", name.substr(2, name.length()).c_str(), value.substr(3, value.length() - 4).c_str());

        return Token(name.substr(2, name.length()).c_str(), value.substr(3, value.length() - 4).c_str());
    }
};

class Node
{
public:
    std::string name, value;
    std::vector<Node *> children;
    Node(std::string name, std::string value)
        : name(name), value(value)
    {
    }
    Node(Token t)
        : name(t.name), value(t.value)
    {
    }
    ~Node()
    {
    }
    // std::string save()
    // {
    //     return "T:" + this->name + ";V:" + this->value + ">";
    // }
    // static Token readToken(std::string input)
    // {
    // }
};

class AST
{
public:
    Node *root;
    AST(Node *root)
        : root(root)
    {
    }
    ~AST()
    {
    }
    static void visualize(Node *node, int indent = 0)
    {
        if (node == nullptr)
            return;

        printf("%*s%s", indent, "", node->name.c_str());
        if (node->value != "")
            printf(", %s", node->value.c_str());
        printf("\n");

        for (auto child : node->children)
        {
            visualize(child, indent + 4);
        }
    }
    void print()
    {
        AST::visualize(this->root);
    }
};

class MemoryObj
{
public:
    // str name;
    str type;
    void *data;
    MemoryObj(str type, void *data)
        : type(type), data(data)
    {
    }
    ~MemoryObj()
    {
    }
    int getInt()
    {
        return *((int *)this->data); // pointer magix
    }
    float getFloat()
    {
        return *((float *)this->data);
    }
    bool getBool()
    {
        return *((bool *)this->data);
    }
};

#endif