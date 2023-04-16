#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

int parse_int(char *data) {

	return *(int *)data;

}

char parse_char(char *data) {

	return *(char *)data;

}

char *parse_string(char *data, int len) {

	char *str;

	if (len < MIN_STR_LEN || len > MAX_STR_LEN) {
		return strdup("(invalid)");
	}

	str = malloc(len * sizeof(char));

	if (!str) {
		return strdup("(invalid)");
	}

	strcpy(str, data);

	return str;

}

void parse(char *data, size_t len) {

	unsigned char type;
	int header, tmp_int;
	size_t ptr, sz;
	char tmp_char, *tmp_string;

	if (len < 4) {
		fprintf(stderr, "Invalid data file\n");
		return;
	}

	ptr = 0;

	header = parse_int(data + ptr);
	ptr += sizeof(int);

	if (header != HEADER_MAGIC) {
		fprintf(stderr, "Invalid data file\n");
		return;
	}

	while(ptr < len) {

		type = data[ptr++];

		switch(type) {

			case TYPE_INT:

				if (len < (ptr + sizeof(int))) {
					fprintf(stderr, "Invalid entry\n");
					return;
				}

				tmp_int = parse_int(data + ptr);
				ptr += sizeof(int);
				printf("i: 0x%08x\n", tmp_int);

				break;

			case TYPE_CHAR:

				if (len < (ptr + sizeof(char))) {
					fprintf(stderr, "Invalid entry\n");
					return;
				}

				tmp_char = parse_char(data + ptr);
				ptr += sizeof(char);
				printf("c: 0x%02hhx\n", tmp_char);

				break;

			case TYPE_STRING:

				if (len < (ptr + sizeof(int))) {
					fprintf(stderr, "Invalid entry\n");
					return;
				}

				sz = parse_int(data + ptr);
				ptr += sizeof(int);

				if (sz < 0 || (ptr + sz) >= len) {
					fprintf(stderr, "Invalid entry\n");
					return;
				}

				tmp_string = parse_string(data + ptr, sz);
				printf("s: %s\n", tmp_string);
				free(tmp_string);
				ptr += sz;

				break;

			case TYPE_END:

				return;

			default:

				fprintf(stderr, "Unknown data type '%c'\n", type);

				return;

		}

	}

}
