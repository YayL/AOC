#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

std::vector<std::pair<std::string, std::vector<std::pair<std::string, int>>>> bags;
std::vector<int> goldHolders;

void printBags(){
    for(int i = 0; i < bags.size(); i++){
        std::cout << bags[i].first << ":";
        for(auto bag : bags[i].second){
            std::cout << " " << bag.first << " x" << bag.second;
        }
        PRINT(" can hold " << goldHolders[i] << " gold");
    }
}

int main(){
    std::ifstream file("../input.txt", std::ios::in);
    std::string line, color;

    std::regex currentBag(R"(^\w+\s\w+)"), comingBags(R"(\d (\w+\s\w+))");
    std::smatch matches;
    std::vector<std::pair<std::string, int>> temp;
    int count, index = 0;

    while(getline(file, line)){
        auto begin = std::sregex_iterator(line.begin(), line.end(), comingBags), end = std::sregex_iterator();

        if(!std::regex_search(line, matches, currentBag))
            PRINT("No matches found! " << line);
        color = matches[0].str();

        if(begin != end){
            temp.clear();
            for(std::sregex_iterator i = begin; i != end; ++i){
                line = (*i).str();
                count = 0, index = 0;
                while(isdigit(line[index++])){
                    count *= 10;
                    count += line[index - 1] - '0';
                }
                temp.push_back(std::make_pair((*i).str().erase(0, index), count));
            }
        }else{
            temp.clear();
        }
        bags.push_back(std::make_pair(color, temp));
        goldHolders.push_back(0);
    }
    file.close();

    for(int i = 0; i < bags.size(); i++){
        auto bag = bags[i].second;
        for(int inside = 0; inside < bag.size(); inside++){
            if(bag[inside].first == "shiny gold"){
                goldHolders[i] = bag[inside].second;
                bag.erase(bag.begin() + inside);
                break;
            }
        }
    }

    printBags();

    int changes = -1;
    while(changes != 0){
        changes = 0;
        //PRINT(1);
        for(int i = 0; i < bags.size(); i++){
            //PRINT(2);
            auto bag = bags[i].second;
            for(int inside = 0; inside < bag.size(); inside++){
                //PRINT(3);
                for(int otherBags = 0; otherBags < bags.size(); otherBags++){
                    if(bags[otherBags].second.size() == 0 && bag[inside].first == bags[otherBags].first){
                        PRINT(5);
                        goldHolders[i] += goldHolders[otherBags];
                        bag.erase(bag.begin() + inside);
                        changes++;
                        break;
                    }
                }
            }
        }
    }

    PRINT(goldHolders.size());
    printBags();

}