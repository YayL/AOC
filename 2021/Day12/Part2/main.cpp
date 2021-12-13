#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

std::map<std::string, std::set<std::string>> nodes;
std::map<std::string, int> pathways;
int completePaths = 0;

void traverseNode(const std::set<std::string> branches, bool hasVisited2 = false){
    auto pathway = pathways;
    std::map<std::string, int>::iterator br;
    
    for(auto &branch : branches){
        br = pathways.find(branch);
        if(!pathways.empty() && hasVisited2 && br != pathways.end())
            continue;

        if(branch != "end"){
            if(!isupper(branch[0])){
                if(br == pathways.end())
                    br = pathways.emplace_hint(pathways.begin(), std::make_pair(branch, 0));
                ++br->second;
                traverseNode(nodes.find(branch)->second, hasVisited2 ? true : br->second == 2); // Change the 2 to 1 for Part1
            }else
                traverseNode(nodes.find(branch)->second, hasVisited2);
        }else{
            completePaths++;
        }
        pathways = pathway;
    }
}

int main(){

    clock_t start = clock();

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

    clock_t algoStart = clock();
    traverseNode(nodes["start"]);

    PRINT("Count: " << completePaths);
    printf("Total time: %.2fms\n", ((double)(clock() - start)/CLOCKS_PER_SEC) * 1000);
    printf("Time for DFS: %.2fms\n", ((double)(clock() - algoStart)/CLOCKS_PER_SEC) * 1000);
}
