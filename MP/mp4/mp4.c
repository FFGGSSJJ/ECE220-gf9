
/* 
Student:        Guanshujie Fu; 
ID Number:      3190110666
Homework:       Machine Problem 4
Creation Date:  2020.10.19
File Name:      mp4.c
Description:
This program determines which pixels in each row to be black 
    in Nonogram with the parameters: width, r1, r2, r3, r4, r5.
    And print out this row with "*" stands for black and "-" stands
    for white. (But in mp4.h it indicates that "." should be used to
    indicate white.) 
For this code, I apply array to store the correspnding information of 
    each row with 0 indicates white pixel and 1 indicates black pixel.
*/

#include<stdio.h>
#include"mp4.h"

int32_t print_row(int32_t width, int32_t r1, int32_t r2, int32_t r3, int32_t r4, int32_t r5);

int32_t print_row(int32_t width, int32_t r1, int32_t r2, int32_t r3, int32_t r4, int32_t r5) {
    int total_region, standard_region, region_num, difference;
    int counter, index_1, index_2;                                      /* Counter, index_1, index_2 are used as counter in loop and index for array*/
    int region[5];                                                      /* region is used to store the size of each region*/
    int standard[50];                                                   /* I want to use array to indicates the max total regions 
                                                                            and store the order of location of pixel for each row*/
    region[0] = r1;                                     // Store each region size into array;
    region[1] = r2;
    region[2] = r3;
    region[3] = r4;
    region[4] = r5;

    for (counter = 0, region_num = 0; counter < 5 ; counter++)
    {
        if (region[counter])                            // Calculate the region_num
            region_num++;                               // region_num indicate the number of regions that is larger than 0;
        else                                            
            continue;
    }
    for (counter = 0; counter < 50; counter++)
        standard[counter] = 0;                          // Set all  elements in array to be 0 at first;

    total_region = r1 + r2 + r3 + r4 + r5;              // total region indicates the total black pixels;
    region_num -= 1;                                    // region_num - 1 indicates the spaces needed;
    standard_region = total_region + region_num;        // standard_region indicate the number of both * and . when it fits;
    difference = width - standard_region;               // difference indicates the how many extra spaces are added;
 
    if (difference < 0)                                 // If difference is less than 0, it cannot print out a row and return 0;
        return 0;
    else
    {
        index_2 = 1;                                    
        for (index_1 = 0; index_1 <= 4; index_1++)      // First indicates the standard combination of the regions, such as 4 2 1 0 0 0 or 5 2 2 0 0 0;
        {                                               // index_1 is the index of region array;
            if (region[index_1] > 0)                    // index_2 is the index of standard array, storing starts at index = 1;
            {                                           // counter is used to indicates the size of each region;
                counter = region[index_1];
                while (counter)
                {
                    standard[index_2] = 1;
                    index_2++;
                    counter--;
                }
                standard[index_2] = 0;
                index_2++;
            }
            else
                continue;                               // When certain region size is 0, pass it;
        }

        if (difference > 0)                             // when difference is bigger than 0, it indicates that it allows more than one space to exist between each regions
        {                                               // when difference is 0, just print the row according to the standard array;
            for ( counter = difference; counter > 0; counter--)             // counter indicates the times of change need to be taken according to value of difference;
            {                                                               // index_2 is the index of standard array;
                for (index_2 = 0; index_2 < width+1; index_2++)
                {
                    if (standard[index_2] || (standard[index_2] == 0 && standard[index_2+1] == 0))
                        continue;                                           // If the current bit is 1 or the current bit is 0 while the next bit is also 0,
                    if (~standard[index_2] && standard[index_2+1])          // ignore this and come to next index;
                        standard[index_2+1] = 0;                            // If the current bit is 0 while the next bit is 1, change current bit to 0;
                    index_2++; 
                }
            }
        }

        for (counter = 1; counter < width+1; counter++)                     // Loop through the standard array and print out the row;
        {
            if (standard[counter])
                printf("*");
            else
                printf("-");
        } 
        printf("\n");
        return 1; 
    }
}