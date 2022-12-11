#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/list.c"
#include "../../includes/vector.c"

#define nextline() getline(&line, &len, fp)
#define to_num(x) ((x) - '0')

typedef struct Monkey {
	int worry;
	struct Vector * items;
	char oper;
	char * oper_num;
	int test_div;
	int test_true;
	int test_false;
	long inspected;
} Monkey;

int gcd(int, int);

int main() {
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}
	
	struct List * monkeys = init_list(sizeof(Monkey));
	Monkey * monkey;

	while((read = nextline()) != -1) {
		monkey = malloc(sizeof(Monkey));
		monkey->inspected = 0;
		monkey->items = init_vector();
		int length = (nextline() - 16) >> 2;
		for (int i = 0; i < length; ++i) {
			// int num = 10 * to_num(line[18 + (i << 2)])
			// 			 + to_num(line[19 + (1 << 2)]);
			vector_push(monkey->items, 10 * to_num(line[18 + (i << 2)])
										  + to_num(line[19 + (i << 2)]));
			//println("{2li: = }", num, vector_at(monkey->items, i));
		}
		nextline();
		sscanf(line, "%*s new = old %c %ms", 
					&monkey->oper, &monkey->oper_num);
		nextline();
		sscanf(line, "%*s divisible by %d", &monkey->test_div);
		nextline();
		sscanf(line, "%*s true: throw to monkey %d", &monkey->test_true);
		nextline();
		sscanf(line, "%*s false: throw to monkey %d", &monkey->test_false);
		nextline();
		if (line[0] == '\n') {
			list_push(monkeys, monkey);
		}
	}

	long lcm = 1;
	for (int i = 0; i < monkeys->size; ++i) {
		monkey = list_at(monkeys, i);
		if (monkey->test_div)
			lcm *= monkey->test_div;
	}

	monkey = list_at(monkeys, 0);
	int _gcd = monkey->test_div;

	for (int i = 1; i < monkeys->size; ++i) {
		monkey = list_at(monkeys, i);
		_gcd = gcd(_gcd, monkey->test_div);
	}

	lcm /= _gcd;

	for (int round = 0; round < 10000; ++round) {
		for (int i = 0; i < monkeys->size; ++i) {
			monkey = list_at(monkeys, i);
			monkey->inspected += monkey->items->size;
			for (int j = 0; monkey->items->size; ++j) {
				long long worry = vector_pop(monkey->items), second;
				if (monkey->oper_num[0] == 'o')
					second = worry;
				else
					second = atoi(monkey->oper_num);
				switch (monkey->oper) {
					case '+':
						worry += second;
						break;
					case '*':
						worry *= second;
						break;
				}
				worry %= lcm;
				int new_monk = (worry % monkey->test_div == 0)
						? monkey->test_true
						: monkey->test_false;
				monkey = list_at(monkeys, new_monk);
				vector_push(monkey->items, worry);
				monkey = list_at(monkeys, i);
			}
		}
	}

	unsigned long end = micros();
	long long product = 1, max1 = 0, max2 = 0;
	for (int i = 0; i < monkeys->size; ++i) {
		monkey = list_at(monkeys, i);
		if (monkey->inspected > max1) {
			if (max1 > max2)
				max2 = max1;
			max1 = monkey->inspected;
		} else if (monkey->inspected > max2) {
			if (max2 > max1)
				max1 = max2;
			max2 = monkey->inspected;
		}
	}
	println("{Li}", max1 * max2);
	println("Execution Time: {lu}", (end - start));

}

int gcd(int x, int y) {
	int temp;
	while (y) {
		temp = y;
		y = x%y;
		x = temp;
	}
	return x;
}
