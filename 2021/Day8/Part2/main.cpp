#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

std::array<std::bitset<7>, 10> pattern_list;
std::array<std::bitset<7>, 10> digits;
std::array<u_char, 1 << 7> map;

void setPattern(std::bitset<7> pattern, u_char digit) {
    digits[digit] = pattern;
    map[pattern.to_ullong()] = digit;
}

uint SolveLine(std::istream& input) {
    std::string pattern_str;
    uint num = 0;

    pattern_list.fill(0);
    digits.fill(0);
    map.fill(0);

    for (auto& pattern : pattern_list) {
        input >> pattern_str;
        for (char c : pattern_str) 
            pattern[c - 'a'] = true;
    }

    for (auto pattern : pattern_list) {
        switch (pattern.count()) {
            case 2: setPattern(pattern, 1); break;
            case 3: setPattern(pattern, 7); break;
            case 4: setPattern(pattern, 4); break;
            case 7: setPattern(pattern, 8); break;
        }
    }
    
    for (auto pattern : pattern_list) {
        switch (pattern.count()) {
            case 5: // 2, 3, 5
                if ((pattern & digits[1]) == digits[1])
                    setPattern(pattern, 3);
                else if ((pattern | digits[4]) == digits[8])
                    setPattern(pattern, 2);
                else
                    setPattern(pattern, 5);
                break;
            case 6: // 0, 6, 9
                if ((pattern & digits[4]) == digits[4])
                    setPattern(pattern, 9);
                else if ((pattern & digits[1]) == digits[1])
                    setPattern(pattern, 0);
                else
                    setPattern(pattern, 6);
                break;
        }
    }
    input.ignore(3);

    for (int i = 0; i < 4; ++i) {
        input >> pattern_str;
        std::bitset<7> pattern;

        for (char c : pattern_str) 
            pattern[c - 'a'] = true;

        num *= 10;
        num += map[pattern.to_ullong()];
    }
    return num;
}

int main() {
    std::ifstream input("../input.txt", std::ios::in);
    unsigned easy_digit_sum = 0;
    unsigned sum = 0;

    while (!input.eof()) {
        sum += SolveLine(input);
    }

    PRINT("Result: " << sum);
}