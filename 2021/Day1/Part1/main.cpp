#include <vector>
#include <iostream>
#include <fstream>

int main(){
    std::ifstream file("input.txt", std::ios::in);

    int num, prev, count = 0;
    file >> prev;

    while(file >> num){
        count += (num > prev);
        prev = num;
    }
    file.close();

    std::cout << count << std::endl;
}