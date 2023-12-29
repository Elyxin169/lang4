#include "main.hh"

// constant operators
std::unordered_map<str, int> operators;
std::unordered_map<str, MemoryObj *> memory;

void interpret(AST ast, std::unordered_map<str, MemoryObj *> *memory);

// Node* iden = new Node(s1, s2)
#define newNode(iden, s1, s2) Node *iden = new Node(s1, s2)
#define nodeChild(n1, n2) n1->children.push_back(n2);
// if i put this in macros.hh id probably forget about it, so its here now

std::vector<Token> lex(char *fileName)
{
    // generate tokens into file
#ifdef linux
    std::system(("./lexTest " + str(fileName)).c_str());
#endif
#ifdef _WIN32
    std::system((".\\lexTest " + str(fileName)).c_str());
#endif

    // get tokens from file
    std::vector<Token> tokens;
    auto lexed = std::stringstream(readFile("lexOutTemp"));
    str line = "";
    for (int lineCount = 0; std::getline(lexed, line, '\n'); lineCount++)
    {
        Token t = Token::readToken(line);
        // log("%s, %s\n", t.name.c_str(), t.value.c_str());
        tokens.push_back(t);
    }
    return tokens;
}

int getLineEndI(std::vector<Token> *tokens, int currI)
{
    int t = currI;
    for (; tokens->at(t).name != "line_end" && t < tokens->size(); t++)
        ;
    return t;
}

#define lineEnd(x) int x = getLineEndI(&tokens, i)

AST parse(std::vector<Token> tokens)
{
    Node *root = new Node("ROOT", "69");
    // testing
    // Node *test1 = new Node("test1", "1337");
    // Node *test2 = new Node("test2", "420");
    // Node *test3 = new Node("test3", "420");
    // root->children.push_back(test1);
    // root->children.push_back(test2);
    // test2->children.push_back(test3);
    newNode(line1, "line", "1");
    nodeChild(root, line1);
    Node *currLine;
    currLine = line1;

    int lineCount = 1;
    for (int i = 0; i < tokens.size(); i++)
    {
        Token v = tokens[i];
        // printf("%s %s\n", v.name.c_str(), v.value.c_str());
        // handle new lines
        if (v.name == "line_end")
        {
            if (i == tokens.size() - 1)
                break;
            lineCount++;
            newNode(newLine, "line", std::to_string(lineCount));
            nodeChild(root, newLine);
            currLine = newLine;
            continue;
        }

        // state machine kinda kinda not

        if (tokens[i].name == "type_name" && tokens[i + 1].name == "identifier" && tokens[i + 2].name == "assign")
        {
            i += 3;
            // declaration -> (int, 69)
            newNode(temp1, "declaration", "hi mom");
            newNode(temp2, "identifier", tokens[i - 2].value);
            newNode(temp3, "expression", "");
            // newNode(temp3, "int", tokens[i + 3].value);
            nodeChild(temp1, temp2);
            nodeChild(temp1, temp3);
            nodeChild(currLine, temp1);

            lineEnd(uwu);
            for (; i < uwu; i++)
            {
                newNode(t, tokens[i].name, tokens[i].value);
                nodeChild(temp3, t);
            }
            i--;
        }
        else if (tokens[i].name == "identifier" && tokens[i + 1].name == "left_paren")
        {
            // blindly assuming it ends in a )
            // call -> (name, value)
            newNode(temp1, "func_call", "hi mom");
            newNode(temp2, "identifier", tokens[i].value);
            newNode(temp3, "expression", "");
            nodeChild(temp1, temp2);
            nodeChild(temp1, temp3);
            nodeChild(currLine, temp1);

            i += 2;
            lineEnd(uwu);
            for (; i < uwu - 1; i++) // -1 accounts for closing brace
            {
                newNode(t, tokens[i].name, tokens[i].value);
                nodeChild(temp3, t);
            };
        }
        else if (tokens[i].name == "identifier" && tokens[i + 1].name == "assign")
        {
            // (reassignment) -> (iden, expr)
            newNode(temp1, "reassignment", "hi mom");
            newNode(temp2, "identifier", tokens[i].value);
            newNode(temp3, "expression", "");
            nodeChild(temp1, temp2);
            nodeChild(temp1, temp3);
            nodeChild(currLine, temp1);

            i += 2;
            lineEnd(uwu);
            for (; i < uwu; i++)
            {
                newNode(t, tokens[i].name, tokens[i].value);
                nodeChild(temp3, t);
            };
            i--;
        }
        else
        {
            currLine->value = "ERR_LINE";
            fprintf(stdout, "syntax error at line %d\n", lineCount);
            // loop till end of line
            for (; tokens[i].name != "line_end" && i < tokens.size(); i++)
            {
                // printf("%s\n", tokens[i].name.c_str());
                Node *t = new Node(tokens[i]);
                nodeChild(currLine, t);
            }
            i--;
        }
    }

    return AST(root);
}

Node *meth(Node *op, Node *lh, Node *rh)
{
    // log("%s, %s\n", op->name.c_str(), op->value.c_str());
    str o = op->value;
    if ((lh->name == "float" || rh->name == "float") && op->name == "mathop") // if any of the 2 sides is a float, both will now be floats
    {
        float result = std::stof(lh->value);
        switch (o[0])
        {
        case '+':
            result += std::stof(rh->value);
            break;
        case '-':
            result -= std::stof(rh->value);
            break;
        case '*':
            result *= std::stof(rh->value);
            break;
        case '/':
            result /= std::stof(rh->value);
            break;
        }
        newNode(t, "float", std::to_string(result));
        return t;
    }
    else if (op->name == "cmp")
    {
        // log("%s, %s %s %s, %s\n", lh->name.c_str(), lh->value.c_str(), op->value.c_str(), rh->name.c_str(), rh->value.c_str());
        bool result = false;
        if (o == ">")
            result = std::stof(lh->value) > std::stof(rh->value);
        else if (o == "<")
            result = std::stof(lh->value) < std::stof(rh->value);
        // only gonna work on ints
        else if (o == "!=")
            result = std::stoi(lh->value) != std::stoi(rh->value);
        else if (o == "==")
            result = std::stoi(lh->value) == std::stoi(rh->value);
        newNode(t, "bool", result ? "true" : "false");
        return t;
    }
    else
    {
        int result = std::stoi(lh->value);
        switch (o[0])
        {
        case '+':
            result += std::stoi(rh->value);
            break;
        case '-':
            result -= std::stoi(rh->value);
            break;
        case '*':
            result *= std::stoi(rh->value);
            break;
        case '/':
            result /= std::stoi(rh->value);
            break;
        }
        newNode(t, "int", std::to_string(result));
        return t;
    }
}

Node *evalExpression(Node *root)
{
    // should get a node such that (expression) -> (420, +, 69)....
    // then return a node such that (number, 489)
    if (root->children.size() == 1)
    {
        // TODO: handle functions and shi
        Node *child = root->children[0];
        if (child->name == "int" || child->name == "float" || child->name == "bool")
        {
            newNode(t, child->name, child->value);
            // log("%s, %s\n", child->name.c_str(), child->value.c_str());
            return t;
        }
        else if (child->name == "identifier")
        {
            MemoryObj *obj = memory[child->value];
            if (obj->type == "int")
            {
                newNode(t, "int", std::to_string(obj->getInt()));
                return t;
            }
            else if (obj->type == "float")
            {
                newNode(t, "float", std::to_string(obj->getFloat()));
                return t;
            }
            else if (obj->type == "bool")
            {
                newNode(t, "bool", obj->getBool() ? "true" : "false");
                return t;
            }
        }
    }
    else if (root->children.size() % 2 == 1)
    {
        // shunting yard algorithm

        std::stack<Node *> operator_stack;
        std::stack<Node *> operand_stack;

        for (Node *n : root->children)
        {
            if (n->name == "int" || n->name == "float" || n->name == "bool")
                operand_stack.push(n);
            else if (n->name == "identifier")
            {
                MemoryObj *obj = memory[n->value];
                if (obj->type == "int")
                {
                    newNode(t, "int", std::to_string(obj->getInt()));
                    operand_stack.push(t);
                }
                else if (obj->type == "float")
                {
                    newNode(t, "float", std::to_string(obj->getFloat()));
                    operand_stack.push(t);
                }
            }
            else if (n->name == "left_paren")
                operator_stack.push(n);
            else if (n->name == "right_paren")
            {
                while (!operator_stack.empty() && operator_stack.top()->name != "left_paren")
                {
                    Node *op = operator_stack.top();
                    operator_stack.pop();
                    Node *rh = operand_stack.top();
                    operand_stack.pop();
                    Node *lh = operand_stack.top();
                    operand_stack.pop();

                    operand_stack.push(meth(op, lh, rh));
                }
                if (!operator_stack.empty() && operator_stack.top()->name == "left_paren")
                    operator_stack.pop();
                else
                    throw std::runtime_error("Mismatched parentheses");
            }
            else if (n->name == "mathop" || n->name == "cmp")
            {
                Node *thisOp = n;
                while (!operator_stack.empty() && operator_stack.top()->name == "mathop")
                {
                    Node *op = operator_stack.top();
                    if (operators[op->value] >= operators[thisOp->value])
                    {
                        operator_stack.pop();
                        Node *rh = operand_stack.top();
                        operand_stack.pop();
                        Node *lh = operand_stack.top();
                        operand_stack.pop();

                        operand_stack.push(meth(op, lh, rh));
                    }
                    else
                    {
                        break;
                    }
                }
                operator_stack.push(thisOp);
            }
        }

        while (operator_stack.size() != 0)
        {
            Node *op = operator_stack.top();
            operator_stack.pop();
            Node *rh = operand_stack.top();
            operand_stack.pop();
            Node *lh = operand_stack.top();
            operand_stack.pop();

            operand_stack.push(meth(op, lh, rh));
        }
        // At this point, the operator_stack should be empty, and the operand_stack
        // should have only one value in it, which is the final result.
        newNode(t, operand_stack.top()->name, operand_stack.top()->value);
        return t;
    }
    else
    {
        AST shi(root);
        shi.print();
        lang::skillIssue("failed to evaluate the above expression", Errors::syntaxError);
        return nullptr; // so that the compiler wont complain
    }
    return nullptr; // should never reach
}

void interpret(AST ast, std::unordered_map<str, MemoryObj *> *memory)
{
    // ast.print();
    for (Node *node : ast.root->children)
    {
        Node *first = node->children[0];
        if (first->name == "declaration")
        {
            Node *lh = first->children[0];
            Node *rh = first->children[1]; // should be an expression
            rh = evalExpression(rh);
            if (rh->name == "int")
            {
                int *num = (int *)malloc(sizeof(int));
                *num = std::atoi(rh->value.c_str());
                MemoryObj *newData = new MemoryObj("int", num);
                memory->insert({lh->value, newData});
            }
            else if (rh->name == "float")
            {
                float *num = (float *)malloc(sizeof(float));
                *num = std::atof(rh->value.c_str());
                MemoryObj *newData = new MemoryObj("float", num);
                memory->insert({lh->value, newData});
            }
            else if (rh->name == "bool")
            {
                bool *num = (bool *)malloc(sizeof(bool));
                *num = true ? rh->value == "true" : false;
                MemoryObj *newData = new MemoryObj("bool", num);
                memory->insert({lh->value, newData});
            }
            else
            {
                lang::skillIssue(("unknown type: " + rh->name).c_str(), Errors::typeError);
            }
        }
        else if (first->name == "func_call")
        {
            Node *lh = first->children[0];
            Node *rh = first->children[1];
            rh = evalExpression(rh);
            // if (lh->value == "println")
            if (lh->value[0] == 'p' && lh->value[1] == 'r' && lh->value[2] == 'i' && lh->value[3] == 'n' && lh->value[4] == 't')
            {
                if (rh->name == "int")
                {
                    printf("%d", std::stoi(rh->value));
                }
                else if (rh->name == "float")
                {
                    printf("%.4f", std::stof(rh->value));
                }
                else if (rh->name == "bool")
                {
                    printf("%s", rh->value.c_str());
                }
                else
                {
                    lang::skillIssue(("cant print type: " + rh->name).c_str(), 69);
                }
                if (lh->value.length() > 5)
                    if (lh->value[5] == 'l' && lh->value[6] == 'n')
                        printf("\n");
            }
        }
        else if (first->name == "reassignment")
        {
            Node *lh = first->children[0];
            Node *rh = first->children[1]; // should be an expression
            rh = evalExpression(rh);
            if (rh->name == "int")
            {
                int *num = (int *)memory->at(lh->value)->data;
                *num = std::stoi(rh->value);
                memory->at(lh->value)->data = num;
            }
            else if (rh->name == "float")
            {
                float *num = (float *)memory->at(lh->value)->data;
                *num = std::stof(rh->value);
                memory->at(lh->value)->data = num;
            }
            else if (rh->name == "bool")
            {
                bool *num = (bool *)memory->at(lh->value)->data;
                *num = true ? rh->value == "true" : false;
                memory->at(lh->value)->data = num;
            }
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        lang::skillIssue("must have precisely 1 argument, that is the input file.", Errors::compilerArgsError);
    }
    // log("%d\n", argc);

    operators["+"] = 2;
    operators["-"] = 2;
    operators["*"] = 3;
    operators["/"] = 3;
    operators[">"] = 1;
    operators["<"] = 1;
    operators["=="] = 1;
    operators["!="] = 1;

    std::vector<Token> tokens = lex(argv[1]);
    AST ast = parse(tokens);

    ast.print();
    interpret(ast, &memory);
    // printf("asd, %s, %d", memory["asd"]->type.c_str(), memory["asd"]->getInt());
    return 0;
}