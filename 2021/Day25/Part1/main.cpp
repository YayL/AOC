#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;
#define PRINTS(x) std::cout << x;

size_t width, height;
std::vector<std::vector<char>> board;
std::bitset<20000> checked;

void printBoard(){
    PRINT("\n");
    for(int row = 0; row < height; ++row){
        for(int col = 0; col < width; ++col){
            PRINTS(board[row][col]);
        }
        PRINT("");
    }
}

bool moveBoard(){
    bool ret = false;
    checked.reset();
    for(int row = 0; row < height; ++row){
        for(int col = 0; col < width; ++col){
            if(!checked[row * width + col] && !checked[row * width + ((col + 1) % width)] && board[row][col] == '>' && board[row][(col + 1) % width] == '.'){
                board[row][(col + 1) % width] = '>';
                board[row][col] = '.';
                checked[row * width + col] = true;
                checked[row * width + ((++col) % width)] = true;
                ret = true;
            }
        }
    }
    checked.reset();
    for(int col = 0; col < width; ++col){
        for(int row = 0; row < height; ++row){
            if(!checked[row * width + col] && !checked[((row + 1) % height) * width + col] && board[row][col] == 'v' && board[(row + 1) % height][col] == '.'){
                board[(row + 1) % height][col] = 'v';
                board[row][col] = '.';
                checked[row * width + col] = true;
                checked[((++row) % height) * width + col] = true;
                ret = true;
            }
        }
    }
    return ret;
}

int main(){
    std::vector<char> row;
    std::ifstream file("../input.txt", std::ios::in);
    
    std::string line;
    
    while(file >> line){
        row.clear();
        for(int i = 0; i < line.length(); ++i){
            row.push_back(line[i]);
        }
        board.push_back(row);
    }
    width = row.size(), height = board.size();
    file.close();

    int count = 0;
    while(moveBoard()){
        count++;
    }
    PRINT("Result: " << (count + 1));

}