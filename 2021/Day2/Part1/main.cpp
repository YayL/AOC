#include <iostream>
#include <fstream>
#include <vector>


int main(){
    int pos[2] {0, 0};
    std::vector<std::string> input;
    std::ifstream file("input.txt", std::ios::in);

    std::string line;

    while(file >> line){
        input.push_back(line);
    }
    file.close();
    int depth;

    std::cout << input.size() << std::endl;

    for(int i = 0; i < input.size(); i += 2){
        depth = std::stoi(input.at(i + 1));
        line = input.at(i);

        if(line == "forward")
            pos[0] += depth;
        else if(line == "down")
            pos[1] += depth;
        else if(line == "up")
            pos[1] -= depth;
    }

    std::cout << pos[0] << " x " << pos[1] << " = " << pos[0] * pos[1] << std::endl;
}