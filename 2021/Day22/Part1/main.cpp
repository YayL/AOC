#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;
#define PRINTS(x) std::cout << x;

std::vector<bool> operation;
std::vector<std::vector<std::pair<int, int>>> bounds;

void split(std::string str, char delim, std::vector<std::string>& output){
    std::stringstream ss (str);
    std::string token;
    
    while(getline(ss, token, delim)){
        if(!token.empty())
            output.push_back(token);
    }
}

int main(){
    std::ifstream file("../input.txt", std::ios::in);
    std::string line, tempStr;
    std::vector<std::pair<int, int>> tempBound;
    std::vector<std::string> splitted;

    int lowerBound, upperBound, total;
    while(getline(file, line)){
        splitted.clear();
        split(line, ',', splitted);
        for(auto &str : splitted){
            sscanf(str.c_str(), "%*[^=]=%i..%i", &lowerBound, &upperBound);
            // total = lowerBound + upperBound, upperBound = std::max(lowerBound, upperBound), lowerBound = total - upperBound;
            if(lowerBound < -50 || upperBound > 50){
                break;
            }
            tempBound.push_back({lowerBound + 50, upperBound + 50});
        }
        if(tempBound.size() == 3){
            operation.push_back(splitted[0][0] + splitted[0][1] == 221);
            bounds.push_back(tempBound);
        }
        tempBound.clear();
    }
    file.close();

    std::bitset<100 * 100 * 100> cubes;

    for(int i = 0; i < bounds.size(); i++){
        if(operation[i]){
            for(int x = bounds[i][0].first; x <= bounds[i][0].second; ++x){
                for(int y = bounds[i][1].first; y <= bounds[i][1].second; ++y){
                    for(int z = bounds[i][2].first; z <= bounds[i][2].second; ++z){
                        cubes.set(z * 10000 + y * 100 + x);
                    }
                }
            }
        }else{
            for(int x = bounds[i][0].first; x <= bounds[i][0].second; ++x){
                for(int y = bounds[i][1].first; y <= bounds[i][1].second; ++y){
                    for(int z = bounds[i][2].first; z <= bounds[i][2].second; ++z){
                        cubes.reset(z * 10000 + y * 100 + x);
                    }
                }
            }
        }
    }

    PRINT(cubes.count());
    
}