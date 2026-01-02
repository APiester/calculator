Hello!

Welcome to my calculator program.

This is a c++ program that can calulate simple expressions.

It uses a binary search and shunting yard algorithm to calculate the expressions.

The way it works is as follows:
1. Tokenization: The input string is broken down into tokens (numbers, operators, and parentheses).
2. Data Curation: The tokens are converted from infix notation to postfix notation using the shunting-yard algorithm. This step takes into account operator precedence and parentheses.
3. Data Tree Construction: A binary expression tree is built from the result of the data curation. Each node represents either an operator or an operand.
4. Evaluation: The expression tree is recursively evaluated to determine the final result.

Some examples of expressions that can be evaluated:
- "3 + 5"
- "10 + 2 * 6"
- "100 * 2 + 12"
- "100 * ( 2 + 12 )"
- "100 * ( 2 + 12 ) / 14"
- "(-5 + 3) + 9 - 2 + (-2 + -5)"
- "3 + 4 * 2 / (1 - 5) ^ 2 ^ 3"

Limitations:
- The calculator currently supports only basic arithmetic operations: addition (+), subtraction (-), multiplication (*), division (/) and exponentiation (^).
- It does not support functions like sine, cosine, tangent, square root, absolute value, factorial, or modulus or algebraic equations with a varialble.


Requirments:
- C++17 or later
- Standard Libraries used:
-   iostream
-   sstream
-   stack
-   vector
-   cctype
-   cmath
-   string
-   stdexcept
