#include <stdint.h>
#include <stdio.h>

#include "mp5.h"
#include "mp10.h"
#include <stdlib.h>

#define MY_INFINITY   1000000000


/*
 *  Homework: Machine Problem 9
 *  Student: Guanshujie Fu
 *  File Name: mp9.c
 *  Description:    with the use of dijkstra algrothim and 
 *                  heap, this code could search for the shortest path
 * 
 *  Notation: I modified the vertex_t structure as PDF said it is ok to add
 *            field to it to implement my dijkstra.
 */


/*
 * initialize_heap
 * 
 * DESCRIPTION: initialize the heap such that it contains only the id of staring vertex
 *              at the begining. Meanwhile, initialize the information of starting vertex
 *              such as from_src, pred.
 * 
 * INPUTS: g ---- the structure pointer of the graph
 *         h ---- the structure pointer of the heap
 *         src ---- the structure pointer of the starting vertex
 * OUTPUT: the initialized heap
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
initialize_heap(graph_t* g, heap_t* h, vertex_set_t* src);


/*
 * pop_heap_vertex
 * 
 * DESCRIPTION: pop an element from the heap and change the heap_id of other
 *              vertex in the heap
 * 
 * INPUT: h ---- the heap structure pointer
 * OUTPUT: the heap after popping an element 
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void
pop_heap_vertex(heap_t* h);

/*
 * swap
 * 
 * DESCRIPTION: swap two elements and their heap_id
 * INPUT: a---- pointer to first element
 *        b ---- pointer to second element
 *        g ---- the graph pointer
 * OUTPUT: none
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void
swap(int32_t* a, int32_t* b, graph_t* g);

/*
 * heapify
 * 
 * DESCROPTION: use heap to search for the smallest element
 * INPUT: h ---- the heap pointer
 *        g ---- the graph pointer
 *        i ---- index
 *        n ---- size of array
 * OUTPUT: the heap with smallest element at the top
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void 
heapify(heap_t* h, graph_t* g, int32_t i, int32_t n);

void
sort_insert(vertex_set_t* vs, int32_t id);




void
find_nodes (locale_t* loc, vertex_set_t* vs, pyr_tree_t* p, int32_t nnum)
{
    // Do not modify the following line nor add anything above in the function.
    record_fn_call ();
    
    int32_t x_r, x_l, y_u, y_d;
    //int32_t y_max, y_min;
    int32_t* temp_ptr;

    x_r = loc->x + loc->range;
    x_l = loc->x - loc->range;
    y_d = loc->y - loc->range;
    y_u = loc->y + loc->range;
    //y_max = (p->node[nnum].y_left > p->node[nnum].y_right) ? p->node[nnum].y_left : p->node[nnum].y_right;
    //y_min = (p->node[nnum].y_left < p->node[nnum].y_right) ? p->node[nnum].y_left : p->node[nnum].y_right;


    if (4*nnum + 1 >= p->n_nodes){       // if 4*nnum+1 > n_nodes, current nnum is at leaf nodes          
        if (in_range(loc, p->node[nnum].x, p->node[nnum].y_left)){     // check if it is in the range
        //************************* mp10 **************************************
            if (vs->id_array_size <= vs->count) {
                temp_ptr = realloc(vs->id, 2*vs->id_array_size*sizeof(int32_t));
                if (temp_ptr == NULL)   return;
                vs->id = temp_ptr;
                vs->id_array_size *= 2;     // allocate the space of a factor of 2
            }
            sort_insert(vs, p->node[nnum].id);
        //*********************************************************************
        } 
    } else if (x_r < p->node[nnum].x){      // if the circle falls in left side
        if (y_d > p->node[nnum].y_left || (y_d < p->node[nnum].y_left && y_u > p->node[nnum].y_left)) {
            find_nodes(loc, vs, p, 4*nnum+3);
        }
        if (y_u < p->node[nnum].y_left || (y_d < p->node[nnum].y_left && y_u > p->node[nnum].y_left)) {
            find_nodes(loc, vs, p, 4*nnum+1);
        }
    } else if (x_l > p->node[nnum].x){      // if the circle falls in right side
        if (y_d > p->node[nnum].y_right || (y_d < p->node[nnum].y_right && y_u > p->node[nnum].y_right)) {
            find_nodes(loc, vs, p, 4*nnum+4);
        }
        if (y_u < p->node[nnum].y_right || (y_d < p->node[nnum].y_right && y_u > p->node[nnum].y_right)) {
            find_nodes(loc, vs, p, 4*nnum+2);
        }
    } else if (y_u > p->node[nnum].y_right && y_d > p->node[nnum].y_left && y_d < p->node[nnum].y_right && x_l < p->node[nnum].x && x_r > p->node[nnum].x) {
        find_nodes(loc, vs, p, 4*nnum+2);   // if falls in region N+2 and 3 and 4
        find_nodes(loc, vs, p, 4*nnum+3);
        find_nodes(loc, vs, p, 4*nnum+4);
    } else if (y_d < p->node[nnum].y_left && y_u < p->node[nnum].y_right && y_u > p->node[nnum].y_left && x_l < p->node[nnum].x && x_r > p->node[nnum].x) {
        find_nodes(loc, vs, p, 4*nnum+1);   // if in N+1 and 2 and 3
        find_nodes(loc, vs, p, 4*nnum+2);
        find_nodes(loc, vs, p, 4*nnum+3);
    } else if (y_d > p->node[nnum].y_right && y_u < p->node[nnum].y_left && y_u > p->node[nnum].y_right && x_l < p->node[nnum].x && x_r > p->node[nnum].x) {
        find_nodes(loc, vs, p, 4*nnum+1);   // if in N+1 and 2 and 4
        find_nodes(loc, vs, p, 4*nnum+2);
        find_nodes(loc, vs, p, 4*nnum+4);
    } else if (y_u > p->node[nnum].y_left && y_d < p->node[nnum].y_left && y_d > p->node[nnum].y_right && x_l < p->node[nnum].x && x_r > p->node[nnum].x) {
        find_nodes(loc, vs, p, 4*nnum+1);   // if in N+1 and 3 and 4
        find_nodes(loc, vs, p, 4*nnum+3);
        find_nodes(loc, vs, p, 4*nnum+4);
    } /*else if (y_u < y_max && x_l < p->node[nnum].x && x_r > p->node[nnum].x) {
        find_nodes(loc, vs, p, 4*nnum+1);   // if in N+1 and 2
        find_nodes(loc, vs, p, 4*nnum+2);
    } else if (y_d > y_min && x_l < p->node[nnum].x && x_r > p->node[nnum].x) {
        find_nodes(loc, vs, p, 4*nnum+3);   // if in N+3 and 4
        find_nodes(loc, vs, p, 4*nnum+4);
    } else if (y_u < p->node[nnum].y_right && y_d > p->node[nnum].y_left && x_l < p->node[nnum].x && x_r > p->node[nnum].x) {
        find_nodes(loc, vs, p, 4*nnum+2);   // if between y_right and y_left
        find_nodes(loc, vs, p, 4*nnum+3);
    } else if (y_d > p->node[nnum].y_right && y_u < p->node[nnum].y_left && x_l < p->node[nnum].x && x_r > p->node[nnum].x) {
        find_nodes(loc, vs, p, 4*nnum+1);
        find_nodes(loc, vs, p, 4*nnum+4);
    } */else {
        find_nodes(loc, vs, p, 4*nnum+1);
        find_nodes(loc, vs, p, 4*nnum+2);
        find_nodes(loc, vs, p, 4*nnum+3);
        find_nodes(loc, vs, p, 4*nnum+4);
    } return;
}

void
sort_insert(vertex_set_t* vs, int32_t id) 
{
    int32_t position = 0;
    for (int i = 0; i < vs->count; i++){    // just loop to find the position
        if (vs->id[i] < id){
            position++;
        } else{break;}
    }
    for (int i = vs->count; i > position; i--){
        vs->id[i] = vs->id[i-1];
    }
    vs->id[position] = id;
    vs->count++;
}

void
trim_nodes (graph_t* g, vertex_set_t* vs, locale_t* loc)
{
    int32_t check, counter = 0;
    int32_t inrange[vs->count];

    for (int i = 0; i < vs->count; i++){    // i is the index of vs->id
        check = in_range(loc, g->vertex[vs->id[i]].x, g->vertex[vs->id[i]].y);
        if (check){
            inrange[counter] = vs->id[i];
            counter += 1;
        }
    }
    vs->count = counter;    // load the number of vertex
    for (int i = 0; i < counter; i++){
        vs->id[i] = inrange[i];
    }
}




/* *************************** 2020.12.13 ***************************************
 * NOTATION: I use Hanwen Liu (3191110665)'s dijkstra function in mp10
 *           with his permission.
 *           And modify it for mp10 by myself as you can indicate.
 * ****************************************************************************/

void heap_ini(graph_t* g, heap_t* h, vertex_set_t* src);
int32_t heap_pop(graph_t* g, heap_t* h);

int32_t
dijkstra (graph_t* g, heap_t* h, vertex_set_t* src, vertex_set_t* dest,
          path_t* path)
{
        int32_t i, z, p, gid, dis, min_n;
        // i is the cyclic variable and z is the poped result
        // p is the pointer in the heap pointing to the node that is handling
        // gid is the vertex id of the modified node and dis is its distance from the source
        // min_n is the vertex that is the end point of the shortest path
        heap_ini(g, h, src);
        // heap initialization
        while (h->n_elts > 0){
                // pop the smallest element
                z = heap_pop(g, h);
                for (i = 0; i < g->vertex[z].n_neighbors; ++i){ // check every neighbors if its shortest path can be shorter
                        if (g->vertex[z].from_src + g->vertex[z].distance[i] < g->vertex[g->vertex[z].neighbor[i]].from_src){
                                // if it can be shorter, update relevant data and update the heap.
                                gid = g->vertex[z].neighbor[i];
                                // Store the vertex # as gid
                                dis = g->vertex[z].from_src + g->vertex[z].distance[i];
                                g->vertex[gid].from_src = dis;
                                g->vertex[gid].pred = z;
                                p = g->vertex[gid].heap_id;
                                // point p to the shortened node
                                while (p > 0){
                                        if (dis < g->vertex[h->elt[(p - 1)>> 1]].from_src){
                                                // if its distance from source is shorter than its father node's
                                                // swap itself and its father
                                                h->elt[p] = h->elt[(p - 1)>> 1];
                                                g->vertex[h->elt[p]].heap_id = p;
                                                p = (p - 1)>> 1;
                                        }else{
                                                break;
                                        }
                                }h->elt[p] = gid;
                                g->vertex[gid].heap_id = p;     // point it back.
                        }
                }
        }min_n = dest->id[0];   // start to find the shortest distance and its end point


        path->tot_dist = g->vertex[dest->id[0]].from_src;
        for (i = 1; i < dest->count; ++i){
                if (g->vertex[dest->id[i]].from_src < path->tot_dist){
                        path->tot_dist = g->vertex[dest->id[i]].from_src;
                        min_n = dest->id[i];
                }
        }if (g->vertex[min_n].pred == -1) return 0;     // If all destination is unreachable, return 0
        //********************************* mp10 modified ********************************************
        int32_t temp = min_n, count = 1;
        path->n_vertices = 1;
        while (g->vertex[temp].pred != -2 ){
            temp = g->vertex[temp].pred;
            ++count;
        }   // calculate the number of vertex
        path->id = realloc(path->id, count*sizeof(*(path->id)));
        if (path->id == NULL) return 0;
        path->id[0] = min_n;
        while (g->vertex[path->id[path->n_vertices - 1]].pred != -2){   // Repeat until meet the starting point
                path->id[path->n_vertices] = g->vertex[path->id[path->n_vertices - 1]].pred;
                ++path->n_vertices;     // Record the path
        }
        //**********************************************************************************************
        for (i = 0; i < path->n_vertices >> 1; ++i){   // Reverse the path, since path->id[0] record the end point
                z = path->id[i];
                path->id[i] = path->id[path->n_vertices - 1 - i];
                path->id[path->n_vertices - 1 - i] = z;
        }return 1;      // All success!
}

/* 
 *
 * heap_ini     -- push all points to the heap according to the distance from
 *                 the source and initialize the graph.
 * 
 * INPUTS: g    -- the graph
 *         src  -- the src vertex set
 * OUTPUTS: g   -- initialized graph
 *          h   -- initialized heap
 * RETURN VALUE: none
 * SIDE EFFECTS: none
 */
void heap_ini(graph_t* g, heap_t* h, vertex_set_t* src){
        int i, p;       // i is the cycle variable, p points at the node in the heap that we are dealing with
        h->n_elts = g->n_vertices;      // the number of elements in heap is equal to the number of vertex in graph
        for (i = 0; i < g->n_vertices; ++i){
                g->vertex[i].pred = -1;                 // all pred should to -1 except for the source vertexs
        }for (p = 0; p < src->count; ++p){
                h->elt[p] = src->id[p];
                g->vertex[h->elt[p]].heap_id = p;
                g->vertex[h->elt[p]].from_src = 0;      // the distance of source vertex is a trivial 0.
                g->vertex[h->elt[p]].pred = -2;         // the pred of source vertex is -2
        }for(i = 0; p < g->n_vertices; ++i){
                if (g->vertex[i].pred == -1){
                        h->elt[p] = i;
                        g->vertex[h->elt[p]].heap_id = p;
                        g->vertex[h->elt[p]].from_src = MY_INFINITY;
                        ++p;
                }
        }return;
}

/* 
 *
 * heap_pop     -- pop the smallest element
 * 
 * INPUTS: g    -- the graph
 *         h    -- the heap
 * OUTPUTS: h   -- poped heap
 * RETURN VALUE: the vertex # of the poped node.
 * SIDE EFFECTS: none
 */
int32_t heap_pop(graph_t* g, heap_t* h){
        int32_t p, dis, gid, rst = h->elt[0];
        // p points to the current position of the last node in the heap
        // dis is the distance from the source of the last node in the heap
        // gid is the vertex # of the last node in the graph
        // rst is the pop result
        --(h->n_elts);  // size of heap minus 1
        gid = h->elt[h->n_elts];
        dis = g->vertex[gid].from_src;
        p = 0;  // point p to the root
        while (2 * p + 1 < h->n_elts){          // if p has a left child
                if (2 * p + 2 < h->n_elts){     // if p has a right child
                        if (g->vertex[h->elt[2 * p + 1]].from_src < g->vertex[h->elt[2 * p + 2]].from_src){
                                // compare the distances from source of left child and right child
                                if (g->vertex[h->elt[2 * p + 1]].from_src < dis){
                                        // compare the distance of left child and "the last node"
                                        h->elt[p] = h->elt[2 * p + 1];
                                        g->vertex[h->elt[p]].heap_id = p;
                                        p = 2 * p + 1;
                                }else{
                                        break;  // correct position
                                }
                        }else{
                                if (g->vertex[h->elt[2 * p + 2]].from_src < dis){
                                        // compare the distance of right child and "the last node"
                                        h->elt[p] = h->elt[2 * p + 2];
                                        g->vertex[h->elt[p]].heap_id = p;
                                        p = 2 * p + 2;
                                }else{
                                        break;  // correct position
                                }
                        }
                }else{
                        // have no right child
                        if (g->vertex[h->elt[2 * p + 1]].from_src < dis){
                                h->elt[p] = h->elt[2 * p + 1];
                                g->vertex[h->elt[p]].heap_id = p;
                                p = 2 * p + 1;
                        }else{
                                break;  // correct position
                        }
                }
        }h->elt[p] = gid;
        g->vertex[gid].heap_id = p;     // Update the heap and the graph
        return rst; // pop the result
}
















