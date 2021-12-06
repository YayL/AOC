#include <bits/stdc++.h>

int main(){
    std::vector<int> input;
    std::ifstream file("../input.txt", std::ios::in);
    
    std::string line;
    
    while(file >> line){
        std::stringstream ss (line);
        while(getline(ss, line, ',')){
            if(line.length() != 0)
                input.push_back(stoi(line));
        }
    }
    file.close();

    int size = 0;

    for(int year = 0; year < 80; year++){
        size = input.size();
        for(int i = 0; i < size; i++){
            if(input[i] == 0){
                input[i] = 6;
                input.push_back(8);
            }else
                input[i]--;
        }
    }

    std::cout << input.size() << std::endl;

}