#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

int main(){
    std::ifstream file("../input.txt", std::ios::in);
    std::string line, temp;
    std::bitset<26> group;

    int sum = 0;
    
    while(getline(file, line)){
        if(line.length() == 0){
            sum += group.count();
            group.reset();
            continue;
        }
        for(int i = 0; i < line.length(); i++)
            group[line[i] - 'a'] = true;
    }
    sum += group.count();
    file.close();

    PRINT(sum);

}