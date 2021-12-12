#include <bits/stdc++.h>

#define print(x) std::cout << x << std::endl;

std::vector<char*> forest;

long checkSlope(int xSlope, int ySlope){
    int xLen = strlen(forest[0]), yLen = forest.size();
    long trees = 0;

    for(int xCoord = 0, yCoord = 0; yCoord < yLen; yCoord += ySlope, xCoord = (xCoord + xSlope) % xLen){
        print(forest[yCoord][xCoord]);
        if(forest[yCoord][xCoord] == '#')
            trees++;
    }
    return trees;
}

int main(){
    FILE *file = fopen("../input.txt", "r");
    if(file == NULL){
        print("Error with reading file!");
        return 0;
    }

    char *line;

    while(fscanf(file, "%ms%*[^\n]", &line) != EOF){
        forest.push_back(line);
    }

    print((checkSlope(1, 1) * checkSlope(3, 1) * checkSlope(5, 1) * checkSlope(7, 1) * checkSlope(1, 2)));

}