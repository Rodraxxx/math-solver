#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <variant>
#include <string>
#include <memory>
#include "../include/Functions.h"
#include "../include/AtomicTypes.h"

unsigned nodesCreated,nodesDestroyed;

struct AtomicValue{
  AtomicTypes type;
  std::variant<int,double> value;
};

struct NodeType
{
  std::variant<AtomicValue,Functions> value;
};

struct ASTNode {
  ASTNode();
  ~ASTNode();
};

struct AtomicNode : ASTNode {
  AtomicTypes atomic;
  double value;
};

struct UnaryOpNode : ASTNode {
  std::shared_ptr<ASTNode> term;
};

struct BinaryOpNode : ASTNode {
  std::shared_ptr<ASTNode> left;
  //Token op;
  std::shared_ptr<ASTNode> right;
};

struct NaryOpNode : ASTNode {
  std::vector<std::shared_ptr<ASTNode>> operands;
  NaryOpNode(size_t length) {
    operands.resize(length);
  };

  std::shared_ptr<ASTNode> operator[](size_t i) {
    return operands[i];
  }
};

ASTNode::ASTNode()
{
  nodesCreated++;
}
ASTNode::~ASTNode()
{
  nodesDestroyed++;
}

/*

class Parser {
  std::vector<Token> tokens;
  size_t pos;

  public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

    Token current() const {}

    Token advance() {}

    std::shared_ptr<ASTNode> parseExpression();
    std::shared_ptr<ASTNode> parsePrimary();
    std::shared_ptr<ASTNode> parseBinaryOpRHS(int exprPrec, std::shared_ptr<ASTNode> lhs);
};

*/

#endif PARSER_H