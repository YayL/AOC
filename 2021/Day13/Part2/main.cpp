
#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;
#define PRINTS(x) std::cout << x;

int main() {

	clock_t input = clock();

	std::fstream file("../input.txt");
	std::set<std::pair<int, int>> coords, new_coords;
	std::vector<std::pair<int, bool>> folds;
	std::string line;
	int first, second;
	char c;

	while(getline(file, line)){
		if (line.empty()) {
			while(getline(file, line)){
				sscanf(line.c_str(), "%*[^xy]%c=%i", &c, &second);
				folds.push_back(std::make_pair(second, c == 'y'));
			}
			break;
		}else{
			sscanf(line.c_str(), "%i,%i", &first, &second);
			coords.insert(std::make_pair(first, second));
		}
	}
	
	clock_t algoStart = clock();

	for(const auto &fold : folds){
		new_coords.clear();
		for(auto coord : coords){

			if(fold.second && coord.second > fold.first)
				coord.second = (fold.first << 1) - coord.second;
			else if(!fold.second && coord.first > fold.first)
				coord.first = (fold.first << 1) - coord.first;
			else{
				new_coords.insert(coord);
				continue;
			}

			if(new_coords.find(coord) == new_coords.end()){
				new_coords.insert(coord);
			}
		}
		
		coords = new_coords;
	}

	clock_t end = clock();

	PRINT("");
	for(int i = 0; i < 6; i++){
		for(int j = 0; j < 40; j++){
			if(coords.find({j, i}) != coords.end())
				std::cout << '#';
			else
				std::cout << '.';
		}
		PRINT("");
	}

	PRINT("Results: \n");
	printf("Total time: %.3fms(Input & Setup Included)\n", (((double)end - input)/CLOCKS_PER_SEC) * 1000);
	printf("Time for flip: %.3fms(Input & Setup Excluded)\n", (((double)end - algoStart)/CLOCKS_PER_SEC) * 1000);
}