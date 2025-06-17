#include <iostream>
#include <fstream>
#include <vector>
#include <cctype>
#include <stdlib.h>
#include <chrono>
#include <iomanip>
#include <string>
#include <string_view>
#include <memory>
#include <variant>

static int numOfHeapAllocations = 0;

void* operator new(size_t size)
{
    numOfHeapAllocations++;
    return malloc(size);
}

enum class TokenType {
    NULLTOKEN,
    COMMAND,
    NUMBER,
    VARIABLE,
    OPERATOR,
    OPEN_BRACE,
    CLOSE_BRACE,
    END
};

struct Token {
    TokenType type;
    std::string_view value;  // Avoids unnecesary copying
    Token(TokenType t, std::string_view v) : type(t), value(v) {}
};

/*
Test:
std::string latexMath = R"(\frac{a^{2} + b^{2} - 2ab\cos{\theta}}{2\sqrt{r^2 + s^2 - 2rs\cos{\phi}}} + \sum_{i=1}^{n} \left(\frac{x_i - \mu}{\sigma}\right)^2 + \int_{0}^{\infty} e^{-x^2}dx)";

main() was tasked to tokenize latexMath exactly 10000 times

Average runtime when using std::push_back:
101µs

Average runtime when using std::emplace_back:
98µs (3µs save)
*/

class LaTeXMathTokenizer {
public:
    LaTeXMathTokenizer(const std::string& input) : input(input), pos(0) {
        tokens.reserve(input.size() / 5);  // Reserve memory before use to save allocations (Based on an arbitrary estimation)
    }

    std::vector<Token> tokenize() {
        while (pos < input.size()) {
            char current = input[pos];

            if (current == '\\') {
                tokens.emplace_back(parseCommand());
            } else if (current == '{') {
                tokens.emplace_back(TokenType::OPEN_BRACE, "{");
                pos++;
            } else if (current == '}') {
                tokens.emplace_back(TokenType::OPEN_BRACE, "}");
                pos++;
            } else if (std::isdigit(current) || current == '.') {
                tokens.emplace_back(parseNumber());
            } else if (std::isalpha(current)) {
                tokens.emplace_back(parseVariable());
            } else if (current == '+' || current == '-' || current == '*' || current == '/' || current == '^') {
                tokens.emplace_back(TokenType::OPERATOR, std::string_view(&current, 1));
                pos++;
            } else {
                pos++;
            }
        }

        tokens.emplace_back(TokenType::END, "");
        return tokens;
    }

private:
    std::string input;
    size_t pos;
    std::vector<Token> tokens;

    Token parseCommand() {
        std::string_view command;
        size_t startPos = pos;
        pos++;
        while (pos < input.size() && std::isalnum(input[pos])) {
            pos++;
        }

        command = std::string_view(input.data() + startPos, pos - startPos);
        return {TokenType::COMMAND, command};
    }

    Token parseNumber() {
        size_t startPos = pos;
        while (pos < input.size() && (std::isdigit(input[pos]) || input[pos] == '.')) {
            pos++;
        }
        std::string_view number(input.data() + startPos, pos - startPos);
        return {TokenType::NUMBER, number};
    }

    Token parseVariable() {
        size_t startPos = pos;
        while (pos < input.size() && std::isalnum(input[pos])) {
            pos++;
        }
        std::string_view variable(input.data() + startPos, pos - startPos);
        return {TokenType::VARIABLE, variable};
    }
};

enum class TokenBehavior {
    NUMBER,
    VARIABLE,
    BINARY_OP,
    PREFIX_UNARY_OP,
    POSTFIX_UNARY_OP,
    FUNCTION
};

std::string binary_op[] = {
    "<=>",
    "=>",
    "=",">=","<=",">","<",
    "^",
    "*",
    "+","-"
};

std::string prefix_op[] = {
    "#" // Primorial
};

std::string postfix_op[] = {
    "!" // Factorial
};

struct ASTNode {
    
    std::vector<std::unique_ptr<ASTNode>> children;

    ASTNode(std::string_view v) {}
};

class Parser
{
private:
    const std::vector<std::variant<Token,std::unique_ptr<ASTNode>>>& tokens;

public:
    Parser(const std::vector<std::variant<Token,std::unique_ptr<ASTNode>>>& tokens) : tokens(tokens)
    {
        if (tokens.empty())
        {
            std::cout << "[-] Error: No tokens to parse\n";
        }
    }

    std::variant<Token, std::unique_ptr<ASTNode>> ParseExpression(size_t start);
};

/*
    This function is meant to be used to parse whole expressions or grouped expressions (meaning
    expressions grouped by parenthesis, brackets or keys) for example, in the expression:

    2*(3+5)
    ^-----^ ParseExpression is called here
    2*(3+5)
      ^---^ Parse Expression is also called here

    The starting point must be used to call parse expression, but the ending point will be inferred
    by the function.
*/
std::variant<Token, std::unique_ptr<ASTNode>> Parser::ParseExpression(size_t start)
{
    const auto& tokens = this->tokens;
    // First refers to the group starting character "(, [, {, ..."
    const auto& first = tokens[start];
    // Uses first to find the group ending character "), ], }, ..."
    // If first isn't a group starting character, the ending point is the final token (Parse whole expression)
    
}

/*
void printAST(const std::unique_ptr<ASTNode>& node, int indent = 0) {
    if (!node) return;
    std::cout << std::string(indent, ' ') << ": " << node->value << "\n";
    for (const auto& child : node->children)
        printAST(child, indent + 2);
}
*/

int main() {
    std::string latexMath = R"(a+b*c)";

    LaTeXMathTokenizer tokenizer(latexMath);
    std::chrono::duration<double> total{0};

    auto start = std::chrono::high_resolution_clock::now();
    
    auto end = std::chrono::high_resolution_clock::now();
    total = end-start;
    //printAST(ast);

    std::cout << "ASTNode is built using " << sizeof(ASTNode) << " bytes" << "\n";

    std::cout << "[+] Runtime: " << std::fixed << std::setprecision(6) << (total.count()*1000) << "ms" << std::endl;

    std::cout << "[+] Heap allocations: " << numOfHeapAllocations << std::endl;

    return 0;
}
