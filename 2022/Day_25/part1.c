#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/string.c"

long long convert_to_10 (char * str) {
	long long number = 0, pow = 1;
	size_t length = strlen(str);

	for (int i = length - 1; i >= 0; --i) {
		switch (str[i]) {
			case '2':
				number += pow << 1; break;
			case '1':
				number += pow; break;
			case '0':
				break;
			case '-':
				number -= pow; break;
			case '=':
				number -= pow << 1; break;
		}
		pow *= 5;
	}
	return number;
}

const char * convert_to_5 (double number) {
	long long power = floor(log2(number) / log2(5)), num;
	String * str = new_string("");

	while (power >= 0) {
		num = pow(5, power--);
		switch ((int)round(number / num)) {
			case 2:
				number -= num << 1;
				string_append(str, '2'); break;
			case 1:
				number -= num;
				string_append(str, '1'); break;
			case 0:
				string_append(str, '0'); break;
			case -1:
				number += num;
				string_append(str, '-'); break;
			case -2:
				number += num << 1;
				string_append(str, '='); break;
		}
	}

	return str->c_str;
}

int main() {
	unsigned long __start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}
	
	long long sum = 0;

	while((read = getline(&line, &len, fp)) != -1) {
		line[read - 1] = 0;
		sum += convert_to_10(line);
	}

	println("Sum: {i}", sum);
	println("Answer: {s}", convert_to_5(sum));


	unsigned long __end = micros();
	println("Execution Time: {lu}", (__end - __start));

}
