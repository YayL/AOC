#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;
#define PRINTS(x) std::cout << x;

std::vector<bool> map, temp;
std::vector<std::vector<bool>> image, copy;
bool isInfinitelyOnes = false;

void printImage(){
    for(int y = 0; y < image.size(); ++y){
        for(int x = 0; x < image[y].size(); ++x){
            PRINTS((image[y][x] ? '#' : '.'));
        }
        PRINT("");
    }
    PRINT("\n");
}

void addEmptyRow(bool front){
    if(front){
        image.insert(image.begin(), std::vector<bool>(image[0].size(), false));
        copy.insert(copy.begin(), image[0]);
    }else{
        image.push_back(std::vector<bool>(image[0].size(), false));
        copy.push_back(image.back());
    }
}

void addEmptyCol(bool front){
    if(front){
        for(int i = 0; i < image.size(); i++){
            image[i].insert(image[i].begin(), false);
            copy[i].insert(copy[i].begin(), false);
        }
    }else{
        for(int i = 0; i < image.size(); i++){
            image[i].push_back(false);
            copy[i].push_back(false);
        }
    }
}

int getIndexFromSquare(int x, int y){
    int index = 0, tempX = 0, tempY = 0;
    x--, y--;
    for(int yOffset = 0; yOffset < 3; ++yOffset){
        for(int xOffset = 0; xOffset < 3; ++xOffset){
            index <<= 1;
            tempX = x + xOffset, tempY = y + yOffset;
            if(1 <= tempY && tempY < copy.size() - 1 && 1 <= tempX && tempX < copy[tempY].size() - 1){
                index += copy[tempY][tempX];
            }else if(isInfinitelyOnes){
                index += 1;
            }
        }
    }
    return index;
}


int main(){

    clock_t input = clock();

    std::ifstream file("../input.txt", std::ios::in);
    std::string line;

    file >> line;
    
    for(auto &c : line){
        map.push_back(c == '#');
    }

    while(getline(file, line)){
        if(!line.empty()){
            for(auto &c : line){
                temp.push_back(c == '#');
            }
            image.push_back(temp);
            temp.clear();
        }
    }
    file.close();
    copy = image;

    clock_t algoStart = clock();

    for(int iteration = 0; iteration < 1000; ++iteration){
        isInfinitelyOnes = map[0] ? (iteration % 2) : false;
        addEmptyCol(true), addEmptyCol(false);
        addEmptyRow(true), addEmptyRow(false);
        for(int y = 0; y < image.size(); ++y){
            for(int x = 0; x < image[y].size(); ++x){
                image[y][x] = map[getIndexFromSquare(x, y)];
            }
        }
        copy = image;
    }

    int count = 0;

    for(int y = 0; y < image.size(); ++y){
        for(int x = 0; x < image[0].size(); ++x){
            if(image[y][x])
                count++;
        }
    }

    clock_t end = clock();

    PRINT("Result: " << count);
    printf("\n\nTotal time: %.3fms(Input & Setup Included)\n", (((double)end - input)/CLOCKS_PER_SEC) * 1000);
	printf("Time for calculation: %.3fms(Input & Setup Excluded)\n", (((double)end - algoStart)/CLOCKS_PER_SEC) * 1000);
    
}