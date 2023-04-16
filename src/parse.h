
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

int parse_int(char *data);

char parse_char(char *data);

char *parse_string(char *data, int len);

void parse(char *data, size_t len);

