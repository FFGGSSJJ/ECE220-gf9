/*									tab:8
 *
 * main.c - skeleton source file for ECE220 picture drawing program
 *
 * "Copyright (c) 2018 by Charles H. Zega, and Saransh Sinha."
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice and the following
 * two paragraphs appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE AUTHOR OR THE UNIVERSITY OF ILLINOIS BE LIABLE TO 
 * ANY PARTY FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL 
 * DAMAGES ARISING OUT  OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, 
 * EVEN IF THE AUTHOR AND/OR THE UNIVERSITY OF ILLINOIS HAS BEEN ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE AUTHOR AND THE UNIVERSITY OF ILLINOIS SPECIFICALLY DISCLAIM ANY 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE 
 * PROVIDED HEREUNDER IS ON AN "AS IS" BASIS, AND NEITHER THE AUTHOR NOR
 * THE UNIVERSITY OF ILLINOIS HAS ANY OBLIGATION TO PROVIDE MAINTENANCE, 
 * SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Author:	    Charles Zega, Saransh Sinha, Hanwen Liu
 * Version:	    1
 * Creation Date:   12 February 2018
 * Filename:	    mp5.h
 * History:
 *	CZ	1	12 February 2018
 *		First written.
 *      HL      2       31 October 2020
 *              This program is turned in as Hanwen's MP5 in ECE 220.
 *              No one has permission to modify or copy the context of 
 *              draw_picture() without handwritten agreement from HL.
 */

#include "mp5.h"
/*
	You must write all your code only in this file, for all the functions!
*/


int32_t
draw_solid_parallelogram(int32_t x_A, int32_t y_A, int32_t x_B, int32_t y_B, int32_t x_C, int32_t y_C, int32_t color);

int32_t
near_horizontal_2(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end);

int32_t
near_vertical_2(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end);

int32_t
draw_line_2(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end);

/* 
 *  near_horizontal
 *	 
 *	 
 *	
 *	 
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
near_horizontal(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
        // Here is the condition of priting a single dot.
        if (x_start == x_end && y_start == y_end)
                return draw_dot(x_start, y_start);
	int32_t return_value = 1;
	int32_t i, y_present, tmp;
        // i is used as a cyclic variable, y_present is calculated through an equation.
        if (x_start > x_end){ // x_start should smaller than x_end.
                tmp = x_start; x_start = x_end; x_end = tmp;
                tmp = y_start; y_start = y_end; y_end = tmp; // swap.
        }for (i = x_start; i <= x_end; i = i + 1){
		y_present = (2*(y_end-y_start)*(i-x_start)+(y_end-y_start>0? 1: -1)*(x_end-x_start))/
			    (2*(x_end-x_start))+ y_start; // caululate y_present.
		return_value = return_value & draw_dot(i, y_present);
                // draw dot and change the return value accordingly.
	}return return_value;
}


/* 
 *  near_vertical
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
near_vertical(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
        // Here is the condition of priting a single dot.
        if (x_start == x_end && y_start == y_end)
                return draw_dot(x_start, y_start);
	int32_t return_value = 1;
	int32_t i, x_present, tmp;
        // i is used as a cyclic variable, x_present is calculated through an equation.
        if (y_start > y_end){ // y_start should smaller than x_end.
                tmp = x_start; x_start = x_end; x_end = tmp;
                tmp = y_start; y_start = y_end; y_end = tmp;  // swap.
        }for (i = y_start; i <= y_end; i = i + 1){
		x_present = (2*(x_end-x_start)*(i-y_start)+(x_end-x_start>0? 1: -1)*(y_end-y_start))/
			    (2*(y_end-y_start))+ x_start; // caululate x_present.
		return_value = return_value & draw_dot(x_present, i);
                // draw dot and change the return value accordingly.
	}return return_value;
}

/* 
 *  draw_line
 *	 
 *	 
 *	
 *	
 * INPUTS: x_start,y_start -- the coordinates of the pixel at one end of the line
 * 	   x_end, y_end    -- the coordinates of the pixel at the other end
 * OUTPUTS: draws a pixel to all points in between the two given pixels including
 *          the end points
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_line(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
        // Here is the condition of priting a single dot.
        if (x_start == x_end && y_start == y_end)
                return draw_dot(x_start, y_start);
        // Here is the condition of priting a vertical line.
	if (x_end == x_start)
		return near_vertical(x_start, y_start, x_end, y_end);
        // Here is the condition of the line's slope belongs to [-1, 1].
	if ((y_end-y_start == x_end-x_start)||(y_end-y_start == x_start-x_end)||
		((y_end-y_start)/(x_start-x_end)== 0))
		return near_horizontal(x_start, y_start, x_end, y_end);
        // Otherwise.
	return near_vertical(x_start, y_start, x_end, y_end);
}


/* 
 *  draw_rect
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the coordinates of the of the top-left pixel of the rectangle
 *         w,h -- the width and height, respectively, of the rectangle
 * OUTPUTS: draws a pixel to every point of the edges of the rectangle
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_rect(int32_t x, int32_t y, int32_t w, int32_t h){
        // Invalid input.
        if (w < 0 || h < 0)
                return 0;
        // Draw four lines.
	return draw_line(x, y, x+w, y)&draw_line(x+w, y, x+w, y+h)&
               draw_line(x+w, y+h, x, y+h)&draw_line(x, y+h, x, y);
}


/* 
 *  draw_triangle
 *	 
 *	 
 *	
 *	
 * INPUTS: x_A,y_A -- the coordinates of one of the vertices of the triangle
 *         x_B,y_B -- the coordinates of another of the vertices of the triangle
 *         x_C,y_C -- the coordinates of the final of the vertices of the triangle
 * OUTPUTS: draws a pixel to every point of the edges of the triangle
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_triangle(int32_t x_A, int32_t y_A, int32_t x_B, int32_t y_B, int32_t x_C, int32_t y_C){
	// Draw three lines.
        return draw_line(x_A, y_A, x_B, y_B)&draw_line(x_B, y_B, x_C, y_C)&draw_line(x_C, y_C, x_A, y_A);
}

/* 
 *  draw_parallelogram
 *	 
 *	 
 *	
 *	
 * INPUTS: x_A,y_A -- the coordinates of one of the vertices of the parallelogram
 *         x_B,y_B -- the coordinates of another of the vertices of the parallelogram
 *         x_C,y_C -- the coordinates of another of the vertices of the parallelogram
 * OUTPUTS: draws a pixel to every point of the edges of the parallelogram
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_parallelogram(int32_t x_A, int32_t y_A, int32_t x_B, int32_t y_B, int32_t x_C, int32_t y_C){
	int32_t x_D, y_D;
	x_D = x_A + x_C - x_B;
	y_D = y_A + y_C - y_B; // Calculate the rest vertice.
        // Draw four lines.
	return draw_line(x_A, y_A, x_B, y_B)&draw_line(x_B, y_B, x_C, y_C)&
	       draw_line(x_C, y_C, x_D, y_D)&draw_line(x_D, y_D, x_A, y_A);
}


/* 
 *  draw_circle
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the center of the circle
 *         inner_r,outer_r -- the inner and outer radius of the circle
 * OUTPUTS: draws a pixel to every point whose distance from the center is
 * 	    greater than or equal to inner_r and less than or equal to outer_r
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
draw_circle(int32_t x, int32_t y, int32_t inner_r, int32_t outer_r){
        //Invalid input
        if (inner_r > outer_r || inner_r < 0)
                return 0;
        int32_t i, j, d_2, return_value = 1;
        for (i = x - outer_r; i <= x + outer_r; i = i + 1){
                for (j = y - outer_r; j <= y + outer_r; j = j + 1){ 
                        // Check every single possible point.
                        d_2 =(i - x)*(i - x)+(j - y)*(j - y);
                        // Calculate the square of the distance between the point and the center.
                        if (d_2 <= outer_r*outer_r && d_2 >= inner_r*inner_r)
                                return_value = return_value & draw_dot(i, j);
                }
        }return return_value;
}


/* 
 *  rect_gradient
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the coordinates of the of the top-left pixel of the rectangle
 *         w,h -- the width and height, respectively, of the rectangle
 *         start_color -- the color of the far left side of the rectangle
 *         end_color -- the color of the far right side of the rectangle
 * OUTPUTS: fills every pixel within the bounds of the rectangle with a color
 *	    based on its position within the rectangle and the difference in
 *          color between start_color and end_color
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */

int32_t
rect_gradient(int32_t x, int32_t y, int32_t w, int32_t h, int32_t start_color, int32_t end_color){
	int32_t i, j, tmp, return_value = 1;
        int32_t scolor[3], ecolor[3], bcolor[3], present_color;
        // Invalid input.
        if (w < 1 || h < 0)
                return 0;
        // Get the red, green, blue level of start_color and end_color.
        for (i = 0; i < 3; i = i + 1){
                scolor[i] = (start_color&(0xff <<(i * 8)))>>(i * 8);
                ecolor[i] = (end_color&(0xff <<(i * 8)))>>(i * 8);
        }for (i = 0; i <= w; i = i + 1){
                present_color = 0;
                for (j = 0; j < 3; j = j + 1){
                        tmp = ecolor[j] - scolor[j];
                        bcolor[j] = (2 * i * tmp +(tmp > 0? 1: -1)* w)/(2 * w) + scolor[j];
                        present_color = present_color +(bcolor[j] <<(j * 8));
                        // Calculate the different levels of colors according the equation one by one.
                }set_color(present_color);
                return_value = return_value & draw_line(x + i, y, x + i, y + h);
                // Draw a verticle line with the calculated color.
        }return return_value;
}


/* 
 *  draw_picture
 *	 
 *	 
 *	
 *	
 * INPUTS: none
 * OUTPUTS: alters the image by calling any of the other functions in the file
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */


int32_t
draw_picture(){
        int return_value = 1;
	int x, y, a, b;
        
        // (x, y) is the coordinate of the very left upper corner of my shape.
        // a and b controls the size of my shape.

        int color[4];
        color[0] = 0xD9EDC6;
        color[1] = 0x6B9362;
        color[2] = 0x2D4436;
        color[3] = 0x2E372E;

        // Here is the left part of my picture.
        x = 50; y = 15; a = 6; b = 7;

        return_value &= draw_solid_parallelogram(x+4*a, y+32*b, x+4*a, y+12*b, x+20*a, y+4*b, color[2]);
	return_value &= draw_solid_parallelogram(x+20*a, y+4*b, x+30*a, y+9*b, x+30*a, y+30*b, color[1]);
	return_value &= draw_solid_parallelogram(x+12*a, y+16*b, x+12*a, y+8*b, x+20*a, y+12*b, color[1]);
	return_value &= draw_solid_parallelogram(x+14*a, y+25*b, x+4*a, y+20*b, x+12*a, y+16*b, color[0]);
	return_value &= draw_solid_parallelogram(x+24*a, y+34*b, x+28*a, y+36*b, x+28*a, y+20*b, color[1]);
	return_value &= draw_solid_parallelogram(x+32*a, y+30*b, x+36*a, y+32*b, x+36*a, y+16*b, color[1]);
	return_value &= draw_solid_parallelogram(x, y+30*b, x, y+10*b, x+4*a, y+12*b, color[1]);
	return_value &= draw_solid_parallelogram(x+4*a, y+24*b, x+4*a, y+20*b, x+14*a, y+25*b, color[1]);
	return_value &= draw_solid_parallelogram(x+16*a, y+18*b, x+20*a, y+20*b, x+20*a, y+40*b, color[1]);
	return_value &= draw_solid_parallelogram(x+20*a, y+20*b, x+20*a, y+40*b, x+24*a, y+38*b, color[2]);
	return_value &= draw_solid_parallelogram(x+28*a, y+20*b, x+28*a, y+36*b, x+32*a, y+34*b, color[2]);
	return_value &= draw_solid_parallelogram(x+36*a, y+16*b, x+36*a, y+32*b, x+40*a, y+30*b, color[2]);
	return_value &= draw_solid_parallelogram(x+24*a, y+18*b, x+24*a, y+22*b, x+40*a, y+14*b, color[2]);
	return_value &= draw_solid_parallelogram(x+14*a, y+25*b, x+14*a, y+29*b, x+16*a, y+28*b, color[2]);
	return_value &= draw_solid_parallelogram(x+22*a, y+13*b, x+22*a, y+15*b, x+26*a, y+13*b, color[2]);
	return_value &= draw_solid_parallelogram(x+30*a, y+9*b, x+30*a, y+11*b, x+34*a, y+9*b, color[2]);
	return_value &= draw_solid_parallelogram(x+16*a, y+18*b, x+20*a, y+20*b, x+40*a, y+10*b, color[0]);
	return_value &= draw_solid_parallelogram(x+20*a, y, x, y+10*b, x+4*a, y+12*b, color[0]);
	return_value &= draw_solid_parallelogram(x+12*a, y+8*b, x+22*a, y+13*b, x+26*a, y+11*b, color[0]);
	return_value &= draw_solid_parallelogram(x+20*a, y+4*b, x+30*a, y+9*b, x+34*a, y+7*b, color[0]);

        // Here is the right part of my picture.
        set_color(color[3]);
        x = 340; y = 15; a = 6; b = 7;

        return_value &= draw_line(x, y+10*b, x, y+30*b);
	return_value &= draw_line(x+4*a, y+12*b, x+4*a, y+20*b);
	return_value &= draw_line(x+4*a, y+24*b, x+4*a, y+32*b);
	return_value &= draw_line(x+12*a, y+8*b, x+12*a, y+16*b);
	return_value &= draw_line(x+20*a, y+4*b, x+20*a, y+8*b);
	return_value &= draw_line(x+16*a, y+18*b, x+16*a, y+38*b);
	return_value &= draw_line(x+20*a, y+20*b, x+20*a, y+40*b);
	return_value &= draw_line(x+24*a, y+22*b, x+24*a, y+38*b);
	return_value &= draw_line(x+28*a, y+20*b, x+28*a, y+36*b);
	return_value &= draw_line(x+32*a, y+18*b, x+32*a, y+34*b);
	return_value &= draw_line(x+36*a, y+16*b, x+36*a, y+32*b);
	return_value &= draw_line(x+40*a, y+10*b, x+40*a, y+30*b);
	return_value &= draw_line(x+22*a, y+13*b, x+22*a, y+15*b);
	return_value &= draw_line(x+26*a, y+11*b, x+26*a, y+13*b);
	return_value &= draw_line(x+30*a, y+9*b, x+30*a, y+11*b);
	return_value &= draw_line(x+34*a, y+7*b, x+34*a, y+9*b);
	return_value &= draw_line(x+14*a, y+25*b, x+14*a, y+29*b);

	return_value &= draw_line(x, y+10*b, x+20*a, y);
	return_value &= draw_line(x+4*a, y+12*b, x+12*a, y+8*b);
	return_value &= draw_line(x+16*a, y+6*b, x+20*a, y+4*b);
	return_value &= draw_line(x+4*a, y+20*b, x+12*a, y+16*b);
	return_value &= draw_line(x+4*a, y+32*b, x+12*a, y+28*b);
	return_value &= draw_line(x+20*a, y+40*b, x+24*a, y+38*b);
	return_value &= draw_line(x+28*a, y+36*b, x+32*a, y+34*b);
	return_value &= draw_line(x+36*a, y+32*b, x+40*a, y+30*b);
	return_value &= draw_line(x+24*a, y+22*b, x+28*a, y+20*b);
	return_value &= draw_line(x+32*a, y+18*b, x+36*a, y+16*b);
	return_value &= draw_line(x+20*a, y+20*b, x+40*a, y+10*b);
	return_value &= draw_line(x+16*a, y+18*b, x+36*a, y+8*b);
	return_value &= draw_line(x+16*a, y+6*b, x+20*a, y+4*b);
	return_value &= draw_line(x+14*a, y+29*b, x+16*a, y+28*b);
	return_value &= draw_line(x+14*a, y+25*b, x+16*a, y+24*b);
	return_value &= draw_line(x+22*a, y+13*b, x+26*a, y+11*b);
	return_value &= draw_line(x+30*a, y+9*b, x+34*a, y+7*b);

	return_value &= draw_line(x, y+10*b, x+4*a, y+12*b);
	return_value &= draw_line(x+12*a, y+16*b, x+20*a, y+20*b);
	return_value &= draw_line(x+36*a, y+8*b, x+40*a, y+10*b);
	return_value &= draw_line(x+24*a, y+34*b, x+28*a, y+36*b);
	return_value &= draw_line(x+32*a, y+30*b, x+36*a, y+32*b);
	return_value &= draw_line(x, y+30*b, x+4*a, y+32*b);
	return_value &= draw_line(x+16*a, y+38*b, x+20*a, y+40*b);
	return_value &= draw_line(x+12*a, y+8*b, x+22*a, y+13*b);
	return_value &= draw_line(x+16*a, y+6*b, x+26*a, y+11*b);
	return_value &= draw_line(x+20*a, y+4*b, x+30*a, y+9*b);
	return_value &= draw_line(x+20*a, y, x+34*a, y+7*b);
	return_value &= draw_line(x+4*a, y+20*b, x+14*a, y+25*b);
	return_value &= draw_line(x+4*a, y+24*b, x+14*a, y+29*b);

	return return_value;
}

int32_t
draw_solid_parallelogram(int32_t x_A, int32_t y_A, int32_t x_B, int32_t y_B, int32_t x_C, int32_t y_C, int32_t color){
        set_color(color);
	int32_t i, x0, x1, y0, y1, return_value = 1;
        if (x_A == x_B){
                for (i = y_A; i != y_B; i += (y_A < y_B? 1: -1)){
                        x0 = x_A; x1 = x_C;
                        y0 = i; y1 = y0 + y_C - y_B;
                        return_value &= draw_line(x0, y0, x1, y1);
                }
        }else if (y_A == y_B){
                for (i = x_A; i != x_B; i += (x_A < x_B? 1: -1)){
                        y0 = y_A; y1 = y_C;
                        x0 = i; x1 = x0 + x_C - x_B;
                        return_value &= draw_line(x0, y0, x1, y1);
                }
        }else if ((y_A-y_B)/(x_A-x_B)== 0){
                if (x_A < x_B){
                        for (i = x_A; i < x_B; ++i){
                                x0 = i; x1 = x0 + x_C - x_B;
                                y0 = (2*(y_B-y_A)*(x0-x_A)+(y_B-y_A>0? 1: -1)*(x_B-x_A))/
                                     (2*(x_B-x_A))+ y_A;
                                y1 = y0 + y_C - y_B;
                                return_value &= draw_line_2(x0, y0, x1, y1)&draw_line_2(x0+1, y0, x1+1, y1);
                        }
                }else{
                        for (i = x_A; i > x_B; --i){
                                x0 = i; x1 = x0 + x_C - x_B;
                                y0 = (2*(y_B-y_A)*(x0-x_A)+(y_B-y_A>0? 1: -1)*(x_B-x_A))/
                                     (2*(x_B-x_A))+ y_A;
                                y1 = y0 + y_C - y_B;
                                return_value &= draw_line_2(x0, y0, x1, y1)&draw_line_2(x0-1, y0, x1-1, y1);
                        }
                }
        }else{
                if (y_A < y_B){
                        for (i = y_A; i < y_B; ++i){
                                y0 = i; y1 = y0 + y_C - y_B;
                                x0 = (2*(x_B-x_A)*(y0-y_A)+(x_B-x_A>0? 1: -1)*(y_B-y_A))/
                                     (2*(y_B-y_A))+ x_A;
                                x1 = x0 + x_C - x_B;
                                return_value &= draw_line_2(x0, y0, x1, y1)&draw_line_2(x0, y0+1, x1, y1+1);
                        }
                }else{
                        for (i = y_A; i > y_B; --i){
                                y0 = i; y1 = y0 + y_C - y_B;
                                x0 = (2*(x_B-x_A)*(y0-y_A)+(x_B-x_A>0? 1: -1)*(y_B-y_A))/
                                     (2*(y_B-y_A))+ x_A;
                                x1 = x0 + x_C - x_B;
                                return_value &= draw_line_2(x0, y0, x1, y1)&draw_line_2(x0, y0+1, x1, y1+1);
                        }
                }
        }return draw_line(x_A, y_A, x_B, y_B)&draw_line(x_B, y_B, x_C, y_C)&
	        draw_line(x_C, y_C, x_A+x_C-x_B, y_A+y_C-y_B)&
                draw_line(x_A+x_C-x_B, y_A+y_C-y_B, x_A, y_A)&return_value;
}

int32_t
near_horizontal_2(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
        if (x_start == x_end && y_start == y_end)
                return 1;
	int32_t return_value = 1;
	int32_t i, y_present, tmp;
        if (x_start > x_end){
                tmp = x_start; x_start = x_end; x_end = tmp;
                tmp = y_start; y_start = y_end; y_end = tmp;
        }for (i = x_start + 1; i < x_end; i = i + 1){
		y_present = (2*(y_end-y_start)*(i-x_start)+(y_end-y_start>0? 1: -1)*(x_end-x_start))/
			    (2*(x_end-x_start))+ y_start;
		return_value = return_value & draw_dot(i, y_present);
	}return return_value;
}

int32_t
near_vertical_2(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
        if (x_start == x_end && y_start == y_end)
                return 1;
	int32_t return_value = 1;
	int32_t i, x_present, tmp;
        if (y_start > y_end){
                tmp = x_start; x_start = x_end; x_end = tmp;
                tmp = y_start; y_start = y_end; y_end = tmp;
        }for (i = y_start + 1; i < y_end; i = i + 1){
		x_present = (2*(x_end-x_start)*(i-y_start)+(x_end-x_start>0? 1: -1)*(y_end-y_start))/
			    (2*(y_end-y_start))+ x_start;
		return_value = return_value & draw_dot(x_present, i);
	}return return_value;
}

int32_t
draw_line_2(int32_t x_start, int32_t y_start, int32_t x_end, int32_t y_end){
        if (x_start == x_end && y_start == y_end)
                return 1;
	if (x_end == x_start)
		return near_vertical_2(x_start, y_start, x_end, y_end);
	if ((y_end-y_start == x_end-x_start)||(y_end-y_start == x_start-x_end)||
		((y_end-y_start)/(x_start-x_end)== 0))
		return near_horizontal_2(x_start, y_start, x_end, y_end);
	return near_vertical_2(x_start, y_start, x_end, y_end);
}