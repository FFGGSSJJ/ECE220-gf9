#include <stdint.h>
#include <stdio.h>

#include "mp5.h"
#include "mp9.h"

#define MY_INFINITY   1000000000

//heap initialization
/*
 * heap——init
 * 
 * DESCRIPTION: initialize the heap such that it contains the id of all the vertex
 *              Also,initialize the from_src information of starting vertex
 *              and heap_id, from_src information of other vertex.
 * 
 * INPUTS: g ---- pointer of the graph
 *         h ---- pointer of the heap
 *         src ---- pointer of the starting vertex
 * OUTPUT: the initialized heap
 * 
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void
heap_init(graph_t* g, heap_t *h, vertex_set_t* src);
//heap poping
/*
 * heap——remove
 * 
 * DESCRIPTION: remove the current index from the heap such that it contains the vertex has unvisited neighbors
 *              Also, change the n_elts and heap_id in vertex behind the current index
 * 
 * INPUTS: g ---- pointer of the graph
 *         h ---- pointer of the heap
 *         min_id ---- the current index
 * OUTPUT: the latest heap
 * 
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void
heap_remove(graph_t* g, heap_t *h, int32_t min_id);

/*
The function walk the pyramid tree and find any nodes within range of a specified locale.
start at array index nnum and walk the pyramid tree recursively in order to fill the vertex set vs 
with the array indices of any graph vertices found to be in range of locale loc. 
*/
void
find_nodes (locale_t* loc, vertex_set_t* vs, pyr_tree_t* p, int32_t nnum)
{
    // Do not modify the following line nor add anything above in the function.
    record_fn_call ();

    if((4*nnum+1) >= p->n_nodes)
    {
        //Use the in_range function to check whether a leaf node (a graph vertex) is in range of the given locale.
        if(in_range(loc, p->node[nnum].x, p->node[nnum].y_left))
        {
            vs->id[vs->count] = p->node[nnum].id;
            vs->count++;
        }
    }
    else
    {
        //function to walk the pyramid tree and find any nodes within range of a specified locale.
        find_nodes (loc, vs, p, 4*nnum+1);
        find_nodes (loc, vs, p, 4*nnum+2);
        find_nodes (loc, vs, p, 4*nnum+3);
        find_nodes (loc, vs, p, 4*nnum+4);
    }

}

/*
The remove any graph vertices that are not in range of a locale from a vertex set
*/
void
trim_nodes (graph_t* g, vertex_set_t* vs, locale_t* loc)
{
    int32_t i, j, k;
    int32_t count = 0;
     
   for(i = 0; i <= vs->count-1; i++)
    {
        if(in_range(loc, g->vertex[vs->id[i]].x, g->vertex[vs->id[i]].y))
        {
            //calculate the number of vetex in both ranges
            count++;
        }
    }

    for(j = 0, k = 0; j <= vs->count-1; j++)
    {
        if(in_range(loc, g->vertex[vs->id[j]].x, g->vertex[vs->id[j]].y))
        {
            //store the vetex into vs until the number reach count value
            vs->id[k] = vs->id[j];
            k++;
        }
        if(k == count)
            break;
    }

    vs->count = count;
}


/*
Dijkstra routine write the shortest path found between any pair of vertices in the src 
and dest vertex sets (respectively) into the path parameter. 
*/


int32_t
dijkstra (graph_t* g, heap_t* h, vertex_set_t* src, vertex_set_t* dest,
          path_t* path)
{
    //call function to initialize the heap
    heap_init(g , h , src );

    int32_t i,j;
    int32_t min_id,min_dist;
    int32_t dest_id,dest_dist,current_id;
    int32_t cur_dist, nei_dist, bet_dist, nei_heap_id;
    

    while(h->n_elts > 0)        //do while the heap is not empty
    {
        min_dist = g->vertex[h->elt[0]].from_src;
        min_id = 0;

        for(i = 0; i < h->n_elts; i++)     //walk the heap to find the vertex that closet to source and store its id and dist
        {
            if(min_dist > g->vertex[h->elt[i]].from_src)
            {
                min_dist = g->vertex[h->elt[i]].from_src;
                min_id = i;
            }
        }

        cur_dist = g->vertex[h->elt[min_id]].from_src;      //the min dist from the current vertex to sourcr

        for(j = 0; j <= g->vertex[h->elt[min_id]].n_neighbors-1; j++)
        {
            nei_dist = g->vertex[g->vertex[h->elt[min_id]].neighbor[j]].from_src;       //neighbor's min distance from source
            nei_heap_id = g->vertex[g->vertex[h->elt[min_id]].neighbor[j]].heap_id;     //neighbor's heap_id
            bet_dist = g->vertex[h->elt[min_id]].distance[j];       //dist between current vertex and neighbor's

            if(cur_dist == 0 && nei_dist == 0 )     //both vertex are starting points
                continue;
            
            if(nei_heap_id == -1)       //neighbot has been popped
                continue;

            if(cur_dist + bet_dist >= nei_dist)     //neighbor's min distance from source is shorter or equal to current one
                continue;

            //when neighbor's min distance from source is longer than the current one, change its min dist and pred
            g->vertex[g->vertex[h->elt[min_id]].neighbor[j]].from_src = cur_dist + bet_dist;    
            g->vertex[g->vertex[h->elt[min_id]].neighbor[j]].pred = h->elt[min_id];
        }

        //After walk all the neighbors, call remove function to pop the current index
        heap_remove(g, h, min_id);
    }
    

    dest_dist = MY_INFINITY;
    dest_id = 0;

    for(i = 0; i <= dest->count-1; i++)     //find the smallest distance needed to reach the ending point and the point
    {           
       if(dest_dist > g->vertex[dest->id[i]].from_src)
        {
            dest_dist = g->vertex[dest->id[i]].from_src;
            dest_id = dest->id[i];
        }            
    }
    
    path->tot_dist = dest_dist;
    current_id = dest_id;
    path->n_vertices = 1;

    while(g->vertex[current_id].from_src > 0)       //count the vertices needed from star to end
    {
        current_id = g->vertex[current_id].pred;
        path->n_vertices++;
    }

    //if the path requires more than MAX_PATH_LENGTH nodes (counting both the starting and ending nodes).
    if(path->n_vertices > MAX_PATH_LENGTH)
        return 0;
    
    for(i = 0, current_id = dest_id; i <= path->n_vertices-1; i++)
    {
        path->id[i] = current_id;
        current_id = g->vertex[current_id].pred;
    }

    //Return 0, if the source node set or the destination node set is empty.
    if(src->count == 0 || dest->count == 0 )
        return 0;
   
    //The function return 1 if a path is found that can fit into the path structure. 
    return 1;
}


void
heap_init(graph_t* g, heap_t *h, vertex_set_t* src)
{
    int32_t i,j;

    //set the verteies' heap id to vertex id and distance from source to infinity
    for(i = 0, h->n_elts = 0; i <= g->n_vertices-1; i++)
    {
        h->elt[i] = i; //
        h->n_elts++;
        g->vertex[i].heap_id = i;
        g->vertex[i].from_src = MY_INFINITY;
    }

    //set the starting points' distance from source to 0
    for(j = 0; j <= src->count-1; j++)
    {  
        g->vertex[src->id[j]].from_src = 0;    
    }

}

void
heap_remove(graph_t* g, heap_t *h, int32_t min_id)
{
    int32_t i;
    //decrese the number and pop the current index
    h->n_elts--;
    g->vertex[h->elt[min_id]].heap_id = -1;
    //move the behind elements in the heap to the front one
    for(i = min_id; i <= h->n_elts-1; i++)
    {
        h->elt[i] = h->elt[i+1];
        g->vertex[h->elt[i]].heap_id--;
    }
}