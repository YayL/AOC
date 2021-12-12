#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

std::vector<std::vector<int>> heightMap;

void getAdjacent(int posX, int posY, std::vector<int>& ret){
    if(posX != 0)
        ret.push_back(heightMap[posY][posX - 1]);
        
    if(posX != heightMap[0].size() - 1)
        ret.push_back(heightMap[posY][posX + 1]);

    if(posY != 0)
        ret.push_back(heightMap[posY - 1][posX]);

    if(posY != heightMap.size() - 1)
        ret.push_back(heightMap[posY + 1][posX]);
}

int getRiskLevel(int posX, int posY){
    std::vector<int> sides;
    bool isSmaller = true;
    getAdjacent(posX, posY, sides);

    for(int i = 0; i < sides.size(); i++){
        if(isSmaller && sides[i] <= heightMap[posY][posX])
            isSmaller = false;
    }
    return isSmaller ? heightMap[posY][posX] + 1 : 0;
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
    file.close();

    PRINT("width: " << heightMap[0].size());
    PRINT("height: " << heightMap.size());

    int total = 0;

    for(int y = 0; y < heightMap.size(); y++){
        for(int x = 0; x < heightMap[y].size(); x++){
            total += getRiskLevel(x, y);
        }
    }

    PRINT(total);

}