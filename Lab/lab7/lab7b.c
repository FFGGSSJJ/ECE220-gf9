#include <math.h>
#include <stdint.h>
#include <stdio.h>

#include "lab7.h"

void 
edgeDetect (int32_t width, int32_t height, 
	    const int32_t* sobelX, const int32_t* sobelY,
	    const uint8_t* inRed, const uint8_t* inGreen, 
	    const uint8_t* inBlue, uint8_t* outMono, uint8_t* temp)
{
	int32_t row, col, sobe_row, sobe_col;					// row and col are used to loop through the whole picture
	int32_t G_x, G_y, G_all;								// sobe_row and col are used to loop through the sobel matrix;

	for (row = 0; row < height; row++){
		for ( col = 0; col < width; col++)
			*(temp + row*width + col) = *(inRed + row*width + col)*0.2125 + 
										*(inGreen + row*width + col)*0.7154 + 
										*(inBlue + row*width + col)*0.0721;
	}

	for ( row = 0; row < height - 2; row++){
		for ( col = 0; col < width - 2; col++){
			for ( sobe_row = 0, G_x = 0, G_y = 0; sobe_row < 3; sobe_row++){		// for every pixel in a picture, find the 3 by 3 sobel matrix and calculate G_x, G_y;
				for (sobe_col = 0; sobe_col < 3; sobe_col++){
					G_x += *(temp + (row + sobe_row)*width + col + sobe_col) * (*(sobelX + sobe_row*3 + sobe_col));
					G_y += *(temp + (row + sobe_row)*width + col + sobe_col) * (*(sobelY + sobe_row*3 + sobe_col));
				}
			} G_all = (int32_t) sqrt(G_x*G_x  + G_y*G_y)>255?255:sqrt(G_x*G_x  + G_y*G_y);
			for (sobe_row = 0; sobe_row < 3; sobe_row++)
				for (sobe_col = 0; sobe_col < 3; sobe_col++)
					*(outMono + (row + sobe_row)*width + col + sobe_col) = G_all;	
		}	 
	}
}

