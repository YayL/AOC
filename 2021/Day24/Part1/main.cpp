#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;
#define PRINTS(x) std::cout << x;

long values[4];
int modelIndex = 0;
std::vector<std::vector<std::string>> instructions;

std::string modelNumber = "100000000000000";

int getInput(){
    return modelNumber[modelIndex++] - '0';
}

void handleInstructions(){
    for(int i = 0; i < instructions.size(); ++i){
        std::string instruction = instructions[i][0];
        long &a = values[instructions[i][1][0] - 'w'];

        if(instruction == "inp"){
            values[instructions[i][1][0] - 'w'] = getInput();
            continue;
        }

        std::string b = instructions[i][2];

        if(instruction == "add"){
            if(isdigit(b.back()))
                a += std::stoi(b);
            else
                a += values[b[0] - 'w'];
        }
        else if(instruction == "mul"){
            if(isdigit(b.back()))
                a *= std::stoi(b);
            else
                a *= values[b[0] - 'w'];
        }
        else if(instruction == "div"){
            if(isdigit(b.back()))
                a /= std::stoi(b);
            else
                a /= values[b[0] - 'w'];
        }
        else if(instruction == "mod"){
            if(isdigit(b.back()))
                a %= std::stoi(b);
            else
                a %= values[b[0] - 'w'];
        }
        else if(instruction == "eql"){
            if(isdigit(b.back()))
                a = (a == std::stoi(b));
            else
                a = (a == (values[b[0] - 'w']));
        }
    }
}

int main(){
    std::ifstream file("../input.txt", std::ios::in);
    std::string line, token;
    std::vector<std::string> splitResult;
    
    
    while(getline(file, line)){
        std::stringstream ss (line);
        splitResult.clear();
        while(getline(ss, token, ' ')){
            if(token.length() != 0)
                splitResult.push_back(token);
        }
        instructions.push_back(splitResult);
        printf("Inst: %s: %s, %s\n", instructions.back()[0].c_str(), instructions.back()[1].c_str(), (instructions.back().size() == 3 ? instructions.back()[2].c_str() : "NULL"));
    }
    file.close();

    for(int a = 1; a < 10; ++a){
        modelNumber[0] = a;
        for(int b = 1; b < 10; ++b){
            modelNumber[1] = b;
            for(int c = 1; c < 10; ++c){
                modelNumber[2] = c;
                for(int d = 1; d < 10; ++d){
                    modelNumber[3] = d;
                    for(int e = 1; e < 10; ++e){
                        modelNumber[4] = e;
                        for(int f = 1; f < 10; ++f){
                            modelNumber[5] = f;
                            for(int g = 1; g < 10; ++g){
                                modelNumber[6] = g;
                                for(int h = 1; h < 10; ++h){
                                    modelNumber[7] = h;
                                    for(int i = 1; i < 10; ++i){
                                        modelNumber[8] = i;
                                        for(int j = 1; j < 10; ++j){
                                            modelNumber[9] = j;
                                            for(int k = 1; k < 10; ++k){
                                                modelNumber[10] = k;
                                                for(int l = 1; l < 10; ++l){
                                                    modelNumber[11] = l;
                                                    for(int m = 1; m < 10; ++m){
                                                        modelNumber[12] = m;
                                                        for(int n = 1; n < 10; ++n){
                                                            modelNumber[13] = n, modelIndex = 0;
                                                            handleInstructions();
                                                            PRINT(modelNumber);
                                                            handleInstructions();
                                                            if(values[3] == 1){
                                                                PRINT(values[3]);
                                                            }
                                                            values[0] = 0, values[1] = 0, values[2] = 0, values[3] = 0;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}