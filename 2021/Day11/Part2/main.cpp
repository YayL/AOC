#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

std::vector<std::vector<int>> grid;

const int width = 10, height = 10;
int flash = 0;
std::bitset<width * height> flashed;

void getAdjacent(int posX, int posY, std::vector<int>& ret){
    if(posX != 0){
        ret.push_back(posY * width + posX - 1);
        if(posY != 0)
            ret.push_back((posY - 1) * width + posX - 1);
    }

    if(posX != width - 1){
        ret.push_back(posY * width + posX + 1);
        if(posY != 0)
            ret.push_back((posY - 1) * width + posX + 1);
    }

    if(posY != 0){
        ret.push_back((posY - 1) * width + posX);
    }

    if(posY != height - 1){
        ret.push_back((posY + 1) * width + posX);
        if(posX != 0)
            ret.push_back((posY + 1) * width + posX - 1);
        if(posX != width - 1)
            ret.push_back((posY + 1) * width + posX + 1);
    }

}

void step(int x, int y){        
    if(!flashed[y * width + x] && ++grid[x][y] > 9){
        flash++;
        flashed[y * width + x] = true;
        grid[x][y] = 0;
        
        std::vector<int> adjacent;
        getAdjacent(x, y, adjacent);
        int sX, sY;

        for(auto &coord : adjacent){
            sX = coord % width, sY = coord / width;
            step(sX, sY);
        }
    }
}

int main(){
    std::vector<int> temp;
    std::ifstream file("../input.txt", std::ios::in);
    std::string line;
    temp.reserve(width);
    grid.reserve(height);
    
    while(file >> line){
        for(auto &c : line){
            temp.emplace_back(c - '0');
        }
        grid.emplace_back(temp);
        temp.clear();
    }
    file.close();
    int depth = 0;

    while(flash != flashed.size()){
        depth++;
        flashed.reset();
        flash = 0;
        for(int x = 0; x < width; x++){
            for(int y = 0; y < height; y++){
                step(x, y);
            }
        }
    }

    for(auto &row : grid){
        for(auto &val : row){
            std::cout << val;
        }
        PRINT("");
    }

    PRINT("Depth: " << depth);

}