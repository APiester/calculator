#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include <cctype>
#include <cmath>
#include <string>
#include <stdexcept>

using namespace std;

/*

Author: Adam Piester
Date: 2024-06-10
Language: C++

This is the header file for a calculator program that can handle basic arithmetic operations, parentheses, and negative numbers.

Functions:
- tokenize: Tokenizes the input expression string into numbers, operators, and parentheses, handling negative numbers appropriately.
- datacuration: Converts the list of tokens from infix notation to postfix notation using the shunting-yard algorithm, with error checking for invalid expressions.
- buildTree: Constructs a binary expression tree from the postfix notation, with error checking for malformed expressions.
- eval: Recursively evaluates the expression tree to compute the final result.
- isOperator: Checks if a string is an operator.
- precedence: Returns the precedence level of an operator.

*/



// Create a node structure for the expression tree
struct Node {
    string value;
    Node* left;
    Node* right;
    Node(string val) : value(val), left(nullptr), right(nullptr) {}
};


// Check if a string is an operator
bool isOperator(const string& c) {
    return c == "+" || c == "-" || c == "*" || c == "/" || c == "^";
}
// Sets operator precedence
int precedence(const string& op) {
    if (op == "^") return 3;
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

// Tokenizer function that handles negative numbers
vector<string> tokenize(const string& expr) {
    vector<string> tokens;
    string num;
    for (size_t i = 0; i < expr.size(); i++) {
        char c = expr[i];

        if (isdigit(c) || c == '.') { // checks to see if it is a number
            num += c;
        } else {
            if (!num.empty()) {
                tokens.push_back(num);
                num.clear();
            }

            if (c == ' ') continue;

            string s(1, c);

            // Handle negative numbers
            if (s == "-" && (tokens.empty() || isOperator(tokens.back()) || tokens.back() == "(")) {
                // Look ahead for numbers
                string negNum = "-";
                size_t j = i + 1;
                while (j < expr.size() && (isdigit(expr[j]) || expr[j] == '.')) {
                    negNum += expr[j];
                    j++;
                }
                if (negNum.size() > 1) { // ensure there's at least one digit after the minus sign
                    tokens.push_back(negNum);
                    i = j - 1; // increment i
                } else {
                    throw runtime_error("Invalid use of negative sign. The minus sign is incorrectly placed."); // error handling for parenthesis
                }
            }
            else if (isOperator(s) || s == "(" || s == ")") { // checks to see if it is an operator or parenthesis
                tokens.push_back(s);
            }
            else {
                throw runtime_error("Invalid character in expression"); // error handling for invalid characters
            }
        }
    }
    if (!num.empty()) tokens.push_back(num);
    return tokens;
}

// Shunting-yard with error checking
vector<string> datacuration(const vector<string>& tokens) {
    vector<string> output;
    stack<string> ops;
    int parenBalance = 0; // parenBalance to track parentheses

    // Process each token
    for (size_t i = 0; i < tokens.size(); i++) {
        const string& token = tokens[i];
        // Check if token is a number (including negative numbers)
        if (isdigit(token[0]) || (token.size() > 1 && (isdigit(token[1]) || token[0] == '-'))) {
            output.push_back(token);
        } else if (isOperator(token)) { // check if token is an operator
            // Check for invalid operator placement
            if (i == 0 || isOperator(tokens[i-1]) || tokens[i-1] == "(")
                throw runtime_error("Operator in invalid position"); // error handling

            while (!ops.empty() && isOperator(ops.top()) && // check for operator precedence
                   ((precedence(token) <= precedence(ops.top())) && token != "^")) {
                output.push_back(ops.top());
                ops.pop();
            }
            ops.push(token);
        } else if (token == "(") { // check for opening parenthesis
            ops.push(token);
            parenBalance++;
        } else if (token == ")") { // check for closing parenthesis
            parenBalance--;
            if (parenBalance < 0) throw runtime_error("Mismatched parentheses"); // error handling for parenthesis mismatch
            while (!ops.empty() && ops.top() != "(") {
                output.push_back(ops.top());
                ops.pop();
            }
            if (ops.empty()) throw runtime_error("Mismatched parentheses"); // error handling for parenthesis mismatch
            ops.pop(); // remove "("
        }
    }

    if (parenBalance != 0) throw runtime_error("Mismatched parentheses"); // error handling for parenthesis mismatch

    while (!ops.empty()) {
        if (ops.top() == "(" || ops.top() == ")")
            throw runtime_error("Mismatched parentheses");
        output.push_back(ops.top());
        ops.pop();
    }

    return output;
}

// Build expression tree safely
Node* buildTree(const vector<string>& postfix) {
    stack<Node*> st;
    for (const auto& token : postfix) { 
        if (!isOperator(token)) {
            st.push(new Node(token)); // if the token is not an operator, create a new node and push it to the stack
        } else {
            if (st.size() < 2) throw runtime_error("Malformed expression"); // error handling for malformed expressions
            Node* right = st.top(); st.pop(); // if the token is an operator, pop two nodes from the stack
            Node* left = st.top(); st.pop(); 
            Node* node = new Node(token); // create a new node with the operator
            node->left = left; 
            node->right = right;
            st.push(node);
        }
    }
    if (st.size() != 1) throw runtime_error("Malformed expression"); // error handling for malformed expressions
    return st.top();
}

// Evaluate expression tree
double eval(Node* root) {
    if (!root->left && !root->right) return stod(root->value); // if the node is a leaf, return its value as a double

    double l = eval(root->left);
    double r = eval(root->right);

    // if the node is an operator, perform the operation on its children
    if (root->value == "+") return l + r;
    if (root->value == "-") return l - r;
    if (root->value == "*") return l * r;
    if (root->value == "/") {
        if (r == 0) throw runtime_error("Division by zero");
        return l / r;
    }
    if (root->value == "^") return pow(l, r);

    throw runtime_error("Unknown operator");
}