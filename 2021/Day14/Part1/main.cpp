#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;
#define PRINTS(x) std::cout << x;

std::vector<std::pair<std::pair<char, char>, char>> polymers;

int main(){
	clock_t input = clock();

	std::vector<char> string;
	std::ifstream file("../input.txt", std::ios::in);
	std::string line;
	char in[3];
	bool insert = false;

	file >> line;
	for(auto &c : line){
		string.push_back(c);
	}

	while(getline(file, line)){
		if(!line.empty()){
			sscanf(line.c_str(), "%c%c -> %c", &in[0], &in[1], &in[2]);
			polymers.push_back(std::make_pair(std::make_pair(in[0], in[1]), in[2]));
		}
	}
	file.close();

	clock_t algoStart = clock();

	for(long step = 0; step < 10; step++){
		for(long i = 1; i < string.size(); i++){
			for(const auto &polymer : polymers){
				if(string[i] == polymer.first.second && string[i - 1] == polymer.first.first){
					string.insert(string.begin() + i++, polymer.second);
					break;
				}
			}
		}

	}

	std::unordered_map<char, long> count;

	for(auto &val : string){
		++count[val];
	}

	long max = 0, min = string.size();

	for(auto &iter : count){
		if(iter.second > max)
			max = iter.second;
		if(min > iter.second)
			min = iter.second;
	}

	clock_t end = clock();

	PRINT("Result: \n" << max << " | " <<  min << ": " << (max - min));
	printf("\n\nTotal time: %.3fms(Input & Setup Included)\n", (((double)end - input)/CLOCKS_PER_SEC) * 1000);
	printf("Time for calculation: %.3fms(Input & Setup Excluded)\n", (((double)end - algoStart)/CLOCKS_PER_SEC) * 1000);

	
}