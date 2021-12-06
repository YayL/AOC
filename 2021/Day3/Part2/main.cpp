#include <vector>
#include <iostream>
#include <fstream>

void count(std::vector<std::string> input, std::vector<int>& memo){
    int ones, zeros;
    for(int bitpos = 0; bitpos < input[0].length(); bitpos++){
        ones = 0, zeros = 0;
        for(int i = 0; i < input.size(); i++){
            if(input[i][bitpos] == '1')
                ones++;
            else
                zeros++;
        }
        memo.push_back(ones - zeros);
    }
    std::cout << std::endl;
}

std::string eval(std::vector<std::string> input, std::vector<int> memo, bool isOxygen){
    for(int bitpos = 0; bitpos < input[0].length(); bitpos++){
        for(int i = 0; i < input.size(); i++){
            if(!((memo[bitpos] >= 0) == (isOxygen ? (input[i][bitpos] - '0') : !(input[i][bitpos] - '0')))){
                (input[i][bitpos] - '0') ? memo[bitpos]++ : memo[bitpos]--;
                input.erase(input.begin() + i--);
            }
            if(input.size() == 1)
                return input[0];
        }
    }
    for(int i = 0; i < input.size(); i++){
        std::cout << input[i] << std::endl;
    }
    return "";
}

int main(){
    std::vector<std::string> input;
    std::ifstream file("input.txt", std::ios::in);

    std::string num;

    while(file >> num){
        input.push_back(num);
    }
    file.close();

    std::vector<int> memo;

    count(input, memo);
    int oxy = std::stoi(eval(input, memo, true), 0, 2), carbon = std::stoi(eval(input, memo, false), 0, 2);
    std::cout << "Oxygen = " << oxy << std::endl;
    std::cout << "CO2 = " << carbon << std::endl;
    std::cout << "Oxygen * CO2 = " << (oxy * carbon) << std::endl; 

}