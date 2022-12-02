#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;
#define PRINTS(x) std::cout << x;

struct Point {
    int64_t x, y, z;
    friend std::ifstream &operator<<(std::ifstream &, const Point &);
    friend bool operator<=>(const Point&, const Point&) = default;
};

template <> struct std::hash<Point> {
    std::size_t operator()(Point const &s)
}

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
            tempBound.push_back({lowerBound, upperBound});
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
        
    }

    PRINT(cubes.count());
    
}