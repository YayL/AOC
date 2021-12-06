#include <vector>
#include <iostream>
#include <fstream>

std::vector<int> memo;

int count(std::vector<int> input){
    int count = 0;
    memo.push_back(input.at(0) + input.at(1) + input.at(2));
    for(int i = 1; i < input.size() - 2; i++){
        memo.push_back(input.at(i) + input.at(i + 1) + input.at(i + 2));
        
        count += (memo.at(i) > memo.at(i - 1));
    }
    return count;
}

int main(){
    std::vector<int> input;
    std::ifstream file("input.txt", std::ios::in);

    int num;

    while(file >> num){
        input.push_back(num);
    }
    file.close();

    std::cout << count(input) << std::endl;
}