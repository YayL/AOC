#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

bool match(const char c1,const char c2) {
    if (c1 == '(') return c2 == ')';
    else if (c1 == '[') return c2 == ']';
    else if (c1 == '{') return c2 == '}';
    else if (c1 == '<') return c2 == '>';
    return false;
};

char get_match(const char c) {
    if (c == '(') return ')';
    else if (c == '[') return ']';
    else if (c == '{') return '}';
    else if (c == '<') return '>';
    return ' ';
};

int lookup_score(const char c) {
    if (c == ')') return 1;
    else if (c == ']') return 2;
    else if (c == '}') return 3;
    else if (c == '>') return 4;
    else return 0;
};


int main(int argc, char * argv[]) {
    std::string line;
    std::ifstream file("../input.txt");

    std::vector <long> vals;

    while (file >> line) {
        std::stack < char > s;
        bool corrupt = false;
        for (const auto c: line) {
            if (c == '(' || c == '[' || c == '{' || c == '<')
                s.push(c);
            else {
                if (!match(s.top(), c)) {
                    corrupt = true;
                    break;
                } else {
                    s.pop();
                }
            }
        }
        if (!corrupt) {
            long value = 0;
            while (!s.empty()) {
                const char c = s.top();
                s.pop();
                value *= 5;
                value += lookup_score(get_match(c));
            }
            vals.push_back(value);
        }
    }

    std::sort(std::begin(vals), std::end(vals));

    for (int i = 0; i < vals.size(); i++) {
        PRINT(i << ": " << vals[i]);
    }

    PRINT(vals[vals.size() >> 2]);
}