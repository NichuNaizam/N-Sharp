#include "NS.h"
#include "Interpreter.h"

float order(const char& op);
float applyOp(const float& a, const float& b, const char& c);
float evaluateExpression(const string& expression, Interpreter& interpreter);