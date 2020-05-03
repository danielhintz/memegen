#include <stdlib.h>
#include <string.h>
#include <png.h>

#include "letters.h"
#include "util.h"

void usage() {
	fprintf(stderr, "memegen <-c text color> <-i inputfile> <-o outputfile> <-t top text> <-b bottom text>\n"
			"if inputfile is missing it reads from stdin, if outputfile is missing goes to stdout \n"
			"Need at least top or bottom text\n"
			"Only supports ascii\n"
			"Output is written to stdout unless an output file is provided\n");
	exit(1);
}

void
blitText(uint32 *buffer, char *text, png_image *image, uint32 color, int xStart, int yStart, float scale) {
	if(!text)return;
	int len = strlen(text);
	for(int y = 0; y < 8*scale;y++) {
		for(int i = 0;i<8*scale;i++) {
			for(int x=0;x<len;x++){
				uint8 *current = font[text[x]];
				int row = current[(int)(y/scale)];
				int y0 = y+yStart;
				int x0 = (x*8*scale+i) + xStart ;
				if((row >> (int)(i/scale)) & 1){
					if(x0 < 0 || x0 > image->width-1)continue;
					if(y0 < 0 || y0 > image->height-1)continue;
					buffer[x0+y0*image->width] = color;
				}
			}
		}
	}
}

char *argv0;
int main(int argc, char **argv) {
	char *top = "";
	char *bot = "";
	FILE *inputfile=stdin;
	FILE *outputfile=stdout;
	float scale = 1;
	uint32 color = 0xffffffff;

	ARGBEGIN {
		case 'i' :
			inputfile = fopen(ARGF_(0), "r");
			break;
		case 'o' :
			outputfile = fopen(ARGF_(0), "w+");
			break;
		case 't':
			top = ARGF_(0);
			break;
		case 'b':
			bot = ARGF_(0);
			break;
		case 'c':
			color = strtol(ARGF_(0), NULL, 16);
			int r = color&0xff;
			int g = (color >> 8)&0xff;
			int b = (color >> 16)&0xff;
			int a=0xff;
			color = (a<<24)|(r<<16)|(g<<8)|b;
			break;
		case 's':
			scale = atof(ARGF_(0));
			break;
		default:
			break;
	} ARGEND

	if((!top && !bot))
		usage();

	png_image image = {.version = PNG_IMAGE_VERSION, .opaque = NULL, };


	if(!png_image_begin_read_from_stdio(&image, inputfile)) usage();
	image.format = PNG_FORMAT_RGBA;

	uint8 *buffer = malloc(PNG_IMAGE_SIZE(image));

	png_image_finish_read(&image, NULL, buffer, 0, NULL);

	uint32 topPos = image.height*0.05;
	uint32 botPos = image.height*0.95;

	uint32 topLen = strlen(top);
	uint32 botLen = strlen(bot);

	blitText((uint32 *)buffer, top, &image, color, image.width/2-(topLen*scale*8)/2, topPos, scale);
	blitText((uint32 *)buffer, bot, &image, color, image.width/2-(botLen*scale*8)/2, botPos-8*scale, scale);

	png_image output = {0};
	output.format = PNG_FORMAT_RGBA;
	output.version = PNG_IMAGE_VERSION;
	output.width = image.width;
	output.height = image.height;

	png_image_write_to_stdio(&output, outputfile, 0, buffer, 0, NULL);

	png_image_free(&image);
	free(buffer);
}
