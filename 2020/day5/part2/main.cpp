#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

int8_t parition(std::string line, size_t start, size_t end, int8_t min, int8_t max){
    int8_t mid = (max + min + 1) >> 1;

    for(size_t i = start; i < end; i++){
        switch(line[i]){
            case 'F':
            case 'L':
                max = mid; break;
            case 'B':
            case 'R':
                min = mid; break;
        }
        mid = (max + min + 1) >> 1;
    }
    return min;
}

int main(){
    std::ifstream file("../input.txt", std::ios::in);
    bool seats[992] {false};
    std::string line;
    int row, col, max = 0;

    while(file >> line){
        row = parition(line, 0, 7, 0, INT8_MAX), col = parition(line, 7, 10, 0, (1 << 3) - 1);
        seats[row * 8 + col] = true;
    }
    for(int i = 1; i < 992; i++){
        if(!seats[i] && seats[i + 1] && seats[i - 1]){
            PRINT(i);
            break;
        }
            
    }

}