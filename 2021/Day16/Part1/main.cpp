#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

int main(){
	std::vector<char> input;
	std::ifstream file("../input.txt", std::ios::in);
	
	std::string line;
	
	while(file >> line){
		for(char &c : line){
			input.push_back(c);
		}
	}
	file.close();

}