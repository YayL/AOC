#include "includes/fmt.c"
#include "includes/vector.c"

void print_vector(struct Vector * vector) {
	println("Vector: ({2i:, })", vector->size, vector->capacity);
	for (size_t i = 0; i < vector->size; ++i) {
		printf("%lu) %ld\n", i, vector->items[i]);
	}
}

int main() {
	
	struct Vector * vector = init_vector();
	
	vector_reserve(vector, 2);

	vector_push(vector, 2);
	vector_push(vector, -1);
	print_vector(vector);

	struct Vector * copy = vector_copy(vector);
	vector_push(copy, 100);
	print_vector(copy);
	puts("\n");
	print_vector(vector);

} 
