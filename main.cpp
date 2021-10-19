#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <iostream>
#include <cstring>
#include <cmath>
using namespace std;

double CalculateExpression(const char s[], int& idx, int level = 1);
double EvaluateTerm(const char s[], int& idx);
double StringToNumber(const char s[], int& idx);
int OperatorPriority(char op);

double CalculateOperator(double left, double right, char op);
void Solve(char s[], int idx, int add, int sub, int mul, int div, int pow, bool seq_pow);
int FindSpace(const char s[], int idx);

const int MAX_EXPR_LENGTH = 100;        // Maximum number of the characters in a single expression
const int MAX_OPERATOR_LEVEL = 3;       // Maximum level of the priority of operators
double max_val = -INFINITY;             // Maximum value of mathematical expressions
char max_s[MAX_EXPR_LENGTH + 1] = "";   // Expression which returns the maximum value

int main() {
	char s[MAX_EXPR_LENGTH + 1] = "";
	int idx;

	idx = 0;
	cout << ">>";
	cin.getline(s, MAX_EXPR_LENGTH + 1);  // getline member function
	cout << s << endl;
	if (strcmp(s, "exit") == 0) {  // strcmp function
		cout << "Exiting.." << endl;
		exit(1);
	}

	int add, sub, mul, div, pow, enter;
	cout << "Number of add sub mul div pow\n>>";
	cin >> add >> sub >> mul >> div >> pow;

	Solve(s, idx, add, sub, mul, div, pow, false);

	if (isnan(max_val) || isinf(max_val)) { // isnan, isinf function, end of c-string = 0
		cout << "Error while evaluating!" << endl;
	}
	else {
		cout << "Maximum Value: " << max_val << endl;
		cout << "Expression: " << max_s << endl;
	}
	return 0;
}

void Solve(char s[], int idx, int add, int sub, int mul, int div, int pow, bool seq_pow) {
	/*
	It fills in the blanks in the given expression and finds the maximum value and the expression corresponding to that value
	add, sub, mul, div, pow represents the remaining number of each operator
	seq_pow represents whether previous operator was pow or not
	*/

	int new_idx = FindSpace(s, idx);

	if (idx == 0)
		max_val = -INFINITY, seq_pow = false; // initialization

	if (s[new_idx] == '\0') {
		/*
		Base Case
		- Calculate the completed formula
		- Compare the calculated value with the (current) maximum value
		*/

		int idn = 0;
		double value = CalculateExpression(s, idn);

		if (isnan(value) || isinf(value));
		else if (value > max_val) {
			max_val = value;
			strcpy(max_s, s);
		}
	}

	/*
	Recursive Step
	- Recognize empty space(‘ ’) and fill in the space with the given operator to create a complete expression
	*/

	else {
		if (add > 0) {
			s[new_idx] = '+';
			Solve(s, new_idx, add - 1, sub, mul, div, pow, false);
		}
		if (sub > 0) {
			s[new_idx] = '-';
			Solve(s, new_idx, add, sub - 1, mul, div, pow, false);
		}
		if (mul > 0) {
			s[new_idx] = '*';
			Solve(s, new_idx, add, sub, mul - 1, div, pow, false);
		}
		if (div > 0) {
			s[new_idx] = '/';
			Solve(s, new_idx, add, sub, mul, div - 1, pow, false);
		}
		if ((pow > 0) && !seq_pow) {
			s[new_idx] = '^';
			Solve(s, new_idx, add, sub, mul, div, pow - 1, true);
		}

		s[new_idx] = ' '; // reset for adjacent trees
	}
}

int FindSpace(const char s[], int idx) {
	// It finds the next index of the blank ‘ ’ in string s

	while (s[++idx] != '\0')
		if (s[idx] == ' ') break;

	return idx;
}

double StringToNumber(const char s[], int& idx) {
	//It converts the string to float number. Update the idx value to next unread index.

	double number = atof(&s[idx]);

	if (s[idx] == '-')
		idx++;
	while (((s[idx] > 47) && (s[idx] < 58)) || (s[idx] == '.'))
		idx++;

	return number;
}

double EvaluateTerm(const char s[], int& idx) {
	// It calculates the expression in parenthesis or converts the string to float number. idx value is updated to the next unread index.

	if (s[idx] == '(') {
		int depth = 1;
		int temp = ++idx;

		while (depth > 0) {
			if (s[idx] == '(')
				depth++;
			else if (s[idx] == ')')
				depth--;
			idx++;
		}

		int len = (idx - 1) - temp;
		char* ss = new char[len + 1];

		for (int i = 0; i < len; i++)
			ss[i] = s[temp + i];
		ss[len] = '\0';

		int idn = 0;
		double result = CalculateExpression(ss, idn);

		delete[] ss;
		return result;
	}
	else {
		return StringToNumber(s, idx);
	}
}

int OperatorPriority(char op) {
	// It returns the priority of given operator ‘op’

	switch (op) {
	case '+':
	case '-':
		return 1;
	case '*':
	case 'X':
	case 'x':
	case '/':
		return 2;
	case '^':
		return 3;
	default:
		return 0;
	}
}

double CalculateOperator(double left, double right, char op) {
	// It returns the result calculated by left and right using the given operator ‘op’

	switch (op) {
	case '+':
		return left + right;
	case '-':
		return left - right;
	case '*':
	case 'X':
	case 'x':
		return left * right;
	case '/':
		return left / right;
	case '^':
		return pow(left, right);
	default:
		return 0.0;
	}
}

double CalculateExpression(const char s[], int& idx, int level) {
	/*
	This calculates the expression recursively
	It returns the value calculated from idx to end point of expression
		- s[idx] to end point should be valid expression
		- Ex) *3/2+5 is invalid expression, 3+2*5 is valid expression
	Internally, it computes the value calculated by the results from left-subtree and right-subtree using the given operator s[idx]
	Deeper level operators should be computed first
	*/

	if (level > MAX_OPERATOR_LEVEL) {
		// Base Case : Return a number if the term is not an operator
		return EvaluateTerm(s, idx);
	}

	double left, right;
	char op;

	left = CalculateExpression(s, idx, level + 1);
	op = s[idx];

	while (OperatorPriority(s[idx]) == level) {
		/*
		Recursive Step
		- Check the priority of operator and move the index to the next number
		- Recursively set the left and right values ​​of the operator
		*/

		idx++;

		right = CalculateExpression(s, idx, level + 1);

		left = CalculateOperator(left, right, op);
		op = s[idx];
	}

	return left;
}
