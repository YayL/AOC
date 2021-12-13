
#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;
#define PRINTS(x) std::cout << x;

int main() {

	clock_t setup = clock();

	std::fstream file("../input.txt");

	std::set<std::pair<int, int>> coords, new_coords;
	std::string line;
	std::vector<std::pair<int, bool>> folds;
	int s[2];
	char c;

	while(getline(file, line)){
		if (line.empty()) {
			while(getline(file, line)){
				sscanf(line.c_str(), "%*[^xy]%c=%i", &c, &s[1]);
				folds.push_back(std::make_pair(s[1], c == 'y'));
			}
		}else{
			sscanf(line.c_str(), "%i,%i", &s[0], &s[1]);
			coords.insert(std::make_pair(s[0], s[1]));
		}
	}

	clock_t algoStart = clock();
	
	for(auto coord : coords){
		if(folds[0].second && coord.second > folds[0].first){
			coord.second = (folds[0].first << 1) - coord.second;
		}else if(!folds[0].second && coord.first > folds[0].first){
			coord.first = (folds[0].first << 1) - coord.first;
		}else{
			new_coords.insert(coord);
			continue;
		}

		if(new_coords.find(coord) == new_coords.end()){
			new_coords.insert(coord);
		}
	}

	clock_t end = clock();

	printf("Total time: %.3fms (Input & Setup Included)\n", (((double)end - setup)/CLOCKS_PER_SEC) * 1000);
	printf("Time for flip: %.3fms (Input & Setup Excluded)\n", (((double)end - algoStart)/CLOCKS_PER_SEC) * 1000);

	PRINT(new_coords.size());
}