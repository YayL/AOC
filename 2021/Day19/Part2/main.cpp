#include <bits/stdc++.h>

#define PRINT(x) std::cout << x << std::endl;

struct Node {
    bool isnum;
    int num;
};

typedef std::vector<Node> Num;

std::string ToString(const Num& num){
    std::string ret;
    for(auto& x : num){
        if(x.isnum)
            ret += x.num;
        else
            ret += (char)x.num;
    }

    return ret;
}

void Add(Num& num, const Num& num2){
    num.insert(num.begin(), { false, '[' });
    num.push_back({ false, ',' });
    for(auto& n : num2){
        num.push_back(n);
    }
    num.push_back({ false, ']' });
}

bool ExplodeOne(Num& num){
    // Find bracket inside 4 pairs
    int bracketcount = 0;
    for(auto it = num.begin(); it != num.end(); ++it){
        if(!it->isnum){
            if(it->num == '['){
                if(++bracketcount > 4){
                    if((it + 1)->isnum &&
                        !it[2].isnum && it[2].num == ',' &&
                        it[3].isnum &&
                        !it[4].isnum && it[4].num == ']'){

                        // Found a regular pair inside 4 brackets
                        int n1 = it[1].num;
                        int n2 = it[3].num;

                        // Scan backwards
                        for (auto it2 = it; it2 != num.begin(); --it2) {
                            if (it2->isnum) {
                                it2->num += n1;
                                break;
                            }
                        }

                        // Scan forwards
                        for (auto it2 = it+5; it2 != num.end(); ++it2) {
                            if (it2->isnum) {
                                it2->num += n2;
                                break;
                            }
                        }

                        // Erase regular pair (leave last paren for replace)
                        it = num.erase(it, it + 4);

                        it->isnum = true;
                        it->num = 0;
                        return true;
                    }
                }
            }
            else if (it->num == ']') {
                --bracketcount;
            }
        }
    }

    return false;
}

bool SplitOne(Num& num){
    for(auto it = num.begin(); it != num.end(); ++it){
        if(it->isnum && it->num > 9){
            int n = it->num;
            it = num.erase(it);
            num.insert(it, { {false, '['}, {true, n / 2}, {false, ','}, {true, n / 2 + n % 2}, {false, ']'} });
            return true;
        }
    }
    return false;
}

void Reduce(Num& num){
//    cout << ToString(num) << endl;
    while(true){
        if(!ExplodeOne(num)){
            if(!SplitOne(num)){
                break;
            }
        }
//        cout << ToString(num) << endl;
    }
}

int Magnitude(Num num){
    bool loop = true;
    while(loop && num.size() > 4){
        loop = false;
        for(auto it = num.begin(); it+4 != num.end(); ++it){
            if(!it->isnum && it->num == '[' &&
                (it + 1)->isnum &&
                !(it + 2)->isnum && (it + 2)->num == ',' &&
                (it + 3)->isnum &&
                !(it + 4)->isnum && (it + 4)->num == ']'){

                // Got a pair, collapse it
                int n1 = (it + 1)->num;
                int n2 = (it + 3)->num;
                it = num.erase(it, it + 4);
                it->isnum = true;
                it->num = 3 * n1 + 2 * n2;

                loop = true;
                break;
            }
        }
    }

    if(num.size() != 1)
        return 0;
    return num[0].num;
}

int SumAll(const std::vector<Num>& nums){
    Num acc;
    for (auto num : nums) {
        if (!acc.empty()) {
            Add(acc, num);
            Reduce(acc);
        }
        else {
            swap(acc, num);
        }
    }

    return Magnitude(acc);
}

int main()
{
    std::vector<Num> nums;
    std::ifstream file("../input.txt", std::ios::in);

    std::string line;
    while (getline(file, line)) {
        Num num;
        for (char x : line) {
            Node n;
            if (isdigit(x)) {
                n.num = x - '0';
                n.isnum = true;
            }
            else {
                n.num = x;
                n.isnum = false;
            }

            num.push_back(n);
        }

        nums.push_back(num);
    }


    int result = 0;
    for (int i = 0; i < nums.size(); ++i) {
        for (int j = 0; j < nums.size(); ++j) {
            if (i != j) 
            	result = std::max(result, SumAll({ nums[i], nums[j] }));
        }
    }

    PRINT("Result: " << result);
}