#include "StackPractice.h"

#include <cmath>
#include <cstring>

enum Operator:char {ADD='+',SUB='-',MUL='*',DIV='/',POW='^',FAC='!',NEG='N',L_P='(',R_P=')'};

EvaluateResultType strToNumber(const char* exp)
{
	bool isNegative = false;
	if (*exp == '-')
	{
		isNegative = true;
		++exp;
	}

	EvaluateResultType number = 0;

	while (isdigit(*exp))
	{
		number = number * 10 + static_cast<int>(*exp - '0');
		++exp;
	}

	if (*exp == '.')
	{
		++exp;

		EvaluateResultType base = 0.1f;
		while (isdigit(*exp))
		{
			number += static_cast<int>(*exp - '0') * base;
			++exp;
			base /= 10;
		}
	}

	if (isNegative) number = -number;

	return number;
}

const char rpnSeparateSymbol = ' ';

void appEndRpn(Vector<char>& RPN, float number)
{
	RPN.push_back(rpnSeparateSymbol);
	
	char buffer[64];
	if (number == (float)(int)number)
	{
		sprintf(buffer, "%i\0", (int)number);
	}
	else
	{
		sprintf(buffer, "%.2f\0", number);
	}
	char * p = buffer;
	while (*p != '\0')
	{
		RPN.push_back(*(p++));
	}
}

void appEndRpn(Vector<char>& RPN, Operator op)
{
	RPN.push_back(rpnSeparateSymbol);
	if (op == NEG)
	{
		RPN.push_back('N');
		RPN.push_back('E');
		RPN.push_back('G');
	}
	else
	{
		RPN.push_back(static_cast<char>(op));
	}
}

bool readNextNumber(const char*& exp, EvaluateResultType& number)
{
	number = strToNumber(exp);

	if (*exp == '-') ++exp;
	while (isdigit(*exp) || *exp == '.')
	{
		++exp;
	}

	return true;
}

bool shouldDelayCalculation(Operator opLhs, Operator opRhs)
{
	if (opLhs == L_P) return true;

	// Special case like 2^-2
	if (opLhs == POW && opRhs == NEG) return true;

	// Store the operator priority
	static Vector<Vector<Operator>> operatorPriority;
	static bool isInitialized = false;

	if(!isInitialized)
	{
		Vector<Operator> priority4;
		priority4.push_back(FAC);
		Vector<Operator> priority3;
		priority3.push_back(POW);
		Vector<Operator> priority2;
		priority2.push_back(NEG);
		Vector<Operator> priority1;
		priority1.push_back(MUL); priority1.push_back(DIV);
		Vector<Operator> priority0;
		priority0.push_back(ADD); priority0.push_back(SUB);

		operatorPriority.push_back(priority0);
		operatorPriority.push_back(priority1);
		operatorPriority.push_back(priority2);
		operatorPriority.push_back(priority3);
		operatorPriority.push_back(priority4);

		isInitialized = true;
	}

	// Get priority of two given operators
	int opLPriority = 0, opRPriority = 0;

	for (int i = 0; i < operatorPriority.size(); ++i)
	{
		auto p = operatorPriority[i];
		if (Algorithm::find(p.begin(), p.end(), opLhs) != p.end())
		{
			opLPriority = i;
			break;
		}
	}
	for (int i = 0; i < operatorPriority.size(); ++i)
	{
		auto p = operatorPriority[i];
		if (Algorithm::find(p.begin(), p.end(), opRhs) != p.end())
		{
			opRPriority = i;
			break;
		}
	}

	// If the right operator has higher priority, then the left calculation should be delayed.
	return opLPriority < opRPriority;
}

EvaluateResultType doCalculate(EvaluateResultType number1, const Operator op, EvaluateResultType number2)
{
	switch (op)
	{
	case ADD: return number1 + number2;
	case SUB: return number1 - number2;
	case MUL: return number1 * number2;
	case DIV: return number1 / number2;
	case POW: return pow(number1,number2);
	}
	return 0.f;
}

EvaluateResultType doCalculate(EvaluateResultType number1, const Operator op)
{
	int result = 0;
	switch (op)
	{
	case FAC:  // factorial
		result = static_cast<int>(number1);
		for (int i = result - 1; i > 0; --i)
		{
			result *= i;
		}
		return result > 0 ? result : 1;
	case NEG:  // Negative symbol
		return -number1;
	}
	return 0.f;
}

void doCalculateFromStack(Stack<EvaluateResultType>& numberStack, Stack<Operator>& operatorStack)
{
	// Get numbers and operator, push the result back to numberStack

	if (numberStack.empty() && operatorStack.empty()) return;

	Operator currentOpeator = operatorStack.top();
	operatorStack.pop();

	EvaluateResultType secondNumber = numberStack.top();
	numberStack.pop();

	// Check if it's unary operator
	if (currentOpeator == FAC || currentOpeator == NEG)
	{
		EvaluateResultType resultNumber = doCalculate(secondNumber, currentOpeator);
		numberStack.push(resultNumber);
	}
	else
	{
		EvaluateResultType firstNumber = numberStack.top();
		numberStack.pop();

		EvaluateResultType resultNumber = doCalculate(firstNumber, currentOpeator, secondNumber);
		numberStack.push(resultNumber);
	}
}

EvaluateResultType evaluate(const char* exp, Vector<char>& RPN)
{
	// Evaluate the result of an expression and convert it to RPN(Reverse Polish notation)

	if (*exp == '\0') return 0.f;
	// Create two stacks to store numbers and operators
	Stack<Operator> operatorStack;
	Stack<EvaluateResultType> numberStack;
	
	// Handle if the first character is positive/negative symbol
	if (*exp == '-')
	{
		operatorStack.push(NEG);
		++exp;
	}
	if (*exp == '+')
	{
		++exp;
	}
	Operator currentOperator;  // Used for check negative symbol
	while (true)
	{
		if (*exp == '\0') break;  // Reach end of the expression
		if (isdigit(*exp))
		{
			// Push number to stack
			EvaluateResultType currentNumber;
			if(!readNextNumber(exp, currentNumber)) return 0.f;
			numberStack.push(currentNumber);
			appEndRpn(RPN, currentNumber);
		}
		else
		{
			switch (*exp)
			{
			case '(':
				operatorStack.push(L_P);
				++exp;
				break;
			case ')':
				// Calculate the value of subexpression in ()
				while (operatorStack.top() != L_P)
				{
					appEndRpn(RPN, operatorStack.top());
					doCalculateFromStack(numberStack, operatorStack);
				}
				operatorStack.pop();
				++exp;
				break;
			case '-':
			case '+':
			case '*':
			case '/':
			case '^':
			case '!':
				// Check for negative symbol
				if (*exp == '-' && !isdigit(*(exp - 1)) && *(exp - 1) != ')' && *(exp - 1) != '!')
				{
					currentOperator = NEG;
					// If there are two negative symbols, just ignore them, since the number has not be in stack yet, we can't do calculation right now.
					if (operatorStack.top() == NEG)
					{
						operatorStack.pop();
						++exp;
						break;
					}
				}
				else 
					currentOperator = Operator(*exp);
				// Check if we should do the calculation now.
				// Like in num1 op1 num2 op2, if op1's priority is equal or higher than op2, than we can get result of num1 op1 num2.
				while (!operatorStack.empty() && !shouldDelayCalculation(operatorStack.top(), currentOperator))
				{

					appEndRpn(RPN, operatorStack.top());
					doCalculateFromStack(numberStack, operatorStack);
				}
				operatorStack.push(currentOperator);
				++exp;
				break;
			case ' ':
				++exp;
				break;
			default:
				return 0.f;
				break;
			}
		}
	}

	// Now calculate the final result
	while (!operatorStack.empty())
	{
		appEndRpn(RPN, operatorStack.top());
		doCalculateFromStack(numberStack, operatorStack);
	}

	if (numberStack.empty()) return 0.f;
	else return numberStack.top();
}