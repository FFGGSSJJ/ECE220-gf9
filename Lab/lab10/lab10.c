#include <stdint.h>
#include <stdio.h>

#include "lab10.h"

void 
calculate_hashes (int32_t hashes[], char* words[])
{
    // write your code here
    int32_t hash;
    int32_t count, N;

    for (N = 0, count = 0; N < 10; N++){
        hash = hashes[N];
        for (int i = 0; i < NUM_WORDS; i++){
            for (int j = 0; words[i][j] != '\0'; j++){
                hash = (hash << (N + 2))^(words[i][j]);
            }
        }
        hashes[N] = (hash < hashes[N])?hash:hashes[N];
    }


}

