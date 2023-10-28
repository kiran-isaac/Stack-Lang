#include "../include/compiler.h"

using namespace std;

void Compiler::extract_functions(vector<Token> &tokens) {
  vector<Token> temp = vector<Token>();
  string func_name = "";
  for (vector<Token>::iterator i = tokens.begin(); i != tokens.end();) {
    if (i->type == KW_FUNC) {
      i++;
      if (i->type == ID) {
        func_name = i->val;
        i++;
      } else {
        COMPILER_FAIL << "Invalid Function Definition";
      }
    } else if (func_name != "" && i->type != KW_ENDFUNC) {
      temp.push_back(Token{i->type, i->val});
      i++;
    } else if (i->type == KW_ENDFUNC) {
      i++;
      functions[func_name] = vector<Token>();
      for (Token tk : temp) {
        functions[func_name].push_back(tk);
      }
      func_name = "";
      temp.clear();
    } else {
      COMPILER_FAIL << "Unexpected token: " << i->val << " Expected: FUNC";
    }
  }

  tokens.clear();
}