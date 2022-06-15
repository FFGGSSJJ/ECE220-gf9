#include <stdint.h>
#include <stdio.h>

#include "mp10.h"

//void
//search_nodes(graph_t* g, pyr_node_t* p, int32_t nnum, int32_t ancestor);
void search_leaf(int32_t ancestor,int32_t nnum,graph_t* g, pyr_tree_t* p);


// Marks each graph vertex in g with a minimap bit number based on the
// pyramid tree p.
int32_t 
mark_vertex_minimap (graph_t* g, pyr_tree_t* p)
{
    if(g->n_vertices <= 64){    // if nodes is less than 64, simply add index as mm_bit
        for(int i = 0 ;i < g->n_vertices; i++){
            g->vertex[i].mm_bit = i;
        }
    } else {    // loop through all nodes
        for(int i = 21; i <= 84; i++){
            search_leaf(i, i , g, p);   // I apply same recursion as used in find_nodes function
        }
    }
    return 1;
}

// a subroutine used for search leaf nodes
void search_leaf(int32_t ancestor,int32_t nnum,graph_t* g, pyr_tree_t* p){
    if(nnum >= p->n_nodes)    return;
    if(4*nnum + 1 >= p->n_nodes){
        g->vertex[p->node[nnum].id].mm_bit = ancestor - 21;
        return;
    }
    search_leaf(ancestor, 4*nnum+1, g, p);
    search_leaf(ancestor, 4*nnum+2, g, p);
    search_leaf(ancestor, 4*nnum+3, g, p);
    search_leaf(ancestor, 4*nnum+4, g, p);
}


void 
build_vertex_set_minimap (graph_t* g, vertex_set_t* vs)
{
    int32_t vertex_id;
    vs->minimap = 0;    
    for (int i = 0; i < vs->count; i++){
        vertex_id = vs->id[i];
        vs->minimap |= 1ULL << g->vertex[vertex_id].mm_bit;
    } 

}


void 
build_path_minimap (graph_t* g, path_t* p)
{
    int32_t path_id;
    p->minimap = 0;     // initialize minimap to 0
    for (int i = 0; i < p->n_vertices; i++){
        path_id = p->id[i];
        p->minimap |= 1ULL << g->vertex[path_id].mm_bit;
    } 
}

// Merges (intersects) two vertex sets into a third vertex set.  The
// third vertex set must have enough space in its array to hold 
// the resulting number of ids.
int32_t
merge_vertex_sets (const vertex_set_t* v1, const vertex_set_t* v2,
		   vertex_set_t* vint)
{
    uint64_t overlap = v1->minimap & v2->minimap;

    vint->count = 0;
    if (!overlap)  return 0;    // if overlap is 0, return 0
    if ((v1->id[0] > v2->id[v2->count - 1]) || (v1->id[v1->count - 1] < v2->id[0])){
        return 0;   // as the arrays are sorted, we can compare the end and start
    } else {
        for (int i = 0, k = 0; i < v1->count; i++){
            for (int j = k; j < v2->count; j++){
                if (v1->id[i] == v2->id[j]){ // loop to find same id
                    vint->id[vint->count] = v1->id[i];
                    vint->count++;
                    k = j;
                    break;
                }
            }
        } 
    }
    if (vint->count == 0)   return 0;   // if empty, return 0
    return 1;
}

