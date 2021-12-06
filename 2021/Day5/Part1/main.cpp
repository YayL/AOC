#include <bits/stdc++.h>

int main(){

    std::vector<int*> input;
    std::ifstream file("../input.txt", std::ios::in);
    
    std::string line;
    int count;
    
    while(file >> line){
        std::stringstream ss (line);
        
        while(getline(ss, line, ' ')){
            if(line.length() != 0 && line != "->"){
                std::stringstream temp(line);
                count = 0;
                int* coords = (int*) malloc(sizeof(int) * 4);
                while(getline(temp, line, ','))
                    coords[count++] = stoi(line);
                input.push_back(coords);
            }
        }
    }
    file.close();
    int map[990][990];
    memset(map, 0, sizeof(map[0][0]) * 990 * 990);
    int *max = (int*) calloc(2, sizeof(int)), *min = (int*) calloc(2, sizeof(int));
    count = 0;

    for(int i = 0; i < input.size(); i += 2){
        max[0] = input[i][0] >= input[i + 1][0] ? input[i][0] : input[i + 1][0], min[0] = input[i][0] == max[0] ? input[i + 1][0] : input[i][0];
        max[1] = input[i][1] >= input[i + 1][1] ? input[i][1] : input[i + 1][1], min[1] = input[i][1] == max[1] ? input[i + 1][1] : input[i][1];

        if(max[0] != min[0] && max[1] == min[1]){
            for(int x = min[0]; x <= max[0]; x++){
                if(++map[x][max[1]] == 2)
                    count++;
            }   
        }else if(max[1] != min[1] && max[0] == min[0]){
            for(int y = min[1]; y <= max[1]; y++){
                if(++map[max[0]][y] == 2)
                    count++;
            } 
        }
        free(input[i]);
    }
    free(max); free(min);

    std::cout << count << std::endl;
}