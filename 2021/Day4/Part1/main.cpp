#include <bits/stdc++.h>

std::vector<int> split(std::string str, char delim){

    std::vector<int> result;
    std::stringstream ss (str);
    std::string token;
    
    while(getline(ss, token, delim)){
        if(token.length() != 0){
            std::cout << token << " == " << stoi(token) << std::endl;
            result.push_back(stoi(token));
        }
    }

    return result;
}

int boardsize = 5;

bool printBoard(std::vector<std::vector<int>>& board){

    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j < board[0].size(); j++){
            std::cout << board[i][j] << "|";
        }
        std::cout << std::endl;
    }

    return true;
}

void calculateResult(std::vector<std::vector<int>> & board, int lastNum){
    int sum = 0;
    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j < board[0].size(); j++){
            if(board[i][j] != -1)
                sum += board[i][j];
        }
    }
    std::cout << sum << " * " << lastNum << " = " << (sum * lastNum) << std::endl;
}

bool hasBingo(std::vector<std::vector<int>>& board){
    int rowCount, colCount;
    for(int i = 0; i < board.size(); i++){
        rowCount = 0, colCount = 0;
        for(int j = 0; j < board[0].size(); j++){
            if(board[i][j] == -1)
                rowCount++;
            if(board[j][i] == -1)
                colCount++;
        }
        if(rowCount == boardsize || colCount == boardsize){
            return true; //printBoard(board);
        }

    }
    return false;
}

int main(){
    std::ifstream file("../input.txt", std::ios::in);
    
    std::string line;
    
    file >> line;
    std::vector<int> draws = split(line, ',');
    std::vector<std::vector<std::vector<int>>> boards;
    std::vector<std::vector<int>> temp;
    int count = 0;

    while(getline(file, line)){
        if(line == "")
            continue;

        temp.push_back(split(line, ' '));
        if((++count) % boardsize == 0 && count != 0){
            std::cout << "Hey" << std::endl;
            boards.push_back(temp);
            temp.clear();
        }
    }
    std::cout << count << std::endl;

    file.close();

    for(int num = 0; num < draws.size(); num++){
        for(int i = 0; i < boards.size(); i++){
            for(int j = 0; j < boardsize; j++){
                for(int k = 0; k < boardsize; k++){
                    if(boards[i][j][k] == draws[num]){
                        boards[i][j][k] = -1;
                        if(hasBingo(boards[i])){
                            calculateResult(boards[i], draws[num]);
                            return 0;
                        }
                    }
                }
            }
        }
    }
    std::cout << "None found :(" << std::endl;

}