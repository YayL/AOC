#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;
#define PRINTS(x) std::cout << x;

std::vector<std::pair<std::string, char>> polymers;

int getIndex(const std::string& element){
	for(int i = 0; i < polymers.size(); i++){
		if(polymers[i].first == element)
			return i;
	}
	return -1;
}

int main(){
	clock_t input = clock();

	std::ifstream file("../input.txt", std::ios::in);
	std::string string, line, other;
	char in, *val;
	int index[2];
	long charCounts[26] {0}, count = 1;

	file >> string;

	while(getline(file, line)){
		if(!line.empty()){
			sscanf(line.c_str(), "%ms -> %c", &val, &in);
			polymers.push_back(std::make_pair(val, in));
		}
	}
	file.close();

	const size_t size = polymers.size();
	std::vector<long> polymerCount(size, 0), copy(size, 0);

	clock_t algoStart = clock();

	for(int i = 0; i < string.length() - 1; i++){
		line = string[i], line += string[i + 1];
		for(int j = 0; j < size; j++){
			if(polymers[j].first == line)
				copy[j]++;
		}
	}

	for(int turn = 0; turn < 40; turn++){
		polymerCount = copy;
		std::fill(copy.begin(), copy.end(), 0);
		for(int i = 0; i < size; i++){
			if(polymerCount[i] == 0)
				continue;

			other = polymers[i].first[0], other += polymers[i].second;
			line = polymers[i].second, line += polymers[i].first[1];
			copy[getIndex(other)] += polymerCount[i];
			copy[getIndex(line)] += polymerCount[i];
		}
	}

	charCounts[string[0] - 'A'] = 1;

	for(int i = 0; i < copy.size(); i++){
		charCounts[polymers[i].first[1] - 'A'] += copy[i];
		count += copy[i];
	}

	long max = 0, min = count;

	for(int i = 0; i < 26; i++){
		if(charCounts[i] == 0)
			continue;

		if(charCounts[i] > max)
			max = charCounts[i];
		if(min > charCounts[i])
			min = charCounts[i];
	}

	clock_t end = clock();

	PRINT("Result: \n" << max << " | " <<  min << ": " << (max - min));
	printf("\n\nTotal time: %.3fms(Input & Setup Included)\n", (((double)end - input)/CLOCKS_PER_SEC) * 1000);
	printf("Time for calculation: %.3fms(Input & Setup Excluded)\n", (((double)end - algoStart)/CLOCKS_PER_SEC) * 1000);
}