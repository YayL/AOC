#include <bits/stdc++.h>

#define print(str) std::cout << str << std::endl;

int main(){
    FILE *file;

    file = fopen("../input.txt", "r");
    if(file == NULL){
        printf("Error with reading file");
        return 0;
    }

    int count = 0, *bounds = (int*) calloc(2, sizeof(int)), len = 0;
    char c, *token;

    while(fscanf(file, "%i-%i %c: %ms%*[^\n]", &bounds[0], &bounds[1], &c, &token) != EOF){
        if((token[bounds[0] - 1] == c) ^ (token[bounds[1] - 1] == c)){
            count++;
        }
    }
    print(count);
    
}