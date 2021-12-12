#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

std::unordered_map<std::string, std::set<std::string>> nodes;
int completePaths = 0;

void traverseNode(std::unordered_map<std::string, int> pathway, std::set<std::string> branches, bool hasVisited2 = false){
    bool found;
    auto copy = pathway;

    for(auto &branch : branches){
        if(!pathway.empty() && hasVisited2 && pathway.find(branch) != pathway.end())
            continue;

        if(branch != "end"){
            if(!isupper(branch[0])){
                ++pathway[branch];
                traverseNode(pathway, nodes[branch], hasVisited2 ? true : pathway[branch] == 2); // Change the 2 to 1 for Part1
            }else
                traverseNode(pathway, nodes[branch], hasVisited2);
        }else{
            completePaths++;
        }
        pathway = copy;
    }
}

int main(){
    std::ifstream file("../input.txt", std::ios::in);
    std::string line;
    
    while(file >> line){
        std::string temp[2];
        std::stringstream ss (line);
        std::string token;
        
        for(int i = 0; getline(ss, token, '-'); i++){
            if(token.length() != 0)
                temp[i] = token;
        }
        if(temp[1] != "start")
            nodes[temp[0]].insert(temp[1]);
        if(temp[0] != "start")
            nodes[temp[1]].insert(temp[0]);
    }
    file.close();

    traverseNode(std::unordered_map<std::string, int>(), nodes["start"]);

    PRINT("Count: " << completePaths);
}