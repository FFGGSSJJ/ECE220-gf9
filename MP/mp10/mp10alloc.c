#include <stdint.h>
#include <stdlib.h>

#include "mp10.h"


vertex_set_t*
new_vertex_set ()
{
    vertex_set_t* temp;
    temp = malloc(sizeof(vertex_set_t));
    if (temp == NULL)   return NULL;    // if allocate failed, return NULL
    temp->count = 0;
    temp->minimap = 0;
    temp->id_array_size = 1;
    temp->id = malloc(sizeof(int32_t));
    return temp;        // return the pointer if succeeds
}


void
free_vertex_set (vertex_set_t* vs)
{
    free(vs->id);
    free(vs);
}


path_t*
new_path ()
{
    path_t* temp;
    temp = malloc(sizeof(path_t));
    if (temp == NULL)   return NULL;    // if allocate failed, return NULL;
    temp->minimap = 0;
    temp->n_vertices = 0;
    temp->id = malloc(sizeof(int32_t));
    return temp;    // return the pointer
}


void
free_path (path_t* path)
{
    free(path->id);
    free(path);
}

