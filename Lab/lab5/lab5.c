#include <stdint.h>
#include <stdio.h>
#include "lab5.h"



/*
 * The fit_pieces function determines whether the "pieces" (bit patterns,
 * with some number of leading 0s) can "fit together" (line up with exactly
 * one 1 in any bit position, filling all 32 bits with 1s) when shifted
 * left by appropriate amounts (without overflow / loss of any 1 bits).
 *
 * If such shifts can be found, the function prints the left shift amounts
 * for each of the three pieces in order--p1, p2, and p3--then returns 1.
 * If no such shifts exist, the function returns 0.
 */
int32_t fit_pieces (uint32_t p1, uint32_t p2, uint32_t p3)
{
    // write your code here
    int shift_times;                 // Used for recored the number of bits shifted
    uint32_t s1, s2, s3;
    int max1, max2, max3;           // Used for calculate the max number of bits can be shifted
    uint32_t pieces[3] = {p1, p2 , p3};
    int shift[3];
    int index, index_2, index_3, temp_num;

    
    for (index = 0; index < 3; index++)
    {
        shift_times = 0;
        temp_num = pieces[index];
        while ((int32_t)temp_num > 0)
        {
            temp_num <<= 1;
            shift_times++;
        }
        shift[index] = shift_times;
    }

    max1 = shift[0];
    max2 = shift[1];
    max3 = shift[2];
    for (index = 0, s1 = p1; index < max1; index++)
    {
        for (index_2 = 0, s2 = p2; index_2 < max2; index_2++)
        {
            for (index_3 = 0, s3 = p3; index_3 < max3; index_3++)
            {
                if (((int32_t)(s1 | s2 | s3) == -1) && (s1 & s2 & s3 == 0))
                    return 1;
                else{
                    s3 <<= 1;
                }
                
            }
            s2 <<= 1;
        }
        s1 <<= 1;
    }
    // not possible
    return 0;
}

