#include <png.h>
#include <stdlib.h>


#include "arg.h"
char *argv0;

typedef unsigned char uint8;
typedef unsigned int uint32;

void usage() {
	fprintf(stderr, "memegen <-c text color> <-i inputfile> <-t top text> <-b bottom text>\n"
			"inputfile must not be empty and need at least top or bottom text\n"
			"Output is written to stdout\n");
	exit(1);
}

int main(int argc, char **argv) {
	char *top = 0;
	char *bot = 0;
	char *input=0;
	uint32 color = 0xffffff;

	ARGBEGIN {
		case 'i' :
			input = ARGF_(0);
			break;
		case 't':
			top = ARGF_(0);
			break;
		case 'd':
			bot = ARGF_(0);
			break;
		case 'c':
			color = atoi(ARGF_(0));
			break;
		default:
			break;
	} ARGEND

	if(!input || (!top && !bot))
		usage();

	png_image image = {.version = PNG_IMAGE_VERSION, .opaque = NULL, };

	if(!png_image_begin_read_from_file(&image, input)) usage();
	image.format = PNG_FORMAT_RGB;

	uint8 *buffer = malloc(PNG_IMAGE_SIZE(image));

	png_image_finish_read(&image, NULL, buffer, 0, NULL);



	free(buffer);
	png_image_free(&image);



	png_image output = {0};
	output.format = PNG_FORMAT_RGB;
	output.version = PNG_IMAGE_VERSION;
	output.width = image.width;
	output.height = image.height;

	png_image_write_to_stdio(&output, stdout, 0, buffer, 0, NULL);
}
