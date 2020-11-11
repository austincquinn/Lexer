#include "frontend/lexer.h"

namespace cs160::frontend {

std::vector<Token> Lexer::tokenize(const std::string & programText) {
  
  //int initStates[] = {1, 3, 6, 10, 13, 18, 24, 28, 35, 42, 44, 47, 51, 53, 55, 57, 59, 61, 63, 66, 68, 70, 72};
  //int finStates[] = {2, 5, 9, 12, 17, 23, 27, 34, 41, 43, 46, 50, 52, 54, 56, 58, 60, 62, 65, 67, 69, 71, 75};
  //int init = sizeof(initStates) / sizeof(initStates[0]);
  //int fin = sizeof(finStates) / sizeof(finStates[0]);  

  //std::vector<int> curr_states(initStates, initStates + init);
  std::set<int> curr_states = {1, 3, 6, 10, 13, 18, 24, 28, 35, 42, 44, 47, 51, 53, 55, 57, 59, 61, 63, 66, 68, 70, 72};
  unsigned long int curr_input_pos = 0;
  unsigned long int curr_scan_pos = 0;
  long long int curr_accepted_pos = -1;
  int length = 0;
  //std::vector<int> accepting_states(finStates, finStates + fin);
  long int accepted_state = -1;
  std::set<int> curr_accepting_states = {};
  std::set<int> accepting_states = {2, 5, 9, 12, 17, 23, 27, 34, 41, 43, 45, 46, 50, 52, 54, 56, 58, 60, 62, 65, 67, 69, 71, 75};
  std::set<int> :: iterator itr;

  initNFA();

  while (curr_scan_pos < programText.size())
  {
    curr_states = delta(curr_states, programText[curr_scan_pos]);
    curr_scan_pos++;


    for(itr = curr_states.begin(); itr != curr_states.end(); ++itr)
    {
      if(std::find(accepting_states.begin(), accepting_states.end(), *itr) != accepting_states.end())
      {
        curr_accepted_pos = curr_scan_pos;
        if(accepted_state == -1)
        {
          accepted_state = *itr;
          length = curr_accepted_pos - curr_input_pos;
        }
        else
        {
          if(curr_accepted_pos - curr_input_pos > length)
          {
            length = curr_accepted_pos - curr_input_pos;
            accepted_state = *itr;
          }
          else if (curr_accepted_pos - curr_input_pos == length)
          {
            accepted_state = prioritize(accepted_state, *itr);
          }    
          else{}
        }
      }
    }
/*	
    for(itr = curr_states.begin(); itr != curr_states.end(); ++itr)
    {
      //std::cout << ' ' << *itr;
      if(std::find(accepting_states.begin(), accepting_states.end(), *itr) != accepting_states.end())
      {
        curr_accepted_pos = curr_scan_pos;
	curr_accepting_states.clear();
      }
    }
    for(itr = curr_states.begin(); itr != curr_states.end(); ++itr)
    {
      if(std::find(accepting_states.begin(), accepting_states.end(), *itr) != accepting_states.end())
      {
        curr_accepting_states.insert(*itr);
      }
    }
    //std::cout << "\n";
*/
    if(curr_states.empty() || curr_scan_pos == programText.size())
    {
      if(curr_accepted_pos == -1)
      {
        throw InvalidLexemeError();
      }
      else
      {
        curr_scan_pos = curr_accepted_pos;
        curr_accepted_pos = -1;

        std::string info = programText.substr(curr_input_pos, (curr_scan_pos - curr_input_pos));
        info.erase(std::remove_if(info.begin(), info.end(), ::isspace), info.end());
        //emit accepting_state's highest priority token (w/ info if required); //make an emit function (giant switch)
        emitToken(accepted_state, info);

        curr_states.clear();
        //curr_states.insert(curr_states.begin(), std::begin(initStates), std::end(initStates));
        curr_states = {1, 3, 6, 10, 13, 18, 24, 28, 35, 42, 44, 47, 51, 53, 55, 57, 59, 61, 63, 66, 68, 70, 72};
        curr_input_pos = curr_scan_pos;
      }
    }
  }

  return tokens; // dummy return statement to return an empty vector, you should delete this line when implementing your tokenize method.
}

void Lexer::initNFA()
{

  //const int initStates[] = {1, 3, 6, 10, 13, 18, 24, 28, 35, 42, 44, 47, 51, 53, 55, 57, 59, 61, 63, 66, 68, 70, 72};
  //const int finStates[] = {2, 5, 9, 12, 17, 23, 27, 34, 41, 43, 46, 50, 52, 54, 56, 58, 60, 62, 65, 67, 69, 71, 75};
  //initStates.insert(initStates);
  //acceptStates.insert(finStates);
 

  char alphaLowerUpper[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                            'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
  char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

  /*ID*/
  for(int i = 0; i < 52; i++)
  {
    transitions[std::make_pair(1, alphaLowerUpper[i])] = 2;
  }
  for(int i = 0; i < 10; i++)
  {
    transitions[std::make_pair(2, alphaLowerUpper[i])] = 2;
  }
  for(int i = 0; i < 52; i++)
  {
    transitions[std::make_pair(2, alphaLowerUpper[i])] = 2;
  }

  /*Num*/
  transitions[std::make_pair(3, '-')] = 4;
  for (int i = 0; i < 10; i++)
  {
    transitions[std::make_pair(3, digits[i])] = 5;
    transitions[std::make_pair(4, digits[i])] = 5;
    transitions[std::make_pair(5, digits[i])] = 5;
  }

  /*Type*/
  transitions[std::make_pair(6, 'i')] = 7;
  transitions[std::make_pair(7, 'n')] = 8;
  transitions[std::make_pair(8, 't')] = 9;

  /*If*/
  transitions[std::make_pair(10, 'i')] = 11;
  transitions[std::make_pair(11, 'f')] = 12;

  /*Else*/
  transitions[std::make_pair(13, 'e')] = 14;
  transitions[std::make_pair(14, 'l')] = 15;
  transitions[std::make_pair(15, 's')] = 16;
  transitions[std::make_pair(16, 'e')] = 17;

  /*While*/
  transitions[std::make_pair(18, 'w')] = 19;
  transitions[std::make_pair(19, 'h')] = 20;
  transitions[std::make_pair(20, 'i')] = 21;
  transitions[std::make_pair(21, 'l')] = 22;
  transitions[std::make_pair(22, 'e')] = 23;

  /*Def*/
  transitions[std::make_pair(24, 'd')] = 25;
  transitions[std::make_pair(25, 'e')] = 26;
  transitions[std::make_pair(26, 'f')] = 27;

  /*Return*/
  transitions[std::make_pair(28, 'r')] = 29;
  transitions[std::make_pair(29, 'e')] = 30;
  transitions[std::make_pair(30, 't')] = 31;
  transitions[std::make_pair(31, 'u')] = 32;
  transitions[std::make_pair(32, 'r')] = 33;
  transitions[std::make_pair(33, 'n')] = 34;

  /*Output*/
  transitions[std::make_pair(35, 'o')] = 36;
  transitions[std::make_pair(36, 'u')] = 37;
  transitions[std::make_pair(37, 't')] = 38;
  transitions[std::make_pair(38, 'p')] = 39;
  transitions[std::make_pair(39, 'u')] = 40;
  transitions[std::make_pair(40, 't')] = 41;

  /*AOp*/
  transitions[std::make_pair(42, '+')] = 43;
  transitions[std::make_pair(42, '-')] = 43;
  transitions[std::make_pair(42, '*')] = 43;

  /*Rop*/
  transitions[std::make_pair(44, '<')] = 45;
  transitions[std::make_pair(45, '=')] = 46;
  transitions[std::make_pair(44, '=')] = 46;

  /*LBinop*/
  transitions[std::make_pair(47, '&')] = 48;
  transitions[std::make_pair(48, '&')] = 50;
  transitions[std::make_pair(47, '|')] = 49;
  transitions[std::make_pair(49, '|')] = 50;

  /*LNeg*/
  transitions[std::make_pair(51, '!')] = 52;

  /*LParen*/
  transitions[std::make_pair(53, '(')] = 54;

  /*RParen*/
  transitions[std::make_pair(55, ')')] = 56;

  /*LBrace*/
  transitions[std::make_pair(57, '{')] = 58;

  /*RBrace*/
  transitions[std::make_pair(59, '}')] = 60;

  /*Semicolon*/
  transitions[std::make_pair(61, ';')] = 62;

  /*Assign*/
  transitions[std::make_pair(63, ':')] = 64;
  transitions[std::make_pair(64, '=')] = 65;

  /*HasType*/
  transitions[std::make_pair(66, ':')] = 67;

  /*Comma*/
  transitions[std::make_pair(68, ',')] = 69;

  /*Whitespace*/
  transitions[std::make_pair(70, ' ')] = 71;
  transitions[std::make_pair(70, '\n')] = 71;
  transitions[std::make_pair(70, '\t')] = 71;

  /*Comments*/
  transitions[std::make_pair(72, '/')] = 73;
  transitions[std::make_pair(73, '/')] = 74;
  for(int i = 0; i < 10; i++)
  {
    char asciiVal = i;
    transitions[std::make_pair(74, asciiVal)] = 74;
  }
  for(int i = 11; i < 127; i++)
  {
    char asciiVal = i;
    transitions[std::make_pair(74, asciiVal)] = 74;
  }
  transitions[std::make_pair(74, '\n')] = 75;

}

std::set<int> Lexer::delta(std::set<int> old_states, char input)
{
  std::set<int> new_states;
  std::set<int> :: iterator itr;
  for(itr = old_states.begin(); itr != old_states.end(); ++itr)
  {
    if(transitions.find(std::make_pair(*itr, input)) != transitions.end())
    {
      new_states.insert(transitions[std::make_pair(*itr, input)]);
    }
  }
  return new_states;
}

int Lexer::prioritize(long int state1, int state2)
{
  if(state1 == 2)
  {
    return state2;
  }
  else
  {
    return state1;
  }
  
}

void Lexer::emitToken(long int accepted_state, std::string info)
{
  /*
  int max = 0;
  std::set<int> :: iterator itr;
  for(itr = accepting_states.begin(); itr != accepting_states.end(); ++itr)
  {
    if(*itr > max)
    {
      max = *itr;
    }
  }*/

  switch(accepted_state)
  {
    case 2:
      tokens.push_back(Token::makeId(info));
      break;
    case 5:
      {
      int num = std::stoi(info);
      tokens.push_back(Token::makeNum(num));
      }
      break;
    case 9:
      tokens.push_back(Token::makeType(info));
      break;
    case 12:
      tokens.push_back(Token::makeIf());
      break;
    case 17:
      tokens.push_back(Token::makeElse());
      break;
    case 23:
      tokens.push_back(Token::makeWhile());
      break;
    case 27:
      tokens.push_back(Token::makeDef());
      break;
    case 34:
      tokens.push_back(Token::makeReturn());
      break;
    case 41:
      tokens.push_back(Token::makeOutput());
      break;
    case 43:
      {
      	if(info == "+")
      	{
        	tokens.push_back(Token::makeArithOp(ArithOp::Plus));
     	}
      	if(info == "-")
      	{
        	tokens.push_back(Token::makeArithOp(ArithOp::Minus));
      	}
      	if(info == "*")
      	{
        	tokens.push_back(Token::makeArithOp(ArithOp::Times));
      	}
      }
      break;
    case 45:
        tokens.push_back(Token::makeRelOp(RelOp::LessThan));
        break;
    case 46:
      {
      	if(info == "<=")
      	{
        	tokens.push_back(Token::makeRelOp(RelOp::LessEq));
     	}
      	if(info == "=")
      	{
        	tokens.push_back(Token::makeRelOp(RelOp::Equal));
      	}
      }
      break;
    case 50:
      {
      	if(info == "&&")
      	{
        	tokens.push_back(Token::makeLBinOp(LBinOp::And));
     	}
      	if(info == "||")
      	{
        	tokens.push_back(Token::makeLBinOp(LBinOp::Or));
      	}
      }
      break;
    case 52:
      tokens.push_back(Token::makeLNeg());
      break;
    case 54:
      tokens.push_back(Token::makeLParen());
      break;
    case 56:
      tokens.push_back(Token::makeRParen());
      break;
    case 58:
      tokens.push_back(Token::makeLBrace());
      break;
    case 60:
      tokens.push_back(Token::makeRBrace());
      break;
    case 62:
      tokens.push_back(Token::makeSemicolon());
      break;
    case 65:
      tokens.push_back(Token::makeAssign());
      break;
    case 67:
      tokens.push_back(Token::makeHasType());
      break;
    case 69:
      tokens.push_back(Token::makeComma());
      break;
    case 71:
      break;
    case 75:
      break;
    default:
      break;
  }
}

}  // namespace cs160::frontend
