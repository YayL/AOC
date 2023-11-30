#include <stdio.h>
#include <stdlib.h>

#include "../../includes/fmt.c"
#include "../../includes/timer.c"
#include "../../includes/list.c"
#include "../../includes/string.c"

#define MAX_SIZE 100000

typedef struct folder {
	String * name;
	struct Folder * parent;
	struct Folder * root;
	struct List * folders;
	struct List * files;
	size_t size;
} Folder;

typedef struct file {
	String * name;
	size_t size;
} File;

Folder * new_folder (Folder * parent, Folder * root, const char * name) {
	Folder * new_folder = malloc(sizeof(Folder));
	
	new_folder->name = new_string(name);
	new_folder->parent = parent;
	new_folder->root = root;
	new_folder->folders = init_list(sizeof(Folder));
	new_folder->files = init_list(sizeof(File));
	new_folder->size = 0;
	if (parent != NULL)
		list_push(parent->folders, new_folder);

	return new_folder;
}

void new_file (Folder * parent, char * name, size_t size) {
	File * new_file = malloc(sizeof(File));

	new_file->name = new_string(name);
	new_file->size = size;
	loop:
		parent->size += size;
		if (parent->parent != parent) {
			parent = parent->parent;
			goto loop;
		}
	
	list_push(parent->files, new_file);
}

size_t getFolderSize(Folder * folder) {
	
	size_t size = (folder->size <= MAX_SIZE ? folder->size : 0);

	for (int i = 0; i < folder->folders->size; ++i) {
		size += getFolderSize(list_at(folder->folders, i));
	}
		
	return size;
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

	Folder * root = new_folder(NULL, NULL, "/");
	root->parent = root;
	Folder * current = root;

	while((read = getline(&line, &len, fp)) != -1) {
		
		if (line[0] == '$') {
			char * cmd, * arg;
			sscanf(line, "$ %ms", &cmd);

			if (strcmp(cmd, "cd") == 0) {
				
				sscanf(line, "$ %*s %ms", &arg);

				if (strcmp(arg, "..") == 0) {
					current = current->parent;	
				} else {

					Folder * child;
					for (int i = 0; i < current->folders->size; ++i) {
						child = list_at(current->folders, i);
						if (strcmp(arg, child->name->c_str) == 0) {
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

	size_t sum = getFolderSize(root) + (root->size <= MAX_SIZE ? root->size : 0);


	unsigned long end = micros();

	println("Total Size: {lu}", sum);
	println("Execution Time: {lu}", (end - start));

}
