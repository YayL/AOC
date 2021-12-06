#include <bits/stdc++.h>

std::vector<std::string> split(std::string str, char delim){
    std::vector<std::string> split;
    std::string word;

    for(char c : str){
        if(c == delim && word.length() != 0){
            split.push_back(word);
            word = "";
        }
        else
            word += c;
    }
    return split;
}

bool hasEnoughChar(std::string str, char c, int min, int max){
    int count = 0;
    for(int i = 0; i < str.length(); i++){
        if(str[i] == c)
            count++;
    }
    return (min <= c <= max);
}

int main(){
    std::vector<std::vector<std::string>> input;
    std::ifstream file("../input.txt", std::ios::in);

    std::string line;
    
    while(file >> line){    
        input.push_back(split(line, ' '));
    }
    file.close();

    std::vector<std::string> temp;

    int count = 0;

    for(int i = 0; i < input.size(); i++){
        temp = split(input[i][0], '-');
        std::cout << input[i][0] << std::endl;
        if(hasEnoughChar(input[i][2], input[i][1][0], stoi(temp[0]), stoi(temp[1])));
            count++;
    }

    std::cout << count << std::endl;
}