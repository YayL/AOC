#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

std::stack<char> lineStack;

int main(){
    std::ifstream file("../input.txt", std::ios::in);
    std::string line;
    char c;
    int val = 0;
    
    while(file >> line){
        PRINT(line);
        for(int i = 0; i < line.size(); i++){
            switch(line[i]){
                case '(':
                case '[':
                case '{':
                case '<':
                    lineStack.push(line[i]);
                    break;
                case ')':
                    if(lineStack.top() != '('){
                        val += 3;
                        goto outOfFor;
                    }
                    lineStack.pop();
                    break;
                case ']':
                    if(lineStack.top() != '['){
                        val += 57;
                        goto outOfFor;
                    }
                    lineStack.pop();
                    break;
                case '}':
                    if(lineStack.top() != '{'){
                        val += 1197;
                        goto outOfFor;
                    }
                    lineStack.pop();
                    break;
                case '>':
                    if(lineStack.top() != '<'){
                        val += 25137;
                        goto outOfFor;
                    }
                    lineStack.pop();
                    break;
            }
        }
        outOfFor:;
    }

    file.close();

    PRINT(val);
}