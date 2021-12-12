#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;
#define RANGE(x, in_min, in_max, out_min, out_max) (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

int main(){
    FILE *file = fopen("../input.txt", "r");
    if(file == NULL){
        PRINT("Error with reading file!");
        return 0;
    }

    int profileRequired = 0, verifiedProfiles = 0;
    char *line, tempChar;
    std::string tok;

    while(fscanf(file, "%m[^:]%n%*s", &line) != EOF){
        tok = "";
        if(tempChar != '\0')
            tok = tempChar;
        tok += line;

        if(tok == "byr" || tok == "iyr"|| tok == "eyr" || tok == "hgt" || tok == "hcl" || tok == "ecl" || tok == "pid")
            profileRequired++;

        if(fgetc(file) != '\n' && (memset(&tempChar, 0, 1)))
            continue;
        if((tempChar = fgetc(file)) != '\n')
            continue;
        
        tempChar = fgetc(file);
        if(profileRequired > 6)
            verifiedProfiles++;
        profileRequired = 0;
    }
    
    if(profileRequired > 6)
        verifiedProfiles++;

    free(line);

    PRINT("Final result: " << verifiedProfiles);

}