#include "RPN.hpp"

RPN::RPN() {}

RPN::RPN(const RPN &src) {
	*this = src;
}

RPN::~RPN() {}

RPN &RPN::operator=(const RPN &src) {
	if (this != &src) {
		this->_numbers = src._numbers;
		this->_operators = src._operators;
	}
	return *this;
}

const char* RPN::ErrorException::what() const throw() {
	return "Error";
}

bool RPN::isOperator(char c) {
	return (c == '+' || c == '-' || c == '*' || c == '/');
}

void RPN::execute() {
	if (_numbers.size() < 2 || _operators.empty())
		throw ErrorException();

	long long b = _numbers.top(); _numbers.pop();
	long long a = _numbers.top(); _numbers.pop();
	char op = _operators.top(); _operators.pop();

	long long result = 0;
	switch (op) {
		case '+': result = a + b; break;
		case '-': result = a - b; break;
		case '*': result = a * b; break;
		case '/':
			if (b == 0) throw ErrorException();
			result = a / b;
			break;
	}
	_numbers.push(result);
}

void RPN::process(std::string const &input) {
	std::stringstream ss(input);
	std::string token;

	while (ss >> token) {
		if (token.length() == 1 && isOperator(token[0])) {
			_operators.push(token[0]);
			execute();
		} else {
			// Check if it's a valid number < 10 (single digit logic usually applies to input components in RPN exercises, 
			// but prompt said "program must process ... inverted Polish mathematical expression ... where the numbers ... are always less than 10")
			// So we check if it is a single digit number.
			if (token.length() == 1 && isdigit(token[0])) {
				_numbers.push(atol(token.c_str()));
			} else {
				// Handle potentially negative numbers or invalid tokens
				// Subject usually implies: "numbers are always less than 10". 
				// Often interpreted as single digits 0-9.
				// If negative numbers are allowed as input (e.g. -5), let's handle that if needed, 
				// but strict RPN usually space separates everything. "- 5" vs "-5".
				// Given "numbers < 10", it implies 0-9.
				// Verify if we need to handle negative numbers as inputs or just results. 
				// Usually inputs are positive single digits.
				// Let's stick to strict single digit interpretation first as per usual 42 subject interpretation.
				throw ErrorException();
			}
		}
	}
	
	if (_numbers.size() != 1 || !_operators.empty())
		throw ErrorException();
		
	std::cout << _numbers.top() << std::endl;
}
