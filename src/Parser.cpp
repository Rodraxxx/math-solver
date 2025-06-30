#include <iostream>
#include <variant>
#include <memory>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cstdlib>
#include <stack>
#include <queue>
#include <string>

#include "../include/AtomicTypes.h"
#include "../include/Functions.h"
#include "../include/Constants.h"
#include "../include/AuxiliaryTypes.h"

static int numOfHeapAllocations = 0;

void* operator new(size_t size)
{
    numOfHeapAllocations++;
    return malloc(size);
}

std::unordered_map<char,AuxiliaryTypes> groupers =
{
  {'{',AuxiliaryTypes::LEFTKEY},
  {'}',AuxiliaryTypes::RIGHTKEY},
  {'[',AuxiliaryTypes::LEFTBRACKET},
  {']',AuxiliaryTypes::RIGHTBRACKET},
  {'(',AuxiliaryTypes::LEFTPARENTHESIS},
  {')',AuxiliaryTypes::RIGHTPARENTHESIS}
};

std::unordered_map<char,Functions> operators =
{
  {'+',Functions::ADDITION},
  {'-',Functions::SUBTRACTION},
  {'*',Functions::MULTIPLICATION},
  {'/',Functions::DIVISION},
  {'^',Functions::EXPONENTIATION},
  {'!',Functions::FACTORIAL},
  {'#',Functions::PRIMORIAL},
  {'=',Functions::EQUALS}
};

std::unordered_map<std::string_view,Functions> commands =
{
  {"sqrt",Functions::SQRT},

  {"choose",Functions::CHOOSE},

  {"gt",Functions::GT},
  {"lt",Functions::LT},
  {"ge",Functions::GE},
  {"le",Functions::LE}
};

std::unordered_map<std::string_view,Constants> constants =
{
  {"pi",Constants::PI},
  {"e",Constants::e},
  {"i",Constants::i},
  {"phi",Constants::PHI}
};

constexpr size_t getPriority(Functions f)
{
  switch(f) {
    case Functions::ADDITION:
    case Functions::SUBTRACTION:
      return 20;
    case Functions::MULTIPLICATION:
    case Functions::DIVISION:
    case Functions::MODULO:
      return 30;
    case Functions::EXPONENTIATION:
      return 40;
    case Functions::TETRATION:
      return 50;
    case Functions::UNSUBTRACTION:
      return 100;
    case Functions::FACTORIAL:
      return 110;
    case Functions::EQUALS:
    case Functions::GT:
    case Functions::LT:
    case Functions::GE:
    case Functions::LE:
      return 10;
    case Functions::NULLOPERATOR:
      return 200;
    default:
      return 50;
  }
};

struct FunctionProperties
{
  const unsigned short args;
  const bool associative;
  // This property may be interpreted as the fixing of unary operations (left: false, right: true)
  // e.g. FACTORIAL: x!, associative: true because it's postfix        PRIMORIAL: #x, associative: false because it's prefix
  // It doesn't affect functions like SQRT which are written using commands

  FunctionProperties(const unsigned short args, const bool associative) : args(args), associative(associative) {};
};

std::unordered_map<Functions,FunctionProperties> properties =
{
  {Functions::ADDITION,FunctionProperties(2,true)},
  {Functions::SUBTRACTION,FunctionProperties(2,false)},
  {Functions::MULTIPLICATION,FunctionProperties(2,true)},
  {Functions::DIVISION,FunctionProperties(2,false)},
  {Functions::EXPONENTIATION,FunctionProperties(2,false)},
  
  {Functions::CHOOSE,FunctionProperties(2,false)},
  {Functions::FKSTIRLING,FunctionProperties(2,false)},
  {Functions::UNGSTIRLING,FunctionProperties(2,false)},
  {Functions::SKSTIRLING,FunctionProperties(2,false)},
  
  {Functions::FACTORIAL,FunctionProperties(1,true)},
  {Functions::PRIMORIAL,FunctionProperties(1,false)},
  {Functions::SQRT,FunctionProperties(1,false)},
  {Functions::UNSUBTRACTION,FunctionProperties(1,false)},
  
  {Functions::EQUALS,FunctionProperties(2,true)}
};

struct ASTLeaf
{
  std::variant<AtomicTypes, Functions, AuxiliaryTypes> type;
  std::variant<int,double> value; // Value may be interpreted in different forms (double, bool, ...) depending on the type
  std::string toString()
  {
    std::string result;
    if (std::holds_alternative<AtomicTypes>(type))
    {
      switch (std::get<AtomicTypes>(type))
      {
        case AtomicTypes::REAL:
        return std::to_string(std::get<double>(value));
        case AtomicTypes::INTEGER:
        return std::to_string(std::get<int>(value));
        case AtomicTypes::CONSTANT:
        for (const auto& [key, val] : constants)
        {
          if (val == (Constants)std::get<int>(value))
          {
            return (std::string)key;
          }
        }
        std::cout << "ERROR: Constant string representation not found (Constant nº " << std::get<int>(value) << ")\n";
        return "#";
      }
    }
    else if (std::holds_alternative<Functions>(type))
    {
      result += "(f|" +  std::to_string((int)std::get<Functions>(type)) + "|";
    }
    else
    {
      return "(x|" +  std::to_string((int)std::get<AuxiliaryTypes>(type)) + ")";
    }
    if (std::holds_alternative<int>(value))
    {
      return result + std::to_string(std::get<int>(value)) + ")";
    }
    else
    {
      return result + std::to_string(std::get<double>(value)) + ")";
    }
  }
  ASTLeaf(std::variant<AtomicTypes,Functions,AuxiliaryTypes> type) : type(type){};
  ASTLeaf(Constants constant) : type(AtomicTypes::CONSTANT)
  {
    value = (int)constant;
  };
  ASTLeaf() {};
};

struct AST
{
  std::vector<std::variant<ASTLeaf,AST>> args;
  Functions type;
  std::string toString()
  {
    std::string res = "{["+std::to_string((int)type)+"] ";
    for (auto arg:args)
    {
      if (arg.index() == 0)
      {
        res += std::get<ASTLeaf>(arg).toString();
      }
      else
      {
        res += std::get<AST>(arg).toString();
      }
      res+=' ';
    }
    return res + '}';
  }
};

class Tokenizer
{
  public:
  Tokenizer(const std::string_view& input) : input(input){
    tokens.reserve(input.length()/5);
  }
  std::vector<ASTLeaf> Tokenize()
  {
    while (pos < input.size())
    {
      const char& current = input[pos];
      if (current == '\\')
      {
        tokens.emplace_back(parseCommand());
      } else if (groupers.find(current) != groupers.end()) {
        tokens.emplace_back(ASTLeaf(groupers[current]));
        pos++;
      } else if (std::isdigit(current)||current == '.') {
        tokens.emplace_back(parseNumber());
      } else if (std::isalpha(current)) {
        //tokens.emplace_back(parseVariable());
      } else if (operators.find(current) != operators.end()) {
        // Hard code unary addition and subtraction
        if (current == '-')
        {
          if (tokens.empty() || tokens.back().type.index() == 1 && !(properties.at(std::get<Functions>(tokens.back().type)).args == 1 && properties.at(std::get<Functions>(tokens.back().type)).associative))
          {
            tokens.emplace_back(ASTLeaf(Functions::UNSUBTRACTION));
            pos++;
            continue;
          }
        }
        tokens.emplace_back(ASTLeaf(operators[current]));
        pos++;
      } else {
        pos++;
      }
    }
    return tokens;
  }
  private:
  const std::string_view input;
  std::vector<ASTLeaf> tokens;
  size_t pos = 0;

  ASTLeaf parseCommand() {
    size_t startPos = ++pos;
    while (pos < input.size() && std::isalnum(input[pos])) {
      pos++;
    }
    std::string_view command(input.data() + startPos, pos - startPos);
    if (commands.find(command) != commands.end())
    {
      return ASTLeaf(commands[command]);
    }
    else if (constants.find(command) != constants.end())
    {
      return ASTLeaf(constants[command]);
    }
    return ASTLeaf(Functions::NULLOPERATOR);
  }

  ASTLeaf parseNumber() {
    size_t startPos = pos;
    bool decimal = false;
    while (pos < input.size() && (std::isdigit(input[pos]) || input[pos] == '.')) {
      pos++;
      if (input[pos] == '.')
      {
        decimal = true;
      }
    }
    std::string_view number(input.data() + startPos, pos - startPos);
    if (decimal)
    {
      auto ast = ASTLeaf(AtomicTypes::REAL);
      ast.value = std::stod(std::string(number));
      return ast;
    }
    else
    {
      auto ast = ASTLeaf(AtomicTypes::INTEGER);
      ast.value = std::stoi(std::string(number));
      return ast;
    }
  }

  /*
  ASTLeaf parseVariable() {
    size_t startPos = pos;
    while (pos < input.size() && std::isalnum(input[pos])) {
      pos++;
    }
    std::string_view variable(input.data() + startPos, pos - startPos);
    return {TokenType::VARIABLE, variable};
  }
  */
};

class Parser
{
  public:
  // Converts a vector of ASTLeafs into reverse-polish notation
  std::queue<ASTLeaf> RPN(std::vector<ASTLeaf> nodes)
  {
    // Using the shunting-yard algorithm
    std::stack<ASTLeaf> operatorStack;
    std::queue<ASTLeaf> output;
    for (ASTLeaf leaf : nodes)
    {
      switch (leaf.type.index())
      {
        case 0: // AtomicTypes
        {
          output.emplace(leaf);
        }
        break;
        case 1: // Functions
        {
          Functions& newFunction = std::get<Functions>(leaf.type);
          while (!operatorStack.empty())
          {
            ASTLeaf& top = operatorStack.top();
            if (top.type.index() == 1 && getPriority(std::get<Functions>(top.type)) > getPriority(newFunction))
            {
              output.emplace(top);
              operatorStack.pop();
              continue;
            }
            break;
          }
          operatorStack.emplace(leaf);
        }
        break;
        case 2: // AuxiliaryTypes
        {
          AuxiliaryTypes& currentType = std::get<AuxiliaryTypes>(leaf.type);
          if (currentType == AuxiliaryTypes::LEFTPARENTHESIS||
              currentType == AuxiliaryTypes::LEFTBRACKET||
              currentType == AuxiliaryTypes::LEFTKEY||
              currentType == AuxiliaryTypes::LEFTKET||
              currentType == AuxiliaryTypes::RIGHTABS
            )
          {
            operatorStack.emplace(leaf);
          }
          // Empty stack and transfer to queue
          else{
            switch(currentType)
            {
              case AuxiliaryTypes::RIGHTPARENTHESIS:
              while (true)
              {
                ASTLeaf& top = operatorStack.top();
                if (top.type.index() == 2 && std::get<AuxiliaryTypes>(top.type) == AuxiliaryTypes::LEFTPARENTHESIS)
                {
                  operatorStack.pop();
                  break;
                }
                output.emplace(top);
                operatorStack.pop();
              }
              break;
              case AuxiliaryTypes::RIGHTBRACKET:
              while (true)
              {
                ASTLeaf& top = operatorStack.top();
                if (top.type.index() == 2 && std::get<AuxiliaryTypes>(top.type) == AuxiliaryTypes::LEFTBRACKET)
                {
                  operatorStack.pop();
                  break;
                }
                output.emplace(top);
                operatorStack.pop();
              }
              break;
              case AuxiliaryTypes::RIGHTABS:
              while (true)
              {
                ASTLeaf& top = operatorStack.top();
                if (top.type.index() == 2 && std::get<AuxiliaryTypes>(top.type) == AuxiliaryTypes::LEFTABS)
                {
                  operatorStack.pop();
                  break;
                }
                output.emplace(top);
                operatorStack.pop();
              }
              break;
              case AuxiliaryTypes::RIGHTKEY:
              while (true)
              {
                ASTLeaf& top = operatorStack.top();
                if (top.type.index() == 2 && std::get<AuxiliaryTypes>(top.type) == AuxiliaryTypes::LEFTKEY)
                {
                  operatorStack.pop();
                  break;
                }
                output.emplace(top);
                operatorStack.pop();
              }
              break;
              case AuxiliaryTypes::RIGHTKET:
              while (true)
              {
                ASTLeaf& top = operatorStack.top();
                if (top.type.index() == 2 && std::get<AuxiliaryTypes>(top.type) == AuxiliaryTypes::LEFTKET)
                {
                  operatorStack.pop();
                  break;
                }
                output.emplace(top);
                operatorStack.pop();
              }
              break;
            }
          }
        }
        break;
      };
    }
    while (!operatorStack.empty())
    {
      output.emplace(operatorStack.top());
      operatorStack.pop();
    }
    return output;
  }

  AST ParseRPN(std::queue<ASTLeaf> nodes)
  {
    // Keeps a record of all the atomic types as we look through the queue on our quest to find a function
    std::stack<std::variant<ASTLeaf,AST>> argumentStack;
    while (!nodes.empty())
    {
      switch (nodes.front().type.index())
      {
        case 0: // Atomic type
        {
          argumentStack.push(nodes.front());
          nodes.pop();
        }
        break;
        case 1: // Function
        {
          Functions& function = std::get<Functions>(nodes.front().type);
          FunctionProperties& functionProperties = properties.at(function);
          unsigned short args = functionProperties.args;
          AST ast;
          ast.type = function;
          ast.args.resize(args);
          for (int k = 0; k < args; k++)
          {
            std::variant<ASTLeaf,AST>& arg = argumentStack.top();
            ast.args[args-k-1] = arg;
            argumentStack.pop();
          }
          argumentStack.push(ast);
          nodes.pop();
        }
        break;
        case 2: // AuxiliaryTypes
        {
          std::cout << "ERROR: Auxiliary type in RPN";
          std::terminate();
        }
        break;
      }
    }
    if (argumentStack.empty())
    {
      AST ast;
      ast.type = Functions::IDENTITY;
      ast.args.emplace_back(nodes.front());
      return ast;
    }
    return std::get<AST>(argumentStack.top());
  }
};

int main()
{
  std::cout << "Heap allocations: " << numOfHeapAllocations << "\n";

  Tokenizer t = Tokenizer("3!-2!=4*-1*-1");
  Parser p;
  auto RPN = p.RPN(t.Tokenize());
  auto copy = RPN;
  while (!copy.empty())
  {
    std::cout << copy.front().toString() << "\n";
    copy.pop();
  }
  //auto ast = p.ParseRPN(RPN);

  //std::cout << ast.toString() << "\n";
  
  return 0;
};
