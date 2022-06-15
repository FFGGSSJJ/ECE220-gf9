#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include "mp5.h"
#include "mp9.h"


int32_t
match_requests (graph_t* g, pyr_tree_t* p, heap_t* h,
		request_t* r1, request_t* r2,
		vertex_set_t* src_vs, vertex_set_t* dst_vs, path_t* path)
{
	int32_t check;
	clock_t start, finish;

	src_vs->count = 0;
	find_nodes(&(r1->from), src_vs, p, 0);
	trim_nodes(g, src_vs, &(r2->from));
	printf("src_num: %d\n", src_vs->count);
	
	dst_vs->count = 0;
	find_nodes(&(r1->to), dst_vs, p, 0);
	trim_nodes(g, dst_vs, &(r2->to));
	printf("dst_num: %d\n", dst_vs->count);
	
	if (src_vs->count == 0 || dst_vs->count == 0)	return 0;
	start = clock();
	check = dijkstra(g, h, src_vs, dst_vs, path);
	finish = clock();
	printf("time: %0.4f\n", (double) (finish - start)/CLOCKS_PER_SEC);
    return check;
}
