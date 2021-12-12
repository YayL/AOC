#include <bits/stdc++.h>

#define print(x) std::cout << x << std::endl;


int main(){
    FILE *file = fopen("../input.txt", "r");
    if(file == NULL){
        print("Error with reading file!");
        return 0;
    }

    std::vector<char*> forest;
    char *line;

    while(fscanf(file, "%ms%*[^\n]", &line) != EOF){
        forest.push_back(line);
    }

    int xSlope = 3, ySlope = 1, xLen = strlen(forest[0]), yLen = forest.size(), trees = 0;

    for(int xCoord = 0, yCoord = 0; yCoord < yLen; yCoord += ySlope, xCoord = (xCoord + xSlope) % xLen){
        if(forest[yCoord][xCoord] == '#')
            trees++;
    }

    print(trees);
}