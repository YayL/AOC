#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

long wins1 = 0, wins2 = 0;
int temp;

std::map<std::string, std::pair<long, long>> checked;
std::string key;

std::pair<long, long> countWins(long player1, long player2, long score1, long score2){
    if(score1 >= 21)
        return {1, 0};
    if(score2 >= 21)
        return {0, 1};

    key = std::to_string(player1) + std::to_string(player2) + std::to_string(score1) + std::to_string(score2);
    if(checked.find(key) != checked.end())
        return checked[key];

    std::pair<long, long> answer;

    for(int d1 = 1; d1 <= 3; ++d1){
        for(int d2 = 1; d2 <= 3; ++d2){
            for(int d3 = 1; d3 <= 3; ++d3){
                temp = (player1 + d1 + d2 + d3) % 10;
                auto res = countWins(player2, temp, score2, score1 + temp + 1);
                answer.first += res.second, answer.second += res.first;
            }
        }
    }
    checked[key] = answer;
    return answer;
}

int main(){
    
    std::pair<long, long> res = countWins(7-1, 2-1, 0, 0);

    PRINT(res.first);
    PRINT(res.second);

}