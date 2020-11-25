#pragma once

#include "Stack.h"
#include "Vector.h"
#include "List.h"

#include <cctype>

template<typename Stack>
void numeralConvert(long long number, int base, Stack& result)
{
	if (base > 16 || base < 2) return;
	static const char digit[] = { '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	while (number != 0)
	{
		result.push(digit[number%base]);
		number /= base;
	}
}

template<unsigned N>
bool isValidExpression(const char(&exp)[N])
{
	Stack<char> s;
	for (int i = 0; i < N; ++i)
	{
		switch (exp[i])
		{
		case '(':
		case '[':
		case '{':
			s.push(exp[i]);
			break;

		case ')':
			if (s.empty()) return false;
			else if (s.top() != '(') return false;
			else s.pop();
			break;

		case ']':
			if (s.empty()) return false;
			else if (s.top() != '[') return false;
			else s.pop();
			break;

		case '}':
			if (s.empty()) return false;
			else if (s.top() != '{') return false;
			else s.pop();
			break;
		}
	}
	return s.empty();
}

// Evaluate an expression and get the RPN(Reverse Polish notation).
typedef float EvaluateResultType;
EvaluateResultType evaluate(const char* exp, Vector<char>& RPN);

// N Queen Problem
struct Position2D
{
	int x, y;
};
void placeNQueen(int N, List<List<Position2D>>& solutions);
void printNQueenSolution(List<Position2D>& solution);