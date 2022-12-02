#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

std::vector<std::pair<int, int>> players;
int dice = 0;

void rollDice(int player){
    int pos = 0;
    for(int i = 0; i < 3; i++){
        dice = (dice % 100) + 1;
        pos += dice;
    }
    pos = ((pos + players[player].second - 1) % 10) + 1;
    players[player].first += pos, players[player].second = pos;
}

int main(){
    std::vector<std::string> input;
    std::ifstream file("../input.txt", std::ios::in);
    
    std::string line;
    int num, count = 0, lastPlayer;
    
    while(getline(file, line)){
        if(!line.empty()){
            sscanf(line.c_str(), "%*[^:]: %d", &num);
            players.push_back({0, num});
        }
    }
    file.close();

    num = players.size();

    for(int i = 0; i < num; i = ++i % num){
        rollDice(i);
        lastPlayer = i;
        count += 3;
        if(players[i].first >= 1000)
            break;
    }

    num = players[0].first;
    for(int i = 1; i < players.size(); ++i){
        if(players[i].first < num)
            num = players[i].first;
    }
    
    PRINT("Min: " << num);
    PRINT("P1: " << players[0].first);
    PRINT("P2: " << players[1].first);
    PRINT(count);

    PRINT("Result: " << (num * count));

}