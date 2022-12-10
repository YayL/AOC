#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/list.c"
#include "../../includes/string.c"

#define TOTAL_SPACE 70000000
#define UPDATE_SPACE 30000000

struct Folder {
	const char * name;
	struct Folder * parent;
	struct Folder * root;
	struct List * folders;
	struct List * files;
	size_t size;
};

struct File {
	const char * name;
	size_t size;
};

size_t closest_size = 0;
size_t needed_space = 0;

struct Folder * new_folder (struct Folder * parent, struct Folder * root, const char * name) {
	struct Folder * new_folder = malloc(sizeof(struct Folder));
	
	new_folder->name = name;
	new_folder->parent = parent;
	new_folder->root = root;
	new_folder->folders = init_list(sizeof(struct Folder));
	new_folder->files = init_list(sizeof(struct File));
	new_folder->size = 0;
	if (parent != NULL)
		list_push(parent->folders, new_folder);

	return new_folder;
}

void new_file (struct Folder * parent, char * name, size_t size) {
	struct File * new_file = malloc(sizeof(struct File));

	new_file->name = name;
	new_file->size = size;
	loop:
		parent->size += size;
		if (parent->parent != parent) {
			parent = parent->parent;
			goto loop;
		}
	
	list_push(parent->files, new_file);
}

void freeUpSpace(struct Folder * folder) {
	
	for (int i = 0; i < folder->folders->size; ++i) {
		freeUpSpace(list_at(folder->folders, i));
	}

	if ((needed_space <= folder->size) && folder->size < closest_size) {
		closest_size = folder->size;
	}
}

int main() {
	unsigned long start = micros();

	FILE * fp = fopen("input.txt", "r");
	char * line = NULL;
	size_t len = 0, read;

	if (fp == NULL) {
		puts("[Error]: File not found");
		exit(1);
	}

	struct Folder * root = new_folder(NULL, NULL, "/");
	root->parent = root;
	struct Folder * current = root;

	while((read = getline(&line, &len, fp)) != -1) {
		
		if (line[0] == '$') {
			char * cmd, * arg;
			sscanf(line, "$ %ms", &cmd);

			if (cmd[0] ==  'c') {
				
				sscanf(line, "$ %*s %ms", &arg);

				if (arg[0] == '.') {
					current = current->parent;	
				} else {

					struct Folder * child;
					for (int i = 0; i < current->folders->size; ++i) {
						child = list_at(current->folders, i);
						if (strcmp(arg, child->name) == 0) {
							current = child;
							break;
						}
					}

					free(arg);
				}
			}
			free(cmd);
		} else if ('0' <= line[0] && line[0] <= '9') {
			size_t size;
			char * file_name;
			sscanf(line, "%lu %ms", &size, &file_name);

			new_file(current, file_name, size);

			free(file_name);

		} else {
			char * cmd, * arg;
			sscanf(line, "%ms %ms", &cmd, &arg);
			new_folder(current, root, arg);
			free(cmd);
			free(arg);
		}

	}
	

	needed_space = UPDATE_SPACE - (TOTAL_SPACE - root->size);
	closest_size = TOTAL_SPACE;
	freeUpSpace(root);


	unsigned long end = micros();
	println("Needed: {lu}", needed_space);
	println("Closest Space: {lu}", closest_size);
	println("Execution Time: {lu}", (end - start));

}
