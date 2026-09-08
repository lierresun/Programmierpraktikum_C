#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>

#define MAXSIZE 1024

template <typename T>
class Stack {
private:
    static int constexpr maxSize = MAXSIZE;
    T data[maxSize];
    int sp;
    
    void error(const char* s) const {
        std::cerr << s << std::endl;
        exit(1);
    }

public:
    Stack() : sp(-1) {}

    int len() const { return sp + 1; }

    bool full() const { return sp >= maxSize - 1; }

    bool empty() const { return sp < 0; }

    void push(T x) { 
        if (full()) error("Stack overflow.");
        data[++sp] = x; 
    }

    void pop() {
        if (empty()) error("Stack underflow.");
        --sp;
    }

    T peek() const {
        if (empty()) error("Stack is empty.");
        return data[sp];
    }
    //!TODO0
    //...


    friend std::ostream& operator<<(std::ostream& os, const Stack<T>& stack) {
        for (int i = 0; i <= stack.sp; ++i) {
            os << stack.data[i];
            if (i < stack.sp) os << ", ";
        }
        return os;
    }
    
};

std::string replaceBrackets(const std::string& input) {
    std::string result;

    result.reserve(input.size());
    for (char c : input) {
        if (c == '(' || c == ')' || c == ',') result += ' ';
        else result += c;
    }//!TODO1

    return result;
}

double PDA(const std::string& input) {
    Stack<double> stack;
    Stack<std::string> reverseTokens;
    std::istringstream stream(replaceBrackets(input));
    std::string token;
    double result, a, b, number;
    
    while (stream >> token) { reverseTokens.push(token); }

    while (!reverseTokens.empty()) {
        token = reverseTokens.peek();
        reverseTokens.pop();        
        //!TODO3

        if (  token == "+" || token == "-" || token == "*" || token == "/"/*!TODO4*/  ) {
            if (stack.len() < 2) throw std::runtime_error("Not enough operands.");
            
            a = stack.peek(); stack.pop();
            b = stack.peek(); stack.pop();

            if (token == "+") result = a + b;
            else if (token == "-") result = a - b;
            else if (token == "*") result = a * b;
            else result = a / b;

            stack.push(result);
            //!TODO5

        } else {
            try {
                number = std::stod(token);
                stack.push(number);
                //!TODO6
            } catch (...) {
                throw std::runtime_error("Invalid number format: " + token);
            }
        }
    }

    if (  stack.empty()/*!TODO7*/  ) throw std::runtime_error("Invalid expression: Stack is empty at the end.");

    
    result = stack.peek();
    stack.pop();
    //!TODO8

    if (  !stack.empty()/*!TODO9*/  ) throw std::runtime_error("Invalid expression: Stack is not empty at the end.");
    return result;
}

int main(int argc, char* argv[]) {
    std::string input;

    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            if (i > 1) input += " ";
            input += argv[i];
        }
    } else {
        input = "+ * 5 3 * 2 10"; //!TODO: Change for testing purposes.
    }

    try {
        double result = PDA(input);
        std::cout << result << std::endl;
    } catch (const std::exception& err) {
        std::cerr << "Error: " << err.what() << std::endl;
    }

    return 0;
}