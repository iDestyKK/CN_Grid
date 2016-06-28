//Zig-Zag texture generator with CN_Grids

#include <iostream>
#include <cstdlib>
#include <fstream>

#include "../cn_grid.hpp"

typedef unsigned char byte;
typedef unsigned int  uint;

int main() {
	grid<byte> img(1024, 1024);
	//Go down each row and make the gradient
	bool xdir, ydir, in;
	uint i;
	byte v;
	i = 0;
	for (uint a = 0; a < img.sizex(); a++) {
		v = i;
		for (uint b = 0; b < img.sizey(); b++) {
			img[b][a] = v;
			if ((a % 256 == 0 && a % 512 != 0 && a > 0) || (xdir && v == 0))
				img[b][a] = 255 - v;
			if ((a - 256) % 512 == 0 && (b - 256) % 512 == 0)
				img[b][a] = 255 - img[b][a];

			if (v % 256 == 0) xdir = !xdir;
			if (xdir)
				v++;
			else
				v--;
		}
		if (i % 256 == 0) ydir = !ydir;
		if (ydir)
			i++;
		else
			i--;
	}

	//Write the PGM file
	char* header = "P5\n1024 1024\n255\n";
	FILE* fp = fopen("tex.pgm", "wb");
	fwrite(header, sizeof(byte), strlen(header), fp);
	fwrite(img.data(), sizeof(byte), img.size(), fp);
	fclose(fp);
}
