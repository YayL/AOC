#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

std::vector<std::vector<int>> heightMap;
std::vector<std::vector<int>> basins;

std::bitset<10000> checked;

int width, height;

void printBasin(int basin){
    printf("%i", basins[basin][0]);
    for(int i = 0; i < basins[basin].size(); i++){
        printf(",%i", basins[basin][i]);
    }
    PRINT("");
}

void addToBasin(int posX, int posY, int basin){
    int coord = posY * width + posX;
    basins[basin].push_back(coord);
    checked[coord] = true;
}

int inOtherBasin(int posX, int posY){
    int coord = posY * width + posX;
    if(checked[coord])
        return coord;
    
    return -1;
}

void getAdjacent(int posX, int posY, std::vector<int>& ret){
    if(posX != 0 && heightMap[posY][posX - 1] != 9 && inOtherBasin(posX - 1, posY) == -1)
        ret.push_back(posY * width + posX - 1);

    if(posX != width - 1 && heightMap[posY][posX + 1] != 9 && inOtherBasin(posX + 1, posY) == -1)
        ret.push_back(posY * width + posX + 1);

    if(posY != 0 && heightMap[posY - 1][posX] != 9 && inOtherBasin(posX, posY - 1) == -1)
        ret.push_back((posY - 1) * width + posX);

    if(posY != height - 1 && heightMap[posY + 1][posX] != 9 && inOtherBasin(posX, posY + 1) == -1)
        ret.push_back((posY + 1) * width + posX);
}

// Recursively add everything that is one less and one more to a basin which is stored as a vector with their coordinates

void getBasins(int posX = 0, int posY = 0, int basin = -1){
    std::vector<int> sides;

    if(basin == -1){
        basin = basins.size();
        basins.push_back(sides);
        addToBasin(posX, posY, basin);
    }

    int sX, sY, diff;
    getAdjacent(posX, posY, sides);
    for(int i = 0; i < sides.size() && !checked[sides[i]]; i++){
        sY = sides[i] / width, sX = sides[i] % width;
        addToBasin(sX, sY, basin);
        getBasins(sX, sY, basin);
    }
}

int main(){
    std::vector<int> temp;
    std::ifstream file("../input.txt", std::ios::in);
    std::string line;
    
    while(file >> line){
        temp.clear();
        for(int i = 0; i < line.length(); i++)
            temp.push_back(line[i] - '0');
        heightMap.push_back(temp);
    }

    width = heightMap[0].size(), height = heightMap.size();
    file.close();

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(inOtherBasin(x, y) == -1 && inOtherBasin(x, y) == -1 && heightMap[y][x] != 9)
                getBasins(x, y);
        }
    }

    int size, smallest = 0, middle = 0, biggest = 0;
    
    for(int i = 0; i < basins.size(); i++){
        size = basins[i].size();
        if(size > biggest){
            if(biggest <= middle)
                smallest = biggest;
            else
                middle = biggest;
            biggest = size;
        }
        else if(size > middle){
            smallest = middle;
            middle = size;
        }
        else if(size > smallest)
            smallest = size;
    }


    PRINT(biggest << ", " << middle << ", " << smallest);
    PRINT("Result: " << biggest * middle * smallest);

}