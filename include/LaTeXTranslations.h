#ifndef LATEXTRANSLATIONS_H
#define LATEXTRANSLATIONS_H

#include <unordered_map>
#include <string>

#include "../include/AtomicTypes.h"
#include "../include/Functions.h"
#include "../include/Constants.h"
#include "../include/AuxiliaryTypes.h"

std::unordered_map<Constants,std::string_view> constants =
{
  {Constants::PI,"π"},
  {Constants::e,"e"},
  {Constants::i,"i"},
  {Constants::PHI,"φ"}
};

std::string operator2LaTeX(Functions)
{
  
};

namespace opConversion
{

std::string unaryOp(Functions)
{

};

std::string regularOp(Functions)
{

};

}

std::unordered_map<Functions,std::string_view> commands =
{
  {Functions::SQRT,"sqrt"},

  {Functions::CHOOSE,"choose"},

  {Functions::GT,"gt"},
  {Functions::LT,"lt"},
  {Functions::GE,"ge"},
  {Functions::LE,"le"}
};

#endif