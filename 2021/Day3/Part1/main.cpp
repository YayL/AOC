#include <vector>
#include <iostream>
#include <fstream>

int main(){
    std::vector<std::string> input;
    std::ifstream file("input.txt", std::ios::in);

    std::string num;

    while(file >> num){
        input.push_back(num);
    }
    file.close();

    int gamma = 0, epsilon = 0, ones, zeros;
    for(int j = 0; j < 12; j++){
        ones = 0, zeros = 0;
        for(int i = 0; i < input.size(); i++){
            //std::cout << input.at(i) << std::endl;
            if((input.at(i)).at(j) == '1')
                    ones++;
                else
                    zeros++;
        }
        if(ones > zeros)
            gamma += (1 << (11 - j));
        else
            epsilon += (1 << (11 - j));
    }
    
    std::cout << gamma << " * " << epsilon << " = " << (gamma * epsilon) << std::endl;
}