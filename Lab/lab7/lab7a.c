#include <stdint.h>
#include <stdio.h>


static void
show_mapping (int32_t A[3][5][7], int32_t* B)
{
    printf ("A = %p, B = %p\n\n", A, B);
}


int
main ()
{
    int32_t array[3][5][7];
    int32_t* pointer = &array[0][0][0];
    int32_t i, j, k;
    show_mapping (array, (int32_t*)array);

    for  (i = 0; i < 105; i++){
        *(pointer + i) = i;
    }
    
    for ( i = 0; i < 3; i++){
        for (j = 0; j < 5; j++){
            for (k = 0; k< 7; k++){
                printf("%3d\n", array[i][j][k]);
            }            
        }
    }
    
    
    return 0;
}


