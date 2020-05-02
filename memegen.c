#include <stdlib.h>
#include <string.h>
#include <png.h>

#include "letters.h"
#include "util.h"

void usage() {
	fprintf(stderr, "memegen <-c text color> <-i inputfile> <-t top text> <-b bottom text>\n"
			"inputfile must not be empty and need at least top or bottom text\n"
			"Only supports ascii\n"
			"Output is written to stdout\n");
	exit(1);
}

char *argv0;
int main(int argc, char **argv) {
	char *top = "";
	char *bot = "";
	char *input=0;
	float scale = 1;
	uint32 color = 0xffffffff;

	ARGBEGIN {
		case 'i' :
			input = ARGF_(0);
			break;
		case 't':
			top = ARGF_(0);
			break;
		case 'b':
			bot = ARGF_(0);
			break;
		case 'c':
			color = strtol(ARGF_(0), NULL, 16);
			break;
		case 's':
			scale = atof(ARGF_(0));
			break;
		default:
			break;
	} ARGEND

	if(!input || (!top && !bot))
		usage();

	png_image image = {.version = PNG_IMAGE_VERSION, .opaque = NULL, };

	if(!png_image_begin_read_from_file(&image, input)) usage();
	image.format = PNG_FORMAT_ARGB;

	uint8 *buffer = malloc(PNG_IMAGE_SIZE(image));

	png_image_finish_read(&image, NULL, buffer, 0, NULL);

	uint32 topPos = image.height*0.05;
	uint32 botPos = image.height*0.95;

	uint32 topLength= strlen(top);
	uint32 botLength= strlen(bot);

	for(int y = 0; y < 8*scale;y++) {
		for(int x=0;x<topLength;x++){
			uint8 *current = font[top[x]];
			int row = current[(int)(y/scale)];
			int y0 = y+topPos;
			for(int i = 0;i<8*scale;i++) {
				int x0 = (x*8*scale+i)+ image.width/2 ;
				x0-=(topLength*scale*8)/2;
				if((row >> (int)(i/scale)) & 1){
					if(x0 < 0 || x0 > image.width-1)continue;
					if(y0 < 0 || y0 > image.height-1)continue;
					((uint32 *)buffer)[x0+y0*image.width] = color;
				}
			}
		}
		for(int x=0;x<botLength;x++){
			uint8 *current = font[bot[x]];
			int row = current[(int)(y/scale)];
			int y0 = botPos-8*scale+y;
			for(int i = 0;i<8*scale;i++) {
				int x0 = (x*8*scale+i)+ image.width/2 ;
				x0-=(topLength*scale*8)/2;
				if((row >> (int)(i/scale)) & 1){
					if(x0 < 0 || x0 > image.width-1)continue;
					if(y0 < 0 || y0 > image.height-1)continue;
					((uint32 *)buffer)[x0+y0*image.width] = color;
				}
			}
		}
	}


	png_image output = {0};
	output.format = PNG_FORMAT_ARGB;
	output.version = PNG_IMAGE_VERSION;
	output.width = image.width;
	output.height = image.height;

	png_image_write_to_stdio(&output, stdout, 0, buffer, 0, NULL);

	png_image_free(&image);
	free(buffer);
}
