#pragma once
#include "frontend/token.h"
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <set>

namespace cs160::frontend {

// The lexer should throw this exception if the input program contains invalid lexemes.
struct InvalidLexemeError : public std::runtime_error {
  InvalidLexemeError() : runtime_error("Invalid lexeme in input program") {}
};

// This is the lexer class you need to implement. The function you need to implement is the tokenize() method. You can define other class members such as fields or helper functions.
class Lexer final {
 public:

  std::map<std::pair<int,char>, int> transitions;

  std::vector<Token> tokens;

  // The entry point of the lexer you need to implement. It takes the program text as the argument and produces a vector of tokens as the result of lexing the program text.
  //
  // If the input program contains invalid lexemes (lexemes that are outside our specification), this method should throw InvalidLexemeError.
  //
  // The implementation of this method should go into lexer.cpp

  std::vector<Token> tokenize(const std::string & programText);
  
  void initNFA();
	
  int prioritize(long int state1, int state2);

  std::set<int> delta(std::set<int> old_states, char input);

  void emitToken(long int accepted_state, std::string info);
};

}  // namespace cs160::frontend
