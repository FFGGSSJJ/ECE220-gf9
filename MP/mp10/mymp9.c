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
    int32_t y_max, y_min;
    int32_t* temp_ptr;

    x_r = loc->x + loc->range;
    x_l = loc->x - loc->range;
    y_d = loc->y - loc->range;
    y_u = loc->y + loc->range;
    y_max = (p->node[nnum].y_left > p->node[nnum].y_right) ? p->node[nnum].y_left : p->node[nnum].y_right;
    y_min = (p->node[nnum].y_left < p->node[nnum].y_right) ? p->node[nnum].y_left : p->node[nnum].y_right;


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
    } else if (y_u < y_max && x_l < p->node[nnum].x && x_r > p->node[nnum].x) {
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
    } else {
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
        } else{
            break;
        }
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


int32_t
dijkstra (graph_t* g, heap_t* h, vertex_set_t* src, vertex_set_t* dest,
          path_t* path)
{
    int32_t current_min_id, nei_id;   // variables related to current vertex information
    int32_t dis, dis_nei;   // related neighbors information
    int32_t closest_dis, closest_dest_id, current_dest_id;  // dest vertex related information
    int32_t path_start, path_end;   // the id of the end and start of the path in graph
    int32_t* h_ptr = h->elt;
    

    initialize_heap(g, h, src);

    while (h->n_elts != 0){
        for (int i = h->n_elts/2, size = h->n_elts; i >= 0; i--){
                heapify(h, g, i, size);     // heapify to construct small_top heap
        }
        current_min_id = h->elt[0];
        g->vertex[current_min_id].visit = 1;     // set current closest vertex as visited

        // loop through the neighbors of current closest vertex and update related information
        for (int i = 0; i < g->vertex[current_min_id].n_neighbors; i++){    

            nei_id = g->vertex[current_min_id].neighbor[i];     // id of the neighbor;
            dis = g->vertex[current_min_id].distance[i] + g->vertex[current_min_id].from_src;   // calculate the distance to src

            if (g->vertex[nei_id].visit){    // if the neighbors of current closest vertex has been visited
                dis_nei = g->vertex[nei_id].from_src;   // check neighbor's dis to src
                if (dis < dis_nei){         // if closer, update the dis and pred id
                    g->vertex[nei_id].from_src = dis;
                    g->vertex[nei_id].pred = current_min_id;    
                }
            } else{     // if not visited, add to the heap and mark as visited
                h->elt[h->n_elts] = nei_id;
                g->vertex[nei_id].heap_id = h->n_elts;     // update heap_id
                h->n_elts += 1;
                g->vertex[nei_id].visit = 1;            // update as visited
                g->vertex[nei_id].from_src = dis;       // update the distance
                g->vertex[nei_id].pred = current_min_id;  // update the pred id
            }
        }
        // after loop through all neighbors, pop current vertex from the heap
        pop_heap_vertex(h);
    }
    h->elt = h_ptr;     // reload the original pointer into heap
    // when the heap has been clear, all vertex in graph has been checked with min dis to src
    closest_dis = g->vertex[dest->id[0]].from_src;
    for (int i = 0; i < dest->count; i++){  // find the closest the dest vertex id;
        current_dest_id = dest->id[i];      // current dest's id
        if (g->vertex[current_dest_id].from_src <= closest_dis){    // check if it is smaller than closest dis
            closest_dest_id = current_dest_id;  
            closest_dis = g->vertex[current_dest_id].from_src;
        }
    }

    // from the dest vertex to search for the src vertex using pred and fill into the path;
    path_end = closest_dest_id;     // the closest dest vertex is the end of the path;

    path->tot_dist = g->vertex[path_end].from_src;
    path->n_vertices = 0;
    //****************************** mp10 modified *********************************
    path_start = path_end;
    while (g->vertex[path_start].pred != -1){   // check if the vertex is src
        path->n_vertices += 1;
        path_start = g->vertex[path_start].pred;
    } path->n_vertices += 1;        // calculate the number of vertices

    path->id = malloc(sizeof(*(path->id) * path->n_vertices));  // allocate memory locations for id

    if (path->id == NULL)   return 0;   // return 0 if failed

    for (int i = path->n_vertices - 1, path_start = path_end; i >= 0; i--){
        path->id[i] = path_start;
        path_start = g->vertex[path_start].pred;
    }
    //*******************************************************************************
    return 1;
}


void
initialize_heap(graph_t* g, heap_t* h, vertex_set_t* src)
{
    h->n_elts = 0;
    for (int i = 0; i < g->n_vertices; i++){ // initialize all distance to infinity
        g->vertex[i].from_src = MY_INFINITY;
        g->vertex[i].visit = 0; // set all points as unvisited
    }
    for (int i = 0; i < src->count; i++){   
        g->vertex[src->id[i]].pred = -1;     // the previous vertex of starting point doesnot exist;
        g->vertex[src->id[i]].visit = 1;    // initialize the starting points as visited
        g->vertex[src->id[i]].from_src = 0;  // initialize the starting points to 0;
        g->vertex[src->id[i]].heap_id = i;  // initialize the starting points into the heap
        h->elt[i] = src->id[i];
        h->n_elts += 1;   
    }
}


void
pop_heap_vertex(heap_t* h)
{
    h->elt += 1;
    h->n_elts -= 1;
    
}


void
swap(int32_t* a, int32_t* b, graph_t* g)
{
    int temp_val = *a;
    int temp_id = g->vertex[*a].heap_id;

    g->vertex[*a].heap_id = g->vertex[*b].heap_id;
    *a = *b;
    g->vertex[*b].heap_id = temp_id;
    *b = temp_val;
}


void
heapify(heap_t* h, graph_t* g, int32_t i, int32_t n)
{
    int32_t smallest_heap_id = i;
    int32_t left = 2*i + 1, right = 2*i + 2;
    int32_t current_dis = g->vertex[h->elt[smallest_heap_id]].from_src;
    int32_t left_dis = g->vertex[h->elt[left]].from_src;
    int32_t right_dis = g->vertex[h->elt[right]].from_src;

    // if the left son is larger
    if (left < n && left_dis < current_dis) {
        smallest_heap_id = left;
    }
    // if the right son is larger
    if (right < n && right_dis < current_dis) {
        smallest_heap_id = right;
    }
    // if larger exists
    if (smallest_heap_id != i){
        // swap position in heap as well as heap_id in graph
        swap(&h->elt[i], &h->elt[smallest_heap_id], g);
        // recursive heapify
        heapify(h, g, smallest_heap_id, n);
    }
}























