#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

int main(){
    std::ifstream file("../input.txt", std::ios::in);
    std::string line;
    std::bitset<26> group, indiv;
    group.set();
    
    int sum = 0;
    
    while(getline(file, line)){
        if(line.length() == 0){
            sum += group.count();
            group.set();
            continue;
        }
        for(int i = 0; i < line.length(); i++)
            indiv[line[i] - 'a'] = true;
        group &= indiv;
        indiv.reset();
    }
    sum += group.count();
    file.close();

    PRINT(sum);

}