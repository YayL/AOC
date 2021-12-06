#include <iostream>
#include <fstream>
#include <vector>

int main(){

    std::vector<int> input;
    std::ifstream file("../input.txt", std::ios::in);
    
    int num;
    
    while(file >> num){
        input.push_back(num);
    }
    file.close();

    for(int x = 0; x < input.size(); x++){
        for(int y = x; y < input.size(); y++){
            if(input[x] + input[y]== 2020){
                std::cout << (input[x] * input[y]) << std::endl; 
            }
        }
    }
}