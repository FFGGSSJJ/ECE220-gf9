#include <stdlib.h>
#include <string.h>

#include "mp8.h"


/*
 *	Homework: Machine Problem 8;
 *  Student: Guanshujie Fu
 *  Student ID: 3190110666
 *  File name: mp8recurse.c
 *  Description: For mp8recurse.c, it contains three recursive functions.
 * 				 Each functions would recurse and stop at specific conditions
 * 				 to mark the marking array. Details would be presented in 
 * 				 comments. 
 * 
*/


/*
 * basicRecurse -- flood fill recursively from a point until reaching
 *                 white or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
basicRecurse (int32_t width, int32_t height,
	      const uint8_t* inRed, const uint8_t* inGreen, 
	      const uint8_t* inBlue, 
	      int32_t x, int32_t y, 
	      uint8_t* marking)
{
	int32_t position;	// position is used to indicate the current index in array;
	int32_t xnew[4] = {x, x, x+1, x-1};	// xnew and ynew are arrays used to store the corresponding position of adjacent pixel
	int32_t ynew[4] = {y-1, y+1, y, y};
	
	position = x + y*width;	// calculate the current index;
	if (((inRed[position] & inBlue[position] & inGreen[position]) != 0xFF) && marking[position] == 0 
		&& x>-1 && x<width && y>-1 && y<height ){ // if current pixel is not white and hasn't been seen and is in the boundary
		marking[position] = 1;	// mark as 1
		for (int i = 0; i < 4; i++){	// loop to recurse every adjacent pixel;
			basicRecurse(width, height, inRed, inGreen, inBlue, xnew[i], ynew[i], marking);
		}
	}
	return;
}



/*
 * greyRecurse -- flood fill recursively from a point until reaching
 *                near-white pixels or the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         distSq -- maximum distance squared between white and boundary
 *                   pixel color
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
greyRecurse (int32_t width, int32_t height,
	     const uint8_t* inRed, const uint8_t* inGreen, 
	     const uint8_t* inBlue, 
	     int32_t x, int32_t y, uint32_t distSq, 
	     uint8_t* marking)
{
	int32_t position, check;	// position is used to store the current index, check is used store the return value of colorsWithinDisSq
	int32_t xnew[4] = {x, x, x+1, x-1};	// xnew and ynew are arrays stored coresponding positions of adjacent pixel
	int32_t ynew[4] = {y-1, y+1, y, y};

	position = x + y*width;		// calculate the index
	check = colorsWithinDistSq(inRed[position], inGreen[position], inBlue[position], 255, 255, 255, distSq);
	if ((marking[position] == 0) && check == 0 && x>-1 && x<width && y>-1 && y<height ){
		marking[position] = 1;	// if current pixel has not been seen and is in the boundary while also the RGB is out of max disSq
		for (int i = 0; i < 4; i++){	// loop to recurse
			greyRecurse(width, height, inRed, inGreen, inBlue, xnew[i], ynew[i], distSq, marking);
		}
	}
	return;
}


/*
 * limitedRecurse -- flood fill recursively from a point until reaching
 *                   pixels too different (in RGB color) from the color at
 *                   the flood start point, too far away (> 35 pixels), or
 *                   beyond the image border
 * INPUTS: width -- width of the input image
 *         height -- height of the input image
 *         inRed -- pointer to the input red channel (1-D array)
 *         inGreen -- pointer to the input green channel (1-D array)
 *         inBlue -- pointer to the input blue channel (1-D array)
 *         origX -- starting x position of the flood
 *         origY -- starting y position of the flood
 *         x -- current x position of the flood
 *         y -- current y position of the flood
 *         distSq -- maximum distance squared between pixel at origin 
 *                   and boundary pixel color
 *         marking -- array used to mark seen positions (indexed
 *                    in the same way as the RGB input arrays)
 * OUTPUTS: marking -- marked with flooded pixels
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
limitedRecurse (int32_t width, int32_t height,
	        const uint8_t* inRed, const uint8_t* inGreen, 
	        const uint8_t* inBlue, 
	        int32_t origX, int32_t origY, int32_t x, int32_t y, 
		uint32_t distSq, uint8_t* marking)
{
	int32_t position_orig, position_current, check1, check2;	// position_orig stored the orig index while position_curret stored the current index
	int32_t xnew[4] = {x, x, x+1, x-1};	// xnew and ynew work similarly as before
	int32_t ynew[4] = {y-1, y+1, y, y};

	position_orig = origX + origY*width;	// calculate the original and current index
	position_current = x + y*width;
	check1 = ((x - origX)*(x - origX) + (y - origY)*(y - origY) - 35*35) <= 0;	// check if the pixel distance is in the range
	check2 = colorsWithinDistSq(inRed[position_orig], inGreen[position_orig], inBlue[position_orig], 
								inRed[position_current], inGreen[position_current], inBlue[position_current], distSq);	// check if RGB distance is in the range

	if ((marking[position_current] == 0) && check1 && check2 && x>-1 && x<width && y>-1 && y<height ){
		marking[position_current] = 1;	// if current pixel has not been seen and is in the boundary and satisfy check1 and check2
		for (int i = 0; i < 4; i++){	// loop to recurse
			limitedRecurse(width, height, inRed, inGreen, inBlue, origX, origY, xnew[i], ynew[i], distSq, marking);
		}
	}
	
	return;
}

