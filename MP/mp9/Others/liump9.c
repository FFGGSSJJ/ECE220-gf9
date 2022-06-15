#include <stdint.h>
#include <stdio.h>

#include "mp5.h"
#include "mp9.h"

#define MY_INFINITY   1000000000



void
find_nodes (locale_t* loc, vertex_set_t* vs, pyr_tree_t* p, int32_t nnum)
{
    record_fn_call ();
 	int32_t locx = loc -> x, locy = loc -> y, locr = loc -> range;
 	int32_t nowx = p -> node[nnum].x, nowyl = p -> node[nnum].y_left, nowyr = p -> node[nnum].y_right, nowid = p -> node[nnum].id;
 	// printf("%d %d %d %d %d %d\n", locx, locy, locr, nowx, nowyl, nowyr);
 	if (4 * nnum + 1 >= p -> n_nodes) {
 		if (in_range(loc, nowx, nowyl)) {
 			vs -> id[vs -> count] = nowid;
 			vs -> count += 1;
 		}
 		return ;
 	}
 	int32_t flag_left = in_range(loc, nowx, nowyl), flag_right = in_range(loc, nowx, nowyr);
 	if (locx <= nowx) {
 		if (flag_right == 1) {
 			find_nodes(loc, vs, p, 4 * nnum + 2);
 			find_nodes(loc, vs, p, 4 * nnum + 4);
 		}
 		else {
 			if (locx + locr >= nowx) {
	 			if (locy <= nowyr) find_nodes(loc, vs, p, 4 * nnum + 2);
	 			else find_nodes(loc, vs, p, 4 * nnum + 4);
	 		}
 		}
 		if (locy <= nowyl + locr) find_nodes(loc, vs, p, 4 * nnum + 1);
 		if (locy > nowyl - locr) find_nodes(loc, vs, p, 4 * nnum + 3);
 	}
 	else {
 		if (flag_left == 1) {
 			find_nodes(loc, vs, p, 4 * nnum + 1);
 			find_nodes(loc, vs, p, 4 * nnum + 3);
 		}
 		else {
 			if (locx - locr <= nowx) {
	 			if (locy <= nowyl) find_nodes(loc, vs, p, 4 * nnum + 1);
	 			else find_nodes(loc, vs, p, 4 * nnum + 3);
	 		}
 		}
 		if (locy <= nowyr + locr) find_nodes(loc, vs, p, 4 * nnum + 2);
 		if (locy > nowyr - locr) find_nodes(loc, vs, p, 4 * nnum + 4);
 	}
}


void
trim_nodes (graph_t* g, vertex_set_t* vs, locale_t* loc)
{
	int32_t n = vs -> count, num = 0;
	for (int32_t i = 0; i < n; ++i) {
		int32_t idx = vs -> id[i];
		int32_t xi = g -> vertex[idx].x, yi = g -> vertex[idx].y;
		if (in_range(loc, xi, yi)) {
			vs -> id[num] = vs -> id[i];
			++ num;
		}
	}
	vs -> count = num;
}


void heap_init(graph_t* g, heap_t* h) {
	h -> n_elts = 0;
	int32_t n = g -> n_vertices;
	for (int32_t i = 0; i < n; ++i) {
		g -> vertex[i].heap_id = -1;
		g -> vertex[i].from_src = MY_INFINITY;
		g -> vertex[i].pred = -1;
	}
	for (int32_t i = 0; i < n; ++i)
		h -> elt[i] = -1;
}

void swap(int32_t *a, int32_t *b) {
	int32_t temp = *a;
	*a = *b, *b = temp;
}

void heap_up(graph_t* g, heap_t* h, int32_t x) {
	if (x == 0) return ;
	int32_t fa = x / 2;
	if (g -> vertex[h -> elt[x]].from_src < g -> vertex[h -> elt[fa]].from_src) {
		swap(&(g -> vertex[h -> elt[x]].heap_id), &(g -> vertex[h -> elt[fa]].heap_id));
		swap(&(h -> elt[x]), &(h -> elt[fa]));
		heap_up(g, h, fa);
	}
}

void heap_down(graph_t* g, heap_t* h, int32_t x) {
	int32_t son1 = 2 * x + 1, son2 = 2 * x + 2;
	int32_t value = g -> vertex[h -> elt[x]].from_src; 
	if (son1 >= h -> n_elts) return ;
	else if (son2 >= h -> n_elts) {
		int32_t value_1 = g -> vertex[h -> elt[son1]].from_src;
		if (value > value_1) {
			swap(&(g -> vertex[h -> elt[x]].heap_id), &(g -> vertex[h -> elt[son1]].heap_id));
			swap(&(h -> elt[x]), &(h -> elt[son1]));
			heap_down(g, h, son1);
		}
	}
	else {
		int32_t value_1 = g -> vertex[h -> elt[son1]].from_src;
		int32_t value_2 = g -> vertex[h -> elt[son2]].from_src;
		if (value_2 >= value_1 && value_1 < value) {
			swap(&(g -> vertex[h -> elt[x]].heap_id), &(g -> vertex[h -> elt[son1]].heap_id));
			swap(&(h -> elt[x]), &(h -> elt[son1]));
			heap_down(g, h, son1);
		}
		else if (value_2 < value_1 && value_2 < value) {
			swap(&(g -> vertex[h -> elt[x]].heap_id), &(g -> vertex[h -> elt[son2]].heap_id));
			swap(&(h -> elt[x]), &(h -> elt[son2]));
			heap_down(g, h, son2);
		}
		else return ;
	}
}

void heap_add(graph_t* g, heap_t* h, int32_t num) {
	h -> elt[h -> n_elts] = num;
	g -> vertex[num].heap_id = h -> n_elts;
	heap_up(g, h, h -> n_elts);
	++ (h -> n_elts);
}

int32_t heap_pop(graph_t* g, heap_t* h) {
	swap(&(g -> vertex[h -> elt[0]].heap_id), &(g -> vertex[h -> elt[h -> n_elts - 1]].heap_id));
	int32_t res = h -> elt[0];
	h -> elt[0] = h -> elt[h -> n_elts - 1];
	h -> elt[h -> n_elts - 1] = res;
	-- (h -> n_elts);
	heap_down(g, h, 0);
	return res;
}


int32_t
dijkstra (graph_t* g, heap_t* h, vertex_set_t* src, vertex_set_t* dest,
          path_t* path)
{
	path -> tot_dist = MY_INFINITY;
	for (int32_t i = 0; i < src -> count; ++i) {
		heap_init(g, h);
		int32_t idx = src -> id[i];
		g -> vertex[idx].from_src = 0;
		g -> vertex[idx].heap_id = 0;
		h -> n_elts = 1;
		h -> elt[0] = idx;
		while (h -> n_elts > 0) {
			int32_t now = heap_pop(g, h), dis = g -> vertex[now].from_src;
			// printf("%d %d\n", now, dis);
			g -> vertex[now].heap_id = -2;
			for (int32_t j = 0; j < g -> vertex[now].n_neighbors; ++j) {
				// printf("n_elts = %d\n", h -> n_elts);
				// for (int i = 0; i < h -> n_elts; ++i) printf("ccc %d ", h -> elt[i]);
				// printf("\n");
				int32_t temp_id = g -> vertex[now].neighbor[j];
				// printf("-%d-\n", temp_id);
				if (g -> vertex[temp_id].heap_id == -2) continue;
				if (dis + g -> vertex[now].distance[j] < g -> vertex[temp_id].from_src) {
					g -> vertex[temp_id].from_src = dis + g -> vertex[now].distance[j];
					g -> vertex[temp_id].pred = now;
					if (g -> vertex[temp_id].heap_id == -1)
						heap_add(g, h, temp_id);
					else heap_up(g, h, g -> vertex[temp_id].heap_id);
				}
			}
			
		}
		int32_t res = -1, ans = MY_INFINITY;
		for (int32_t j = 0; j < dest -> count; ++j) {
			if (g -> vertex[dest -> id[j]].from_src < ans) {
				res = dest -> id[j];
				ans = g -> vertex[res].from_src;
			}
		}
		// printf("-- %d %d\n", res, ans);
		if (ans < path -> tot_dist) {
			path -> tot_dist = ans;
			int32_t temp = res, tot = 0;
			while (temp != -1) {
				temp = g -> vertex[temp].pred;
				++ tot;
			}
			path -> n_vertices = tot;
			temp = res;
			while (temp != -1) {
				-- tot;
				path -> id[tot] = temp;
				temp = g -> vertex[temp].pred;
			}
		}
	}
    return path -> tot_dist;
}

