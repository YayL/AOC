#include <bits/stdc++.h>

int main(){
    std::vector<int> input;
    std::ifstream file("../input.txt", std::ios::in);
    
    std::string line;

    long arr[9] {0,0,0,0,0,0,0,0,0};
    
    while(file >> line){
        std::stringstream ss (line);
        while(getline(ss, line, ',')){
            if(line.length() != 0)
                arr[stoi(line)]++;
        }
    }
    file.close();

    long temp = 0;

    for(int year = 0; year < 256; year++){
        for(int i = 0; i < 8; i++){
            if(i == 0){
                temp = arr[0];
            }
            arr[i] = arr[i + 1];
        }
        arr[6] += temp;
        arr[8] = temp;
    }
    
    temp = 0;

    for(int i = 0; i < 9; i++){
        temp += arr[i];
    }

    std::cout << temp << std::endl;

}