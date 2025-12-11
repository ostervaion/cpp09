#ifndef RPN_HPP
# define RPN_HPP

# include <iostream>
# include <stack>
# include <string>
# include <sstream>
# include <cstdlib>
# include <climits>

class RPN
{
	private:
		std::stack<long long>	_numbers;
		std::stack<char>		_operators;

		void					execute();
		bool					isOperator(char c);

	public:
		RPN();
		RPN(const RPN &src);
		~RPN();
		RPN &operator=(const RPN &src);

		void					process(std::string const &input);
		
		class ErrorException : public std::exception {
			public:
				virtual const char* what() const throw();
		};
};

#endif
