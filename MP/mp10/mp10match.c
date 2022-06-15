#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "mp5.h"
#include "mp10.h"


//
// These variables hold the heads of two singly-linked lists of 
// requests.  
//
// The avaialble list consists of unpaired requests: partner 
// should be NULL, and next is used to form the list.   
// 
// The shared list consists of groups with non-empty start and end 
// vertex intersections.  Only one of the requests in the group is in 
// the list.  The others are linked through the first's partner field 
// and then through the next field of the others in the group.  The 
// next field of the first request in a group is used to form the
// shared list.
//
// Note: for MP2, you should only build groups of two in the shared
// list.
//

static request_t* available = NULL;
static request_t* shared = NULL;


/*
 * Homework: Machine Problem 10
 * Student: Guanshujie Fu
 * Student ID: 3190110666
 * File name: mp10match.c, mp10.c, mp10alloc.c, mp9.c
 * Description:
 *      This mp will handle several requests using linked list and 
 *      find the paired requests or unpaired requests. Dynamic allocation
 *      is applied to creat spaces for array or structure. Details will 
 *      be presented in the functions
 */


int32_t
handle_request (graph_t* g, pyr_tree_t* p, heap_t* h, request_t* r)
{
    path_t* path;
    request_t* ptr_avai = available;    // ptr_avai is the second request while r is the first request
    request_t* pred_avai = available;
    vertex_set_t* vs_intersect_src;
    vertex_set_t* vs_intersect_dst;


    mark_vertex_minimap(g, p);      // mark all vertex

    if ((r->src_vs = new_vertex_set()) == NULL || 
        (r->dst_vs = new_vertex_set()) == NULL) {
        if (r->src_vs != NULL)      free_vertex_set(r->src_vs);
        if (r->dst_vs != NULL)      free_vertex_set(r->dst_vs);
    }

    find_nodes(&(r->from), r->src_vs, p, 0);    // initialize the request‘s src and dst
    build_vertex_set_minimap(g, r->src_vs);     // build minimap for both
    find_nodes(&(r->to), r->dst_vs, p, 0);
    build_vertex_set_minimap(g, r->dst_vs);

    if (0 == r->src_vs->count || 0 == r->dst_vs->count) {   // if no src or dst was found
        free_vertex_set(r->src_vs);
        free_vertex_set(r->dst_vs);
        return 0;
    }
    if (available == NULL) {
        available = r;
        available->path = NULL;     // remember to initialize the path of current unpaired request
        available->next = NULL;     //       to NULL or it will generate bug in free_all_data().
        return 1;
    } 

    while (ptr_avai != NULL){       // loop through available to search for request
        if ((path = new_path()) == NULL){   // if path allocate fail, free
            free_vertex_set(r->src_vs);
            free_vertex_set(r->dst_vs);
            return 0;
        } 
        if ((vs_intersect_src = new_vertex_set()) == NULL || (vs_intersect_dst = new_vertex_set()) == NULL) { // allocate for vs
            free_path(path);
            free_vertex_set(vs_intersect_src);  // if failed, free and return
            free_vertex_set(r->src_vs);
            free_vertex_set(r->dst_vs);
            return 0;  
        }
        if ((vs_intersect_src->id = malloc((r->src_vs->count+ptr_avai->src_vs->count) * sizeof(int32_t))) == NULL || 
            (vs_intersect_dst->id = malloc((r->dst_vs->count+ptr_avai->dst_vs->count) * sizeof(int32_t))) == NULL){ // allocate for vs->id
            free_path(path);
            free_vertex_set(vs_intersect_src);    // if failed, free and return
            free_vertex_set(vs_intersect_dst);
            free_vertex_set(r->src_vs);
            free_vertex_set(r->dst_vs);
            return 0;
        }

        // now vs for intersecting have been allocated. 
        if ((r->src_vs->minimap & ptr_avai->src_vs->minimap) == 0 ||
            (r->dst_vs->minimap & ptr_avai->dst_vs->minimap) == 0) {    //check the minimap
            pred_avai = ptr_avai;
            ptr_avai = ptr_avai->next;
            free_path(path);
            free_vertex_set(vs_intersect_src);    //  free vs
            free_vertex_set(vs_intersect_dst);
            continue;   // if not overlap at all, skip it
        } 
        if (merge_vertex_sets(r->src_vs, ptr_avai->src_vs, vs_intersect_src) == 0 || 
            merge_vertex_sets(r->dst_vs, ptr_avai->dst_vs, vs_intersect_dst) == 0) {    // search for matching
            // if not match, add first request to available
            pred_avai = ptr_avai;
            ptr_avai = ptr_avai->next;
            free_path(path);
            free_vertex_set(vs_intersect_src);    // if failed, free vs
            free_vertex_set(vs_intersect_dst);
            continue;
        }

        // now both src and dst intersection have been found and stored in vs_intersect_src/dst
        // which denotes that match has been found, in this case, search for the path
        if (0 == dijkstra(g, h, vs_intersect_src, vs_intersect_dst, path)) { // if failed to found a path
            pred_avai = ptr_avai;
            ptr_avai = ptr_avai->next;
            free_path(path);
            free_vertex_set(vs_intersect_src);    // if failed, free vs
            free_vertex_set(vs_intersect_dst);
            continue;       // did not match, return 1
        } else {    // found a path, remove second request from available and add both to share
            // first add the first request into the shared
            build_vertex_set_minimap(g, vs_intersect_src);      // creat minimap for both intersection sets
            build_vertex_set_minimap(g, vs_intersect_dst);
            build_path_minimap(g, path);

            *r->src_vs = *vs_intersect_src;
            *r->dst_vs = *vs_intersect_dst;
            //free_vertex_set(vs_intersect_src);    // i do not know why these two free will generate bugs
            //free_vertex_set(vs_intersect_dst);
            r->path = path;
            r->partner = ptr_avai;  // link the second request to first
            if (shared == NULL) {   // if shared is empty for now
                shared = r;
                shared->next = NULL;
            } else {
                r->next = shared;   // add shared's ptr to r->next
                shared = r;     // now the head of shared is r
            }
            // then remove the second requests from the available
            ptr_avai->src_vs = vs_intersect_src;
            ptr_avai->dst_vs = vs_intersect_dst;
            ptr_avai->path = path;
            ptr_avai->partner = NULL;
            if (pred_avai == ptr_avai) {    // if it is the first request in available
                ptr_avai = ptr_avai->next;
                available = available->next;    // move available to remove this request as it is at the head
                pred_avai->next = NULL;
                pred_avai = ptr_avai;   
            } else {    // if it is not the first element
                pred_avai->next = ptr_avai->next;   // set previous next 
                ptr_avai->next = NULL;  // set the next of second request to NULL
            }

            // now the first and second requests has been linked correctly as it pair two requests
            // it should free the dynamic location
            // and return 1 
            return 1;
        }
        
    } // end of while, all requests in available have been checked without finding satisfied request
    r->path = NULL;
    r->next = available;    // insert the request at the head of available
    available = r;
    return 1;
}


void
print_results ()
{
    request_t* r;
    request_t* prt;

    printf ("Matched requests:\n");
    for (r = shared; NULL != r; r = r->next) {
        printf ("%5d", r->uid);
	for (prt = r->partner; NULL != prt; prt = prt->next) {
	    printf (" %5d", prt->uid);
	}
	printf (" src=%016lX dst=%016lX path=%016lX\n", r->src_vs->minimap,
		r->dst_vs->minimap, r->path->minimap);
    }

    printf ("\nUnmatched requests:\n");
    for (r = available; NULL != r; r = r->next) {
        printf ("%5d src=%016lX dst=%016lX\n", r->uid, r->src_vs->minimap,
		r->dst_vs->minimap);
    }
}


int32_t
show_results_for (graph_t* g, int32_t which)
{
    request_t* r;
    request_t* prt;

    // Can only illustrate one partner.
    for (r = shared; NULL != r; r = r->next) {
	if (which == r->uid) {
	    return show_find_results (g, r, r->partner);
	}
	for (prt = r->partner; NULL != prt; prt = prt->next) {
	    if (which == prt->uid) {
		return show_find_results (g, prt, r);
	    }
	}
    }
    for (r = available; NULL != r; r = r->next) {
        if (which == r->uid) {
	    return show_find_results (g, r, r);
	}
    }
    
    return 0;
}


static void
free_request (request_t* r)
{
    free_vertex_set (r->src_vs);
    free_vertex_set (r->dst_vs);
    if (NULL != r->path) {
	free_path (r->path);
    }
    free (r);
}

void
free_all_data ()
{
    request_t* r;
    request_t* prt;
    request_t* next;

    // All requests in a group share source and destination vertex sets
    // as well as a path, so we need free those elements only once.
    for (r = shared; NULL != r; r = next) {
	for (prt = r->partner; NULL != prt; prt = next) {
	    next = prt->next;
	    free (prt);
	}
	next = r->next;
	free_request (r);
    }

    for (r = available; NULL != r; r = next) {
	next = r->next;
	free_request (r);
    }
}


