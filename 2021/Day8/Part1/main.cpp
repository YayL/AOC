#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

int main(){
    std::ifstream file("../input.txt", std::ios::in);
    
    std::vector<std::string> input;
    std::vector<std::string> output;
    bool useOutput = false;
    char c;
    std::string token;
    int count = 0, length;

    while(file >> token){
        PRINT(token);
        if(useOutput)
            output.push_back(token);
        else
            input.push_back(token);

        if(token == "|"){
            useOutput = true;
            PRINT(123);
        }else if(file.get() == '\n'){
            useOutput = false;
            for(int i = 0; i < output.size(); i++){
                length = output[i].length();
                if(length == 2 || length == 4 || length == 3 || length == 7)
                    count++;
            }
            output.clear();
        }
    }
    file.close();

    PRINT(count);

}