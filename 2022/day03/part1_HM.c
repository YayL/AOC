#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../includes/fmt.c"
#include "../../includes/hashmap.c"
#include "../../includes/timer.c"

#define GET_INDEX(x) (((x) <= 'Z') ? ((x) - 'A' + 27) : ((x) - 'a' + 1))

int main() {
	
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("File not found");
		exit(1);
	}
	
	int sum = 0;
	size_t list_size = 53;
	HashMap * map;

	while((read = getline(&line, &len, fp)) != -1) {
		const int _len = ((read - 1) >> 1);
		map = new_HashMap(64);
		for (int i = 0; i < _len; ++i) {
			char * str = format("{c}", line[i]);
			HM_set(map, str, 1);
			free(str);
		}
		
		for (int i = _len; i < read; ++i) {
			char * str = format("{c}", line[i]); 
			if (HM_has(map, str)) {
				sum += GET_INDEX(line[i]);
				HM_remove(map, str);
			}
			free(str);
		}
		HM_free(map);
	}
	
	unsigned long end = micros();

	println("\nSum: {i}", sum);
	println("\n----------\nExecution Time: {lu}", (end - start));

}
