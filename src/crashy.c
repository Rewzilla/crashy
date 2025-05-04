#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "parse.h"

size_t file_size(char *path) {

	struct stat st;
	stat(path, &st);
	return st.st_size;

}

int main(int argc, char *argv[]) {

	int fd;
	size_t len;
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
		fprintf(stderr, "Failed to allocate %zu bytes of memory\n", len);
		return 1;
	}

	read(fd, data, len * sizeof(char));

	parse(data, len);

	free(data);

	close(fd);

	return 0;

}
