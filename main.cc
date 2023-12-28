#include "main.hh"

// constant operators
std::unordered_map<str, int> operators;

void interpret(AST ast, std::unordered_map<str, MemoryObj *> *memory);

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
        tokens.push_back(Token::readToken(line));
    }
    return tokens;
}

// Node* iden = new Node(s1, s2)
#define newNode(iden, s1, s2) Node *iden = new Node(s1, s2)
#define nodeChild(n1, n2) n1->children.push_back(n2);
// if i put this in macros.hh id probably forget about it, so its here now

int getLineEndI(std::vector<Token> *tokens, int currI)
{
    int t = currI;
    for (; tokens->at(t).name != "line_end" && t < tokens->size(); t++)
        ;
    return t;
    // UNTESTED
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
            if (tokens[i].name == "number")
            {
                // declaration -> (int, 69)
                newNode(temp1, "declaration", "something idk");
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
        }
        else if (tokens[i].name == "identifier" && tokens[i + 1].name == "left_paren")
        {
            // blindly assuming it ends in a )
            // call -> (name, value)
            newNode(temp1, "func_call", "something idk");
            newNode(temp2, "identifier", tokens[i].value);
            newNode(temp3, "identifier", tokens[i + 2].value);
            nodeChild(temp1, temp2);
            nodeChild(temp1, temp3);
            nodeChild(currLine, temp1);
            i += 3;
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

Node *evalExpression(Node *root)
{
    // should get a node such that (expression) -> (420, +, 69)....
    // then return a node such that (number, 489)
    if (root->children.size() == 1)
    {
        // TODO: handle functions and shi
        const Node *child = root->children[0];
        if (child->name == "number")
        {
            newNode(t, "int", child->value);
            return t;
        }
    }
    else if (root->children.size() % 2 == 1)
    {
        // shunting yard algorithm
        /*

        Create two empty stacks: operator_stack and operand_stack.
        While there are still tokens to be read in:
            Get the next token.
            If the token is:
                A number: push it onto the operand_stack.
                A variable: get its value, and push onto the operand_stack.
                A left parenthesis: push it onto the operator_stack.
                A right parenthesis:
                    While the thing on top of the operator_stack is not a left parenthesis:
                        Pop the operator from the operator_stack.
                        Pop the value stack twice, getting two operands.
                        Apply the operator to the operands, in the correct order.
                        Push the result onto the operand_stack.
                    Pop the left parenthesis from the operator_stack, and discard it.
                An operator (call it thisOp):
                    While the operator_stack is not empty, and the top thing on the operator_stack has the same or greater precedence as thisOp:
                        Pop the operator from the operator_stack.
                        Pop the value stack twice, getting two operands.
                        Apply the operator to the operands, in the correct order.
                        Push the result onto the operand_stack.
                    Push thisOp onto the operator_stack.
        While the operator_stack is not empty:
            Pop the operator from the operator_stack.
            Pop the value stack twice, getting two operands.
            Apply the operator to the operands, in the correct order.
            Push the result onto the operand_stack.
        At this point, the operator_stack should be empty, and the operand_stack should have only one value in it, which is the final result.
        */
        std::stack<Node *> operator_stack;
        std::stack<Node *> operand_stack;

        for (Node *n : root->children)
        {
            if (n->name == "number")
                operand_stack.push(n);
            else if (n->name == "identifier")
                69; // implement later
            else if (n->name == "left_paren")
                operator_stack.push(n);
            else if (n->name == "right_paren")
            {
                while (!operator_stack.empty() && operator_stack.top()->name != "left_paren")
                {
                    str op = operator_stack.top()->value;
                    operator_stack.pop();
                    Node *rh = operand_stack.top();
                    operand_stack.pop();
                    Node *lh = operand_stack.top();
                    operand_stack.pop();

                    // blindly assuming both sides are ints
                    int result = std::stoi(lh->value);
                    if (op == "+")
                        result += std::stoi(rh->value);
                    else if (op == "-")
                        result -= std::stoi(rh->value);
                    else if (op == "*")
                        result *= std::stoi(rh->value);
                    else if (op == "/")
                        result /= std::stoi(rh->value);

                    newNode(t, "int", std::to_string(result));
                    operand_stack.push(t);
                }
                if (!operator_stack.empty() && operator_stack.top()->name == "left_paren")
                    operator_stack.pop();
                else
                    throw std::runtime_error("Mismatched parentheses");
            }
            else if (n->name == "mathop")
            {
                Node *thisOp = n;
                while (!operator_stack.empty() && operator_stack.top()->name == "mathop")
                {
                    str op = operator_stack.top()->value;
                    if (operators[op] >= operators[thisOp->value])
                    {
                        operator_stack.pop();
                        Node *rh = operand_stack.top();
                        operand_stack.pop();
                        Node *lh = operand_stack.top();
                        operand_stack.pop();

                        // blindly assuming both sides are ints
                        int result = std::stoi(lh->value);
                        if (op == "+")
                            result += std::stoi(rh->value);
                        else if (op == "-")
                            result -= std::stoi(rh->value);
                        else if (op == "*")
                            result *= std::stoi(rh->value);
                        else if (op == "/")
                            result /= std::stoi(rh->value);

                        newNode(t, "int", std::to_string(result));
                        operand_stack.push(t);
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
            str op = operator_stack.top()->value;
            operator_stack.pop();
            Node *rh = operand_stack.top();
            operand_stack.pop();
            Node *lh = operand_stack.top();
            operand_stack.pop();

            // blindly assuming both sides are ints
            int result = std::stoi(lh->value);
            if (op == "+")
                result += std::stoi(rh->value);
            else if (op == "-")
                result -= std::stoi(rh->value);
            else if (op == "*")
                result *= std::stoi(rh->value);
            else if (op == "/")
                result /= std::stoi(rh->value);

            newNode(t, "int", std::to_string(result));
            operand_stack.push(t);
        }
        // At this point, the operator_stack should be empty, and the operand_stack
        // should have only one value in it, which is the final result.
        newNode(t, "int", operand_stack.top()->value);
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
        }
        else if (first->name == "func_call")
        {
            Node *lh = first->children[0];
            Node *rh = first->children[1];
            if (lh->value == "println")
            {
                MemoryObj *obj = memory->at(rh->value);
                if (obj->type == "int")
                {
                    printf("%d\n", obj->getInt());
                }
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

    operators["+"] = 1;
    operators["-"] = 1;
    operators["*"] = 2;
    operators["/"] = 2;

    std::vector<Token> tokens = lex(argv[1]);
    AST ast = parse(tokens);

    std::unordered_map<str, MemoryObj *> memory;

    // ast.print();
    interpret(ast, &memory);
    // printf("asd, %s, %d", memory["asd"]->type.c_str(), memory["asd"]->getInt());
    return 0;
}