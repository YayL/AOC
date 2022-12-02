#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl
#define PRINTS(x) std::cout << x

std::vector<std::vector<int>> map;
int width, height, x, y, val;
bool *visited;

struct MouNode{
	int cost;
	std::pair<int, int> coord;
	std::bitset<10000> path;

	MouNode(const int& cost, const int& x, const int& y)
		: cost{cost}, coord{std::make_pair(x, y)}
		{
			path[0] = true;
		}

	MouNode(const int& cost, const int& x, const int& y, MouNode *prev)
		: cost{cost}, coord{std::make_pair(x, y)}, path{prev->path} {
			path[y * width + x] = true;
		}

	friend const bool operator> (const MouNode& node1, const MouNode& node2){
		return node1.cost > node2.cost;
	};
	const void print() const;
};

const void MouNode::print() const{
	for(int row = 0; row <= this->coord.second; ++row){
		for(int col = 0; col <= this->coord.first; ++col){
			if(this->path[row * width + col]){
				printf("\033[1;7;32m%i\033[0m", map[row][col]);
			}else{
				printf("\033[7;31m%i\033[0m", map[row][col]);
			}
		}
		PRINT("");
	}
}

std::priority_queue<MouNode, std::vector<MouNode>, std::greater<MouNode>> paths;
std::vector<std::pair<int, int>> branches;

void getAdjacent(int posX, int posY, std::vector<std::pair<int, int>>& ret){
	ret.clear();
    if(posX != 0 && !visited[posY * width + posX - 1])
        ret.push_back(std::make_pair(posX - 1, posY));
    if(posX != width - 1 && !visited[posY * width + posX + 1])
        ret.push_back(std::make_pair(posX + 1, posY));
    if(posY != 0 && !visited[(posY - 1) * width + posX])
        ret.push_back(std::make_pair(posX, posY - 1));
    if(posY != height - 1 && !visited[(posY + 1) * width + posX])
        ret.push_back(std::make_pair(posX, posY + 1));
}

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

	height = map.size(), width = map[0].size();
	visited = (bool*) malloc(sizeof(bool) * width * height);

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
				visited[y * width + x] = true;
				if(branches[i].first == width - 1 && branches[i].second == height - 1){
					curr = MouNode(map[y][x] + val, x, y, &curr);
					goto end;
				}
				//printf("(%i, %i) == %i\n", x, y, val + map[y][x]);
				paths.push(MouNode(map[y][x] + val, x, y, &curr));
			}
		}
	}

	end:;

	PRINT("\nResult: " << map[y][x] + val);
	curr.print();

	// 4ms without keeping track of the taken path
}