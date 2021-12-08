#include <bits/stdc++.h>

#define print(x) std::cout << x << std::endl

int main(){

    FILE *file;

    file = fopen("../input.txt", "r");
    if(file == NULL){
        print("An error with opening file!");
        return 0;
    }

    std::vector<int> heights;
    int num, max = 0;

    while(fscanf(file, "%i", &num) != EOF && fgetc(file)){
        if(num > max)
            max = num;
        heights.push_back(num);   
    }

    std::vector<int> fuels;
    int temp, x;

    for(int i = 0; i < max; i++){
        temp = 0;
        for(int j = 0; j < heights.size(); j++){
            x = std::abs(heights[j] - i);
            temp += (x * (x + 1)) >> 1;
        }
        fuels.push_back(temp);
    }

    temp = fuels[0];

    for(int i = 0; i < fuels.size(); i++){
        if(fuels[i] < temp)
            temp = fuels[i];
    }

    print("Result: " << temp);

}