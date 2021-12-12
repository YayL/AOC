#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;
#define PRINTS(x) std::cout << x;

std::unordered_map<std::string, std::set<std::string>> nodes;
std::stack<std::unordered_map<std::string, int>> pathways;
int completePaths;

void traverseNode(std::set<std::string> branches){
    auto pathway = pathways.top();

    for(auto &branch : branches){
        if(!pathway.empty() && pathway.find(branch) != pathway.end()){
            continue;
        }

        pathways.push(pathway);

        if(branch != "end"){
            if(!isupper(branch[0]))
                ++pathways.top()[branch];
            traverseNode(nodes[branch]);
        }else
            completePaths++;
        pathways.pop();
    }
}

int main(){
    std::vector<std::string> input;
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

    pathways.push(std::unordered_map<std::string, int>());
    traverseNode(nodes["start"]);

    PRINT("Count: " << completePaths);
}