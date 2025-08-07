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
#include "../include/FunctionTypes.h"

static int numOfHeapAllocations = 0;
constexpr int CCIntervalIndex = (int)Functions::CCINTV;

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

// The following four functions can be optimized and reduced to a single line if the auxiliaryTypes enumeration keeps
//   the opening and closing characters in an alternate order: ('(',')','[',']','{','}')
//   however this implementation was selected because it allows the header file to be organized in any way

bool isOpeningGrouper(const AuxiliaryTypes x)
{
  switch(x)
  {
    case AuxiliaryTypes::LEFTKEY:
    case AuxiliaryTypes::LEFTBRACKET:
    case AuxiliaryTypes::LEFTPARENTHESIS:
    return true;
  }
  return false;
}

bool isClosingGrouper(const AuxiliaryTypes x)
{
  switch(x)
  {
    case AuxiliaryTypes::RIGHTKEY:
    case AuxiliaryTypes::RIGHTBRACKET:
    case AuxiliaryTypes::RIGHTPARENTHESIS:
    return true;
  }
  return false;
}

char getOppositeGrouper(const char c)
{
  switch (c)
  {
    case '(':
    return ')';
    case ')':
    return '(';
    case '[':
    return ']';
    case ']':
    return '[';
    case '{':
    return '}';
    case '}':
    return '{';
  }
  throw std::invalid_argument("Argument isn't an implemented grouper");
  return '@';
}

AuxiliaryTypes getOppositeGrouper(const AuxiliaryTypes x)
{
  switch (x)
  {
    case AuxiliaryTypes::LEFTPARENTHESIS:
    return AuxiliaryTypes::RIGHTPARENTHESIS;
    case AuxiliaryTypes::RIGHTPARENTHESIS:
    return AuxiliaryTypes::LEFTPARENTHESIS;
    case AuxiliaryTypes::LEFTBRACKET:
    return AuxiliaryTypes::RIGHTBRACKET;
    case AuxiliaryTypes::RIGHTBRACKET:
    return AuxiliaryTypes::LEFTBRACKET;
    case AuxiliaryTypes::LEFTKEY:
    return AuxiliaryTypes::RIGHTKEY;
    case AuxiliaryTypes::RIGHTKEY:
    return AuxiliaryTypes::LEFTKEY;
  }
  throw std::invalid_argument("Argument isn't an implemented grouper");
  return AuxiliaryTypes::NULLAUX;
}


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
  {"abs",Functions::ABS},

  {"choose",Functions::CHOOSE},

  {"gt",Functions::GT},
  {"lt",Functions::LT},
  {"ge",Functions::GE},
  {"le",Functions::LE},

  {"int",Functions::INT},
  {"lim",Functions::LIMIT},
  {"sum",Functions::SUM},
  {"prod",Functions::PROD},

  {"gcd",Functions::GCD},
  {"lcm",Functions::LCM},
  {"vec",Functions::VEC},

  {"matrix",Functions::MATRIX},

  {"interval",Functions::INTERVAL},
};

std::unordered_map<Functions,std::string_view> intervals =
{
  {Functions::CCINTV,"cc interval"},
  {Functions::COINTV,"co interval"},
  {Functions::OCINTV,"oc interval"},
  {Functions::OOINTV,"oo interval"}
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
    case Functions::PRIMORIAL:
      return 60;
    case Functions::UNSUBTRACTION:
      return 70;
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
      return 255;
  }
};

constexpr size_t getArgType()
{
  return 0;
};

// Some functions aren't present because their behaviour depends on context and so are hard-coded during the parsing process
std::unordered_map<Functions,FunctionTypes>properties =
{
  {Functions::ADDITION,FunctionTypes::BINARY},
  {Functions::SUBTRACTION,FunctionTypes::BINARY},
  {Functions::MULTIPLICATION,FunctionTypes::BINARY},
  {Functions::DIVISION,FunctionTypes::BINARY},
  {Functions::EXPONENTIATION,FunctionTypes::BINARY},
  
  {Functions::CHOOSE,FunctionTypes::BINARY},
  {Functions::FKSTIRLING,FunctionTypes::BINARY},
  {Functions::UNGSTIRLING,FunctionTypes::BINARY},
  {Functions::SKSTIRLING,FunctionTypes::BINARY},
  
  {Functions::FACTORIAL,FunctionTypes::UNARYRIGHT},
  {Functions::PRIMORIAL,FunctionTypes::UNARYLEFT},
  {Functions::UNSUBTRACTION,FunctionTypes::UNARYLEFT},
  
  {Functions::EQUALS,FunctionTypes::BINARY},

  {Functions::LIMIT,FunctionTypes::BIGOPERATOR},
  {Functions::INT,FunctionTypes::BIGOPERATOR},
  {Functions::SUM,FunctionTypes::BIGOPERATOR},
  {Functions::PROD,FunctionTypes::BIGOPERATOR},

  {Functions::SQRT,FunctionTypes::OPTIONALARGUMENTS},

  {Functions::LCM,FunctionTypes::ARRAYARGUMENTS},
  {Functions::GCD,FunctionTypes::ARRAYARGUMENTS},
  {Functions::VEC,FunctionTypes::ARRAYARGUMENTS},

  {Functions::MATRIX,FunctionTypes::MATRIXARGUMENTS},

  {Functions::INTERVAL,FunctionTypes::INTERVAL},

  {Functions::CCINTV,FunctionTypes::INTERVAL},
  {Functions::COINTV,FunctionTypes::INTERVAL},
  {Functions::OCINTV,FunctionTypes::INTERVAL},
  {Functions::OOINTV,FunctionTypes::INTERVAL},
};

struct ASTLeaf
{
  std::variant<AtomicTypes, Functions, AuxiliaryTypes> type;
  std::variant<int,double> value; // Value may be interpreted in different forms (double, bool, ...) depending on the type
  std::string toString() const
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
        case AtomicTypes::VARIABLE:
        result = "Variable '";
        result += (char)std::get<int>(value);
        return result + '\'';
      }
    }
    else if (std::holds_alternative<Functions>(type))
    {
      result += "(f|" +  std::to_string((int)std::get<Functions>(type)) + "|";
    }
    else
    {
      result +=  "(x|" +  std::to_string((int)std::get<AuxiliaryTypes>(type)) + "|";
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
  ASTLeaf(std::variant<AtomicTypes,Functions,AuxiliaryTypes> type,std::variant<int,double> value) : type(type), value(value){};
  ASTLeaf() {};
};

struct AST
{
  std::vector<std::variant<ASTLeaf,AST>> args;
  Functions type;
  std::string toString() const
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
  AST(Functions type,std::vector<std::variant<ASTLeaf,AST>> args) : type(type), args(args){};

  std::string toJSON()
  {
    std::string res = "";
    
    // Managing operators
    for (const auto& [key,val] : operators)
    {
      if (val == type)
      {
        res = "\"";
        res += key;
        res += "\":{";
        for (int i = 0; i < args.size(); i++)
        {
          auto& arg = args[i];
          if (std::holds_alternative<ASTLeaf>(arg))
          {
            res += std::get<ASTLeaf>(arg).toString();
          } else {
            res += std::get<AST>(arg).toJSON();
          }
          if (i != args.size()-1)
          {
            res+=",";
          }
        }
        return res+="}";
      }
    }

    // Managing commands
    if (type == Functions::IDENTITY) {
      if (std::holds_alternative<ASTLeaf>(args.at(0))) {
          return std::get<ASTLeaf>(args.at(0)).toString();
      } else {
          return std::get<AST>(args.at(0)).toJSON();
      }
    }
    switch (properties.at(type))
    {
      case FunctionTypes::BINARY:
      case FunctionTypes::ARRAYARGUMENTS:
      case FunctionTypes::MATRIXARGUMENTS:
      case FunctionTypes::UNARYLEFT:
      case FunctionTypes::UNARYRIGHT:
      for (const auto& [key,val] : commands)
      {
        if (val == type)
        {
          res = "\"";
          res += key;
          res += "\":{";
          for (int i = 0; i < args.size(); i++)
          {
            auto& arg = args[i];
            if (std::holds_alternative<ASTLeaf>(arg))
            {
              res += std::get<ASTLeaf>(arg).toString();
            } else {
              res += std::get<AST>(arg).toJSON();
            }
            if (i != args.size()-1)
            {
              res+=",";
            }
          }
          return res+="}";
        }
      }
      break;
      case FunctionTypes::INTERVAL:
      auto& key = intervals.at(type);
      std::cout << "cmd found: " << key << " \n";
      res = "\"";
      res += key;
      res += "\":{";
      for (int i = 0; i < args.size(); i++)
      {
        auto& arg = args[i];
        if (std::holds_alternative<ASTLeaf>(arg))
        {
          res += std::get<ASTLeaf>(arg).toString();
        } else {
          res += std::get<AST>(arg).toJSON();
        }
        if (i != args.size()-1)
        {
          res+=",";
        }
      }
      return res+="}";
    }
    return "Unable to translate to JSON";
  }
};

struct Environment
{
  std::vector<AST> expressions;
};

std::string printAST(std::variant<ASTLeaf,AST> ast)
{
  if (ast.index() == 0)
  {
    return std::get<ASTLeaf>(ast).toString();
  } else {
    return std::get<AST>(ast).toJSON();
  }
}

class Tokenizer
{
  public:
  Tokenizer(const std::string_view& input) : input(input){
    tokens.reserve(input.size()/5);
  }
  std::vector<ASTLeaf> Tokenize()
  {
    std::cout << '\'' << input << "'\n";
    std::cout << input.size() << '\n';
    while (pos < input.size())
    {
      std::cout << "Position " << pos << ", character '" << input[pos] << "'\n";
      std::cout << "Tokens: \n";
      for (auto t: tokens)
      {
        std::cout << t.toString() << '\n';
      }
      std::cout << "Big operators lookup:\n";
      for (auto [key, pair] : bigOperators)
      {
        std::cout << key << " " << pair << "\n";
      }
      std::cout << "Multi-argument operators lookup:\n";
      for (auto [key, pair] : multiArgumentOperators)
      {
        std::cout << key << " " << pair << "\n";
      }
      std::cout << "Interval lookup:\n";
      for (auto [key, pair] : intervalLookup)
      {
        std::cout << key << " " << pair << "\n";
      }
      const char& current = input[pos];
      if (current == '\\')
      {
        std::cout << "a\n";
        parseCommand();
      } else if (groupers.find(current) != groupers.end()) {
        std::cout << "b\n";
        switch (current)
        {
          case '(':
          case '[':
          case '{':
          if (bigOperators.find(depth) != bigOperators.end())
          {
            // Having a closing character before the opening character while having to complete a large operator must mean that the following group introduces
            //   the big operator's expression, therefore the lookup shall be removed
            std::cout << "Closing bracket with lookup\n";
            if (tokens.back().type.index() == 2 && std::get<AuxiliaryTypes>(tokens.back().type) == AuxiliaryTypes::RIGHTKEY)
            {
              std::cout << "Erase lookup " << depth << '\n';
              bigOperators.erase(depth);
            }
          } else if (intervalLookup.find(depth) != intervalLookup.end()) {
            // Having a closing character before the opening character while having to complete an interval must mean that the following group introduces
            //   the big operator's expression, therefore the lookup shall be removed
            std::cout << "Opening parenthesis with lookup\n";
            if (current == '(')
            {
              std::get<int>(tokens[intervalLookup[depth]].value)+=2;
            }
          }
          depth++;
          break;
          case ')':
          case ']':
          case '}':
          depth--;
          if (multiArgumentOperators.find(depth) != multiArgumentOperators.end())
          {
            // A closing grouper at the corresponding level as an opening grouper that came after a multiArgumentOperator must close this operator:
            // e.g. --> gcd(a,b,c)
            //             └─────┴─── These parentheses are in the same level so they limit the amount of arguments
            // This is also the reason why the depth decrements before checking the depht
            multiArgumentOperators.erase(depth);
          } else if (intervalLookup.find(depth) != intervalLookup.end()) {
            // Having a closing character before the opening character while having to complete an interval must mean that the following group introduces
            //   the big operator's expression, therefore the lookup shall be removed
            std::cout << "Closing parenthesis with lookup\n";
            if (current == ')')
            {
              ++std::get<int>(tokens[intervalLookup[depth]].value);
              bigOperators.erase(depth);
            }
          } else if (fracLookup.find(depth) != fracLookup.end()) {
            tokens.emplace_back(ASTLeaf(groupers[current]));
            tokens.emplace_back(ASTLeaf(Functions::DIVISION));
            fracLookup.erase(depth);
            pos++;
            continue;
          }
          break;
        }
        tokens.emplace_back(ASTLeaf(groupers[current]));
        pos++;
      } else if (std::isdigit(current)||current == '.') {
        std::cout << "c\n";
        tokens.emplace_back(parseNumber());
        std::cout << "c2\n";
      } else if (std::isalpha(current)) {
        std::cout << "d\n";
        tokens.emplace_back(parseVariable());
      } else if (operators.find(current) != operators.end()) {
        std::cout << "e\n";
        // Hard code unary subtraction
        if (current == '-')
        {
          if (tokens.empty() || tokens.back().type.index() == 2 && (std::get<AuxiliaryTypes>(tokens.back().type) == AuxiliaryTypes::COMMA || isOpeningGrouper(std::get<AuxiliaryTypes>(tokens.back().type))) || tokens.back().type.index() == 1 && !(properties.at(std::get<Functions>(tokens.back().type)) == FunctionTypes::UNARYRIGHT))
          {
            tokens.emplace_back(ASTLeaf(Functions::UNSUBTRACTION));
            pos++;
            continue;
          }
        }
        else if (current == '^')
        {
          // Check if ^ is used to express the superscript of a big operator
          if (bigOperators.find(depth) != bigOperators.end())
          {
            tokens[bigOperators.at(depth)].value = 2;
            tokens.emplace_back(AuxiliaryTypes::SUPERSCRIPT);
            pos++;
            continue;
          }
        }
        tokens.emplace_back(ASTLeaf(operators[current]));
        pos++;
      } else if (current == '_') {
        std::cout << "f\n";
        // Check if _ is used to express the subscript of a big operator
        if (bigOperators.find(depth) != bigOperators.end())
        {
          tokens[bigOperators.at(depth)].value = 1;
          tokens.emplace_back(AuxiliaryTypes::SUBSCRIPT);
          pos++;
          continue;
        }
        pos++;
      } else if (isEmpty()) {
        std::cout << "Consumed empty space\n";
        consumeEmpty();
      } else if (current == ','){
        // Add up the counter of arguments of a multi-argument operator when finding a comma
        if (multiArgumentOperators.count(depth-1) != 0)
        {
          std::get<int>(tokens.at(multiArgumentOperators[depth-1]).value)++;
        }
        tokens.emplace_back(AuxiliaryTypes::COMMA);
        pos++;
      } else if (current == ';'){
        std::get<int>(tokens[multiArgumentOperators[depth-1]].value)++;
        std::get<int>(tokens[multiArgumentOperators[depth-1]+1].value)++;
        // Shortcut: commas have the same sintactic and semantic meaning as semicolons
        tokens.emplace_back(AuxiliaryTypes::COMMA);
        pos++;
      } else {
        std::cout << "ERROR: Unidentified charcater during tokenization '" << current << "' at position " << pos;
      }
    }
    return tokens;
  }
  private:
  const std::string_view input;
  std::vector<ASTLeaf> tokens;
  // An element here represents the presence of a big operator. The key is the group depth (Brace depth) where the big operator was found and the value is the position of that operator
  std::unordered_map<uint64_t,size_t> bigOperators;
  std::unordered_map<uint64_t,size_t> multiArgumentOperators;
  std::unordered_map<uint64_t,size_t> intervalLookup;
  std::unordered_map<uint64_t,size_t> fracLookup;
  uint64_t depth = 0;

  size_t pos = 0;

  bool isEmpty()
  {
    return input[pos] == ' ';
  }

  void consumeEmpty()
  {
    while (isEmpty())
    {
      std::cout << "Consumed\n";
      pos++;
    }
  }

  void parseCommand() {
    std::cout << "parse command\n";
    size_t startPos = ++pos;
    while (pos < input.size() && std::isalnum(input[pos])) {
      pos++;
    }
    std::string_view command(input.data() + startPos, pos - startPos);
    if (commands.find(command) != commands.end())
    {
      Functions func = commands[command];
      // Add a * if the token before the function was an atomic type
      if (!tokens.empty() && std::holds_alternative<AtomicTypes>(tokens.back().type))
      {
        tokens.emplace_back(Functions::MULTIPLICATION);
      }
      switch(properties.at(func))
      {
        case FunctionTypes::BIGOPERATOR:
        consumeEmpty();
        if (groupers.find(input[pos]) != groupers.end() && isOpeningGrouper(groupers.at(input[pos])))
        {
          // Big operator instantly followed by a '{'
          tokens.emplace_back(func);
        }
        // Track the position of the big operator to modify its value later according to the amount of arguments
        bigOperators[depth] = tokens.size();
        tokens.emplace_back(func);
        break;
        case FunctionTypes::OPTIONALARGUMENTS:
        consumeEmpty();
        if (input[pos] == '[')
        {
          tokens.emplace_back(func,1);
        }
        else
        {
          tokens.emplace_back(func);
        }
        break;
        case FunctionTypes::ARRAYARGUMENTS:
        // Track the position of the multi-argument operator to modify its value later according to the amount of arguments
        multiArgumentOperators[depth] = tokens.size();
        tokens.emplace_back(func);
        break;
        case FunctionTypes::MATRIXARGUMENTS:
        // Track the position of the multi-argument operator to modify its value later according to the amount of columns
        multiArgumentOperators[depth] = tokens.size();
        tokens.emplace_back(func);
        // Adds an extra number that keeps track of the amount of rows
        tokens.emplace_back(AtomicTypes::INTEGER);
        break;
        case FunctionTypes::INTERVAL:
        consumeEmpty();
        intervalLookup[depth] = tokens.size();
        tokens.emplace_back(func);
        break;
        default:
        tokens.emplace_back(func);
        break;
      }
    }
    else if (constants.find(command) != constants.end())
    {
      // A constant next to an atomic type means multiplication
      if (!tokens.empty() && std::holds_alternative<AtomicTypes>(tokens.back().type))
      {
        tokens.emplace_back(Functions::MULTIPLICATION);
      }
      tokens.emplace_back(constants[command]);
    } else if (command == "frac") {
      fracLookup[depth] = tokens.size();
    }
  }

  ASTLeaf parseVariable()
  {
    const char &current = input[pos];
    pos++;
    // A variable next to an atomic type means multiplication
    if (!tokens.empty() && std::holds_alternative<AtomicTypes>(tokens.back().type))
    {
      tokens.emplace_back(Functions::MULTIPLICATION);
    }
    return ASTLeaf(AtomicTypes::VARIABLE,(int)current);
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
      std::cout << "[+] Current token: " << leaf.toString() << "\n";

      auto stackCopy = operatorStack;
      size_t stackSize = operatorStack.size();
      ASTLeaf operatorArray[stackSize];
      for (int i = stackSize-1; i >= 0; i--)
      {
        operatorArray[i] = stackCopy.top();
        stackCopy.pop();
      }
      std::cout << "\t[+] Operator Stack:\n";
      for (auto leaf : operatorArray)
      {
        std::cout << "\t  " << leaf.toString() << '\n';
      }

      auto queueCopy = output;
      std::cout << "\t[+] Output queue:\n";
      while (!queueCopy.empty())
      {
        std::cout << "\t  " << queueCopy.front().toString() << '\n';
        queueCopy.pop();
      }

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
            if (top.type.index() == 1 && getPriority(std::get<Functions>(top.type)) >= getPriority(newFunction))
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
          std::cout << "\t[+] Detected auxiliary type\n";
          AuxiliaryTypes& currentType = std::get<AuxiliaryTypes>(leaf.type);
          if (isOpeningGrouper(currentType))
          {
            operatorStack.emplace(leaf);
          }
          // Empty stack and transfer to queue
          else if (isClosingGrouper(currentType)) {
            AuxiliaryTypes openingGrouper = getOppositeGrouper(currentType);
            while (true)
            {
              ASTLeaf& top = operatorStack.top();
              // Check for opening groupers and commas since they both enclose well formed formulas preceding closing groupers:
              // (... , ...) (...)
              //      ^ ___  ^___       WFFs represented by underscores and triple dots, parse delimiters represented by carrets
              if (top.type.index() == 2 && (std::get<AuxiliaryTypes>(top.type) == openingGrouper || std::get<AuxiliaryTypes>(top.type) == AuxiliaryTypes::COMMA))
              {
                operatorStack.pop();
                break;
              }
              output.emplace(top);
              operatorStack.pop();
            }
          } else if (currentType == AuxiliaryTypes::COMMA) {
            // Check for opening groupers and commas since they both enclose well formed formulas preceding other commas:
            // (... , ...) (...)
            // ^___        ^___       WFFs represented by underscores and triple dots, parse delimiters represented by carrets
            while (true)
            {
              ASTLeaf& top = operatorStack.top();
              if (top.type.index() == 2 && (isOpeningGrouper(std::get<AuxiliaryTypes>(top.type)) || std::get<AuxiliaryTypes>(top.type) == AuxiliaryTypes::COMMA))
              {
                operatorStack.pop();
                break;
              }
              output.emplace(top);
              operatorStack.pop();
            }
            operatorStack.emplace(AuxiliaryTypes::COMMA);
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

  std::variant<ASTLeaf,AST> RPN2AST(std::queue<ASTLeaf> queue)
  {
    std::stack<std::variant<ASTLeaf,AST>> ASTStack;
    while (!queue.empty())
    {
      const ASTLeaf &current = queue.front();
      switch (current.type.index())
      {
        case 0: // Atomic Type
        ASTStack.emplace(current);
        queue.pop();
        break;
        case 1: // Function
        {
          switch (properties.at(std::get<Functions>(current.type)))
          {
            case FunctionTypes::BINARY:
            {
              auto arg2 = ASTStack.top();
              ASTStack.pop();
              auto arg1 = ASTStack.top();
              ASTStack.pop();
              std::vector<std::variant<ASTLeaf,AST>> vec = {arg1,arg2};
              ASTStack.emplace(AST(std::get<Functions>(current.type),vec));
              break;
            }
            case FunctionTypes::UNARYLEFT:
            case FunctionTypes::UNARYRIGHT:
            {
              std::vector<std::variant<ASTLeaf,AST>> vec = {ASTStack.top()};
              ASTStack.pop();
              ASTStack.emplace(AST(std::get<Functions>(current.type),vec));
              break;
            }
            case FunctionTypes::BIGOPERATOR:
            case FunctionTypes::OPTIONALARGUMENTS:
            case FunctionTypes::ARRAYARGUMENTS:
            {
              int argNum = std::get<int>(current.value);
              std::vector<std::variant<ASTLeaf,AST>> vec(argNum+1);
              while(argNum>=0)
              {
                vec[argNum] = ASTStack.top();
                ASTStack.pop();
                argNum--;
              }
              ASTStack.emplace(AST(std::get<Functions>(current.type),vec));
              break;
            }
            case FunctionTypes::MATRIXARGUMENTS:
            {
              size_t argNum = std::get<int>(current.value)+1;
              std::stack<std::variant<ASTLeaf,AST>> args;
              for (int i = 0; i < argNum; i++)
              {
                args.push(ASTStack.top());
                ASTStack.pop();
              }
              size_t rowNum = std::get<int>(std::get<ASTLeaf>(ASTStack.top()).value)+1;
              size_t rowLength = argNum/rowNum;
              std::vector<std::variant<ASTLeaf,AST>> rows(rowNum);
              for (int row = 0; row < rowNum; row++)
              {
                std::vector<std::variant<ASTLeaf,AST>> vec(rowLength);
                for (int column = 0; column < rowLength; column++)
                {
                  vec[column] = args.top();
                  args.pop();
                }
                auto ast = AST(Functions::VEC,vec);
                rows[row] = AST(Functions::VEC,vec);

              }
              ASTStack.emplace(AST(std::get<Functions>(current.type),rows));
              break;
            }
            case FunctionTypes::INTERVAL:
            {
              auto arg2 = ASTStack.top();
              ASTStack.pop();
              auto arg1 = ASTStack.top();
              ASTStack.pop();
              std::vector<std::variant<ASTLeaf,AST>> vec = {arg1,arg2};
              ASTStack.emplace(AST((Functions)(CCIntervalIndex+std::get<int>(current.value)),vec));
              break;
            }
          }
          queue.pop();
        }
        break;
      }
    }
    if (ASTStack.top().index() == 0)
    {
      std::vector<std::variant<ASTLeaf,AST>> vec = {std::get<ASTLeaf>(ASTStack.top())};
      return AST(Functions::IDENTITY,vec);
    }
    return ASTStack.top();
  }
};

int main()
{
  std::cout << "Heap allocations: " << numOfHeapAllocations << "\n";

  std::string input;
  std::getline(std::cin, input);
  Tokenizer t = Tokenizer(input);
  auto tokens = t.Tokenize();
  std::cout << "Tokenized" << std::endl;
  for (auto t: tokens)
  {
    std::cout << t.toString() << '\n';
  }
  std::cout << '\n';
  Parser p;
  auto RPN = p.RPN(tokens);
  auto copy = RPN;
  std::cout << "Reverse Polish Notation\n";
  while (!copy.empty())
  {
    std::cout << copy.front().toString() << "\n";
    copy.pop();
  }
  std::cout << input << '\n';
  //auto ast = p.ParseRPN(RPN);

  //std::cout << ast.toString() << "\n";

  auto ast = std::get<AST>(p.RPN2AST(RPN));

  std::cout << "AST:\n" << ast.toString() << '\n';

  std::cout << "JSON:\n" << ast.toJSON() << '\n';
  
  return 0;
};
