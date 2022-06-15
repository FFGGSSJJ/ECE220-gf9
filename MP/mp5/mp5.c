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
 * Author:	    Charles Zega, Saransh Sinha
 * Version:	    1
 * Creation Date:   12 February 2018
 * Filename:	    mp5.h
 * History:
 *	CZ	1	12 February 2018
 *		First written.
 */
#include "mp5.h"
#include <stdio.h>
#include <math.h>
/*
	You must write all your code only in this file, for all the functions!
*/

/* Three functions written by myself to implement the NASA logo*/
int32_t 
draw_right_semicircle(int32_t x, int32_t y, int32_t inner_r, int32_t outer_r);
int32_t
draw_left_semicircle(int32_t x, int32_t y, int32_t inner_r, int32_t outer_r);
int32_t
draw_ellipse(int32_t F1_x, int32_t F1_y, int32_t F2_x, int32_t F2_y, int32_t a);


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
	/* Your code goes here! */
    int32_t x1, y1, x2, y2;
    int32_t x_cal, y_cal, sign;
    int32_t feedback = 1;                                           // feedback is the return value of the function

    x1 = ((x_start - x_end)<0)?x_start:x_end;                       /* x1 is the smaller x-coordinate while x2 is the lager one */
    y1 = ((x_start - x_end)<0)?y_start:y_end;
    x2 = x_start + x_end - x1;
    y2 = y_start + y_end - y1;
    if (y2 - y1)
        sign = (y2 - y1 < 0)?-1:1;                                  // Calculate the sign if y2 - y1
    else
        sign = 0;
        
    for (x_cal = x1; x_cal <= x2; x_cal++)                          // Use a loop to count for all the points on the line
    {
        y_cal = (2*(y2 - y1)*(x_cal - x1) + sign*(x2 - x1))/(2*(x2 - x1)) + y1;
        feedback &= draw_dot(x_cal, y_cal);                         // draw the dot and use the return value to AND with feedback
    }
    
    return feedback;
    
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
	/* Your code goes here! */
    int32_t x1, y1, x2, y2;
    int32_t x_cal, y_cal, sign;
    int32_t feedback = 1;

    if (x_start == x_end && y_start == y_end){
        return draw_dot(x_start, y_start);
    }
    
    y1 = (y_start - y_end < 0)?y_start:y_end;                       /* y1 is the smaller y-coordinate while y2 is the larger one*/
    x1 = (y_start - y_end < 0)?x_start:x_end;
    y2 = y_start + y_end - y1;
    x2 = x_start + x_end - x1;
    if (x2 - x1)
        sign = (x2 - x1 < 0)?-1:1;                                  // Calculate the sign of x2 - x1
    else
        sign = 0;
    
    for (y_cal = y1; y_cal <= y2; y_cal++){                         // Use a loop to count for all the points on the line
        x_cal = (2*(x2 - x1)*(y_cal - y1) + sign*(y2 - y1))/(2*(y2 - y1)) + x1;
        feedback &= draw_dot(x_cal, y_cal);                         // Use the return value to end with feedback
    }
    return feedback;
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
	/* Your code goes here! */
    int32_t slope;
    int32_t feedback = 1;

    if (x_end == x_start || (x_end == x_start && y_start == y_end))             // Two situations when we should draw vertical line
        feedback &= near_vertical(x_start, y_start, x_end, y_end);
        
    else
    {
        slope = (y_end - y_start)/(x_end - x_start);
        if (1 < slope || -1 > slope)
            feedback &= near_vertical(x_start, y_start, x_end, y_end);
        else
            feedback &= near_horizontal(x_start, y_start, x_end, y_end);   
    }
    
	return feedback;
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
	/* Your code goes here! */
    int32_t feedback = 1;
    if (h < 0 || w <= 0)                                        // if h, w do not satisfy the conditions, return 0;
        return 0;                                               // draw the four lines for rectangle
    else                                                        
    {
        feedback &= draw_line(x, y, x+w, y);
        feedback &= draw_line(x, y, x, y+h);
        feedback &= draw_line(x+w, y, x+w, y+h);
        feedback &= draw_line(x, y+h, x+w, y+h);
    }
    return feedback;
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
	/* Your code goes here! */
    int32_t feedback = 1;                       
                                                            // draw the three lines for triangle;
    feedback &= draw_line(x_A, y_A, x_B, y_B);          
    feedback &= draw_line(x_A, y_A, x_C, y_C);
    feedback &= draw_line(x_B, y_B, x_C, y_C);

	return feedback;
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
	/* Your code goes here! */
    int32_t x_D, y_D;
    int32_t feedback = 1;
    int32_t width, length;

    width = x_B - x_A;
    length = y_B - y_A;
    x_D = x_C - width;
    y_D = y_C - length;

    feedback &= draw_line(x_A, y_A, x_B, y_B);
    feedback &= draw_line(x_A, y_A, x_D, y_D);
    feedback &= draw_line(x_B, y_B, x_C, y_C);
    feedback &= draw_line(x_D, y_D, x_C, y_C);

	return feedback;
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
	/* Your code goes here!*/
    int32_t dis_sq, x_check, y_check;                   // dis_sq is used to calculated the squares of distance of two points
    int32_t out_sq, in_sq;
    int32_t feedback = 1;
    
    out_sq = outer_r*outer_r;
    in_sq = inner_r*inner_r;
    
    if (inner_r < 0 || outer_r < inner_r) return 0;     // if outer_r or inner_r does not satisfy conditions, return 0
    
    for (x_check = x - outer_r; x_check <= x + outer_r; x_check++){
        for (y_check = y - outer_r; y_check <= y + outer_r; y_check++){
            dis_sq = (x - x_check)*(x - x_check) + (y - y_check)*(y - y_check);
            if (dis_sq >= in_sq && dis_sq <= out_sq )   // if the distance is between inner_r and outer_r, so are their squares
                feedback &= draw_dot(x_check, y_check);
        }
        
    }
	return feedback;
}


/* 
 *  draw_right_semicircle
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the center of the circle
 *         inner_r,outer_r -- the inner and outer radius of the circle
 * OUTPUTS: draws a pixel to every point whose distance from the center is
 * 	    greater than or equal to inner_r and less than or equal to outer_r
 *      and the x of it is greater than the center.
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */
int32_t
draw_right_semicircle(int32_t x, int32_t y, int32_t inner_r, int32_t outer_r){
	/* Your code goes here!*/
    int32_t dis_sq, x_check, y_check;
    int32_t out_sq, in_sq;
    int32_t feedback = 1;
    
    out_sq = outer_r*outer_r;
    in_sq = inner_r*inner_r;
    
    if (inner_r < 0 || outer_r < inner_r) return 0;
    
    for (x_check = x - outer_r; x_check <= x + outer_r; x_check++){
        for (y_check = y - outer_r; y_check <= y + outer_r; y_check++){
            dis_sq = (x - x_check)*(x - x_check) + (y - y_check)*(y - y_check);
            if (dis_sq >= in_sq && dis_sq <= out_sq && x <= x_check)
                feedback &= draw_dot(x_check, y_check);
        }
        
    }
	return feedback;
}



/* 
 *  draw_left_semicircle
 *	 
 *	 
 *	
 *	
 * INPUTS: x,y -- the center of the circle
 *         inner_r,outer_r -- the inner and outer radius of the circle
 * OUTPUTS: draws a pixel to every point whose distance from the center is
 * 	    greater than or equal to inner_r and less than or equal to outer_r
 *      and the x of it is less than the center.
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */
int32_t
draw_left_semicircle(int32_t x, int32_t y, int32_t inner_r, int32_t outer_r){
	/* Your code goes here!*/
    int32_t dis_sq, x_check, y_check;
    int32_t out_sq, in_sq;
    int32_t feedback = 1;
    
    out_sq = outer_r*outer_r;
    in_sq = inner_r*inner_r;
    
    if (inner_r < 0 || outer_r < inner_r) return 0;
    
    for (x_check = x - outer_r; x_check <= x + outer_r; x_check++){
        for (y_check = y - outer_r; y_check <= y + outer_r; y_check++){
            dis_sq = (x - x_check)*(x - x_check) + (y - y_check)*(y - y_check);
            if (dis_sq >= in_sq && dis_sq <= out_sq && x >= x_check)
                feedback &= draw_dot(x_check, y_check);
        }      
    }
	return feedback;
}



/* 
 *  draw_left_semicircle
 *	 
 *	 
 *	
 *	
 * INPUTS: F1_x,F1_y, F2_x, F2_y -- the position of two focus points of a ellipse
 *         a -- the long radius of ellipse
 * OUTPUTS: draws a pixel to every point whose sum of the distances to two focus points
 *         is two times of a, the long radius of ellipse
 * RETURN VALUE: 0 if any of the pixels drawn are out of bounds, otherwise 1
 * SIDE EFFECTS: none
 */
int32_t
draw_ellipse(int32_t F1_x, int32_t F1_y, int32_t F2_x, int32_t F2_y, int32_t a){
    int32_t xe, ye;
    int32_t feedback = 1;
    double  dis1, dis2, da;                     // I apply sqrt() in math.h to calculate the distance, so I set 
                                                // the distances' type to double

    for (xe = 190; xe <= 430; xe++){
        for (ye = 40; ye <= 280; ye++){
            dis1 = sqrt((xe - F1_x)*(xe - F1_x) + (ye - F1_y)*(ye - F1_y));
            dis2 = sqrt((xe - F2_x)*(xe - F2_x) + (ye - F2_y)*(ye - F2_y));
            da = (int32_t) (dis1 + dis2);       // convert the resulting double number into integer;
            if (((da - 2*a <= 2 && da - 2*a >0) || (2*a - da > 0 && 2*a - da <= 2)))
                feedback &= draw_dot(xe, ye);
        }
        
    }
    return feedback;
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
	/* Your code goes here! */
    int32_t index, sign, col, feedback = 1;
    int32_t diff, color;                                // diff is used to calculate level_1 - level_2
    int32_t check = 0x000000FF;                         // check is used to AND with colors to get rgb;
    int32_t rgb_start[3], rgb_end[3], rgb_between[3];   /* rgb_start stores the start values of rgb
                                                           rgb_end stores the end values of rgb
                                                           rgb_between stores the middle values of rgb*/
    for (index = 0; index < 3; index++){
        rgb_start[index] = start_color & check;         // Use check to get rgb values    
        rgb_end[index] = end_color & check;
        start_color >>= 8;                              // right shift both colors;
        end_color >>= 8;
    }
       
    for (col = 0; col <= w; col++){
        for (index = 0, color = 0x00000000; index < 3; index++){
            diff = rgb_end[index] - rgb_start[index];

            if (diff) sign = (diff < 0)?-1:1;           // Check the sign
            else sign = 0;
            rgb_between[index] = ((2*col*diff + sign*w)/(2*w) + rgb_start[index]);
        }
        color = rgb_between[0] | (rgb_between[1] <<= 8) | (rgb_between[2] <<= 16);
        set_color(color);                           // color is calculated by ADD all three middle rgb values together;
        feedback &= draw_line(x + col, y, x + col, y + h);
    }  
	return feedback;
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


/* DESCRIPTION: this function draws the logo of NASA, the National Aeronautics and Space Administration
 *              by using the function written above. Specially, in order to finish this, I write another 
 *              three functions called draw_right_semicircle, draw_left_semicircle and draw_ellipse.
 * DETAILS: the detailed input, output and effects of three functions have been listed before the funtions
 *     
 */
int32_t
draw_picture(){
	/* Your code goes here! */
    //WIDTH = 624, HEIGHT = 320;
    const int32_t x_center = 310, y_center = 160;                        // The center of the logo is at (310, 160)
    int32_t feedback = 1;                                                // feedback is the return value of draw_pictire
    int32_t x_color1, y_color1, x_color2, y_color2, counter_x, counter_y;

// Set background;
    rect_gradient(0, 0, WIDTH, HEIGHT, 0x00000000, 0x00000000);             // Make the background black
// Draw the circle at the bottom;
    set_color(0x001A5DAD);                                                  
    feedback &= draw_circle(x_center, y_center, 0, 120);
// Draw the first red curve;
    set_color(0x00EE293D);
    draw_circle(150, -151, 352, 359);
    rect_gradient(0, 0, 170, HEIGHT, 0x00000000, 0x00000000);
    rect_gradient(430, 0, WIDTH-430, HEIGHT, 0x00000000, 0x00000000);

// Draw 'N'
    rect_gradient(210, 143, 5, 43, 0x00FFFFFF, 0x00FFFFFF);
    set_color(0x00FFFFFF);
    draw_parallelogram(207, 143, 228, 143, 256, 186);                       // Use two parallelogram to consist the 'N'
    for (x_color1 = 207, y_color1 = 143, counter_x = 0; counter_x <= 21; counter_x++){
        for (x_color2 = 235, y_color2 = 186, counter_y = 0; counter_y <= 21; counter_y++){
            draw_line(x_color1+counter_x, y_color1, x_color2+counter_y, y_color2);
        }       
    }

    draw_triangle(207, 186, 218, 186, 212, 178);
    draw_dot(207, 185);
    draw_dot(208, 185);
    draw_dot(209, 185);
    draw_dot(209, 184);
    draw_dot(216, 185);
    draw_dot(216, 184);
    draw_dot(215, 185);
    draw_dot(215, 184);
    draw_dot(215, 183);

    rect_gradient(248, 143, 5, 43, 0x00FFFFFF, 0x00FFFFFF);
    draw_triangle(245, 143, 256, 143, 250, 151);
    draw_dot(245, 144);
    draw_dot(246, 144);
    draw_dot(247, 144);
    draw_dot(247, 145);
    draw_dot(254, 144);
    draw_dot(253, 144);
    draw_dot(254, 145);
    draw_dot(253, 145);
    draw_dot(253, 146);

// Draw 'A'
    draw_parallelogram(275, 146, 280, 146, 271, 186);                       // Use loops to fill the parallelogram using draw_line function
    for (x_color1 = 275, y_color1 = 146, counter_x = 0; counter_x <= 5; counter_x++){
        for (x_color2 = 266, y_color2 = 186, counter_y = 0; counter_y <= 5; counter_y++){
            draw_line(x_color1 + counter_x, y_color1, x_color2 + counter_y, y_color2);
        }
    }
    draw_parallelogram(271, 143, 293, 143, 313, 186);
    for (x_color1 = 271, y_color1 = 143, counter_x = 0; counter_x <= 22; counter_x++){
        for (x_color2 = 291, y_color2 = 186, counter_y = 0; counter_y <= 22; counter_y++){
            draw_line(x_color1 + counter_x, y_color1, x_color2 + counter_y, y_color2);
        }
    }
    rect_gradient(273, 174, 14, 5, 0x00FFFFFF, 0x00FFFFFF);                //Loops below are all used to fill a certain shape with color using 
                                                                           //   draw_line functions
// Draw 'S'
    draw_right_semicircle(337, 174, 3, 14);
    draw_left_semicircle(333, 154, 3, 14);
    rect_gradient(333, 140, 14, 9, 0x00FFFFFF, 0x00FFFFFF);
    rect_gradient(327, 179, 10, 9, 0x00FFFFFF, 0x00FFFFFF);
    rect_gradient(333, 157, 2, 11, 0x00FFFFFF, 0x00FFFFFF);
    rect_gradient(335, 159, 2, 11, 0x00FFFFFF, 0x00FFFFFF);
    draw_parallelogram(320, 176, 327, 179, 327, 187);
    for (x_color1 = 320, y_color1 = 176, counter_x = 0; counter_x <= 9; counter_x++){
        for (x_color2 = 327, y_color2 = 179, counter_y = 0; counter_y <= 9; counter_y++){
            draw_line(x_color1, y_color1+counter_x, x_color2, y_color2+counter_y);
        }
        
    }
    draw_parallelogram(347, 140, 350, 143, 350, 152);
    for (x_color1 = 347, y_color1 = 140, counter_x = 0; counter_x <= 9; counter_x++){
        for (x_color2 = 350, y_color2 = 143, counter_y = 0; counter_y <= 9; counter_y++){
            draw_line(x_color1, y_color1+counter_x, x_color2, y_color2+counter_y);
        }
        
    }

// Draw 'A'
    draw_parallelogram(369, 146, 374, 146, 365, 186);
    for (x_color1 = 369, y_color1 = 146, counter_x = 0; counter_x <= 5; counter_x++){
        for (x_color2 = 360, y_color2 = 186, counter_y = 0; counter_y <= 5; counter_y++){
            draw_line(x_color1 + counter_x, y_color1, x_color2 + counter_y, y_color2);
        }
    }
    draw_parallelogram(365, 143, 387, 143, 407, 186);
    for (x_color1 = 365, y_color1 = 143, counter_x = 0; counter_x <= 22; counter_x++){
        for (x_color2 = 385, y_color2 = 186, counter_y = 0; counter_y <= 22; counter_y++){
            draw_line(x_color1 + counter_x, y_color1, x_color2 + counter_y, y_color2);
        }
    }
    rect_gradient(367, 174, 14, 5, 0x00FFFFFF, 0x00FFFFFF);  

// Draw the second red curve;
    set_color(0x00EE293D);
    feedback &= draw_circle(WIDTH + 255, HEIGHT + 420, 794, 797);
    rect_gradient(0, 0, 170, HEIGHT, 0x00000000, 0x00000000);
    rect_gradient(430, 0, WIDTH-430, HEIGHT, 0x00000000, 0x00000000);

    draw_triangle(435, 59, 412, 96, 480, 59);
    for (x_color1 = 435, counter_x = 0; counter_x <= 50; counter_x++){
        draw_line(412, 97, x_color1+counter_x, 59);
    }
    

// Draw the ellipse;
    set_color(0x00FFFFFF);
    draw_ellipse(280, 110, 340, 210, 73);
    rect_gradient(310, 200, 30, 50, 0x001A5DAD, 0x001A5DAD);
    set_color(0x00FFFFFF);
    draw_circle(310, 220, 0, 4);
// Draw seprate dot in low semi circle;
    draw_circle(263, 192, 0, 2);
    draw_circle(264, 212, 0, 2);
    draw_circle(269, 230, 0, 2);
    draw_circle(254, 228, 0, 2);
    draw_circle(254, 202, 0, 2);
    draw_circle(248, 252, 0, 2);
    draw_circle(240, 250, 0, 2);
    draw_circle(278, 230, 0, 2);
    draw_circle(310, 250, 0, 2);
    draw_circle(320, 230, 0, 2);
    draw_circle(336, 210, 0, 2);
    draw_circle(340, 218, 0, 2);
    draw_circle(344, 213, 0, 2);
    draw_circle(378, 214, 0, 2);
    draw_circle(396, 214, 0, 3);
    draw_circle(382, 222, 0, 2);
    draw_circle(372, 210, 0, 2);
    draw_circle(350, 225, 0, 2);
// Draw seprate dor in upper semi circle;
    draw_circle(310, 80, 0, 2);
    draw_circle(340, 60, 0, 3);
    draw_circle(340, 80, 0, 2);
    draw_circle(310, 100, 0, 2);
    draw_circle(295, 85, 0, 2);
    draw_circle(275, 115, 0, 2);
    draw_circle(270, 100, 0, 2);
    draw_circle(275, 130, 0, 2);
    draw_circle(285, 110, 0, 2);


	return feedback;
}
