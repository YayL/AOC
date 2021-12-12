#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;
#define RANGE(x, in_min, in_max, out_min, out_max) (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min

int main(){
    FILE *file = fopen("../input.txt", "r");
    if(file == NULL){
        PRINT("Error with reading file!");
        return 0;
    }

    int profileRequired = 0, verifiedProfiles = 0, num;
    char *line, *value, tempChar = '\0';
    std::string tok;
    std::cmatch match;

    while(fscanf(file, "%m[^:]:%ms", &line, &value) != EOF){
        tok = "";
        if(tempChar != '\0')
            tok = tempChar;
        tok += line;

        if(tok == "byr"){
            num = std::stoi(value);
            if(1920 <= num && num <= 2002)
                profileRequired++;
        }else if(tok == "iyr"){
            num = std::stoi(value);
            if(2010 <= num && num <= 2020)
                profileRequired++;
        }else if(tok == "eyr"){
            num = std::stoi(value);
            if(2020 <= num && num <= 2030)
                profileRequired++;
        }else if(tok == "hgt"){
            sscanf(value, "%i", &num);
            if(std::regex_match(value, std::regex(R"(^\d+cm$)")) && 150 <= num && num <= 193)
                profileRequired++;
            else if(std::regex_match(value, std::regex(R"(^\d+in$)")) && 59 <= num && num <= 76)
                profileRequired++;
        }else if(tok == "hcl"){
            if(std::regex_match(value, std::regex(R"(^#[0-9 a-f]{6}$)")))
                profileRequired++;
        }else if(tok == "ecl"){
            if(std::regex_match(value, std::regex(R"(^(amb|blu|brn|gry|grn|hzl|oth)$)")))
                profileRequired++;
        }else if(tok == "pid"){
            if(std::regex_match(value, std::regex(R"(^[0-9]{9}$)")))
                profileRequired++;
        }

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