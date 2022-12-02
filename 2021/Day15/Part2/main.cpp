#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl
#define PRINTS(x) std::cout << x

std::vector<std::pair<int, int>> branches;
std::vector<std::vector<int>> map;
int fullWidth, fullHeight, width, height, x, y, val, factor = 5;
bool *visited;

int getValue(const int& x, const int& y){
	if(x < 0 || y < 0){
		return 0;
	}

	if(y >= map.size() || x >= map[y].size() || map[y][x] == 0){
		map[y][x] = std::max(getValue(x - width, y), getValue(x, y - height)) + 1;
		if(map[y][x] == 10)
			map[y][x] = 1;
	}
	return map[y][x];
}

void getAdjacent(int posX, int posY, std::vector<std::pair<int, int>>& ret){
	ret.clear();
    if(posX != 0 && !visited[posY * fullWidth + posX - 1])
        ret.push_back(std::make_pair(posX - 1, posY));
    if(posX != fullWidth - 1 && !visited[posY * fullWidth + posX + 1])
        ret.push_back(std::make_pair(posX + 1, posY));
    if(posY != 0 && !visited[(posY - 1) * fullWidth + posX])
        ret.push_back(std::make_pair(posX, posY - 1));
    if(posY != fullHeight - 1 && !visited[(posY + 1) * fullWidth + posX])
        ret.push_back(std::make_pair(posX, posY + 1));
}

struct MouNode{
	int cost;
	std::pair<int, int> coord;

	MouNode(const int& cost, const int& x, const int& y)
		: cost{cost}, coord{std::make_pair(x, y)} {}

	MouNode(const int& cost, const int& x, const int& y, MouNode *prev)
		: cost{cost}, coord{std::make_pair(x, y)} {}

	friend const bool operator> (const MouNode& node1, const MouNode& node2){
		return node1.cost > node2.cost;
	}
};

std::priority_queue<MouNode, std::vector<MouNode>, std::greater<MouNode>> paths;

int main(){
	std::vector<int> temp;
	std::ifstream file("../input.txt", std::ios::in);
	
	std::string line;
	
	while(file >> line){
		for(char &c : line){
			temp.push_back(c - '0');
		}
		map.push_back(temp);
		temp.clear();
	}
	file.close();

	height = map.size(), width = map[0].size(), fullHeight = factor * height, fullWidth = factor * width;
	visited = (bool*) malloc(sizeof(bool) * fullWidth * fullHeight);
	map.reserve(fullHeight);
	MouNode curr(0, 0, 0);

	paths.push(curr);
	visited[0] = true;
	
	while(paths.size() != 0){ // Check if we have reached the end of the path
		curr = MouNode(paths.top());
		getAdjacent(curr.coord.first, curr.coord.second, branches);
		if(branches.size() == 0){
			paths.pop();
		}else{
			val = curr.cost;
			paths.pop();
			for(int i = 0; i < branches.size(); i++){
				x = branches[i].first, y = branches[i].second;
				if(x >= map[y].capacity())
					map[y].reserve(fullWidth);

				visited[y * fullWidth + x] = true;
				if(branches[i].first == fullWidth - 1 && branches[i].second == fullHeight - 1){
					curr = MouNode(getValue(x, y) + val, x, y);
					goto end;
				}
				paths.push(MouNode(getValue(x, y) + val, x, y));
			}
		}
	}

	end:;

	PRINT("\nResult: " << getValue(x, y) + val);
}