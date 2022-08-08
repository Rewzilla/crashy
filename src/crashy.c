#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define VERSION 1.00
#define HEADER_MAGIC 0xdec001c0
#define MIN_STR_LEN 1
#define MAX_STR_LEN 32

enum type {
	TYPE_INT	= 0x01,
	TYPE_CHAR	= 0x02,
	TYPE_STRING	= 0x03,
	TYPE_END	= 0xff,
};

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

void parse(char *data, int len) {

	unsigned char type;
	int header, sz, ptr, tmp_int;
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

				if (sz < 0 || len < (ptr + sz)) {
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
int file_size(char *path) {

	struct stat st;
	stat(path, &st);
	return st.st_size;

}

int main(int argc, char *argv[]) {

	int fd;
	int len;
	char *data;

	if(argc != 2) {
		fprintf(stderr, "Crashy (version %.02f)\n", VERSION);
		fprintf(stderr, "Usage: %s <inputfile>\n", argv[0]);
		return 1;
	}

	fd = open(argv[1], O_RDONLY);
	if(fd < 0) {
		fprintf(stderr, "Failed to open '%s'\n", argv[1]);
		return 1;
	}

	len = file_size(argv[1]);

	data = malloc(len);
	if(!data) {
		fprintf(stderr, "Failed to allocate %d bytes of memory\n", len);
		return 1;
	}

	read(fd, data, len * sizeof(char));

	parse(data, len);

	close(fd);

	return 0;

}
