#include <stdint.h>
#include <stdio.h>

#include "mp5.h"
#include "mp9.h"


int32_t
match_requests (graph_t* g, pyr_tree_t* p, heap_t* h,
		request_t* r1, request_t* r2,
		vertex_set_t* src_vs, vertex_set_t* dst_vs, path_t* path)
{
	src_vs -> count = 0;
	dst_vs -> count = 0;

	// printf("check1\n");

	find_nodes(&r1 -> from, src_vs, p, 0);
	trim_nodes(g, src_vs, &r2 -> from);
	// printf("%d\n", src_vs -> count);
	// for (int i = 0; i <= src_vs -> count; ++i) printf("%d %d %d\n", src_vs -> id[i], g -> vertex[src_vs -> id[i]].x, g -> vertex[src_vs -> id[i]].y);
	// printf("\n");
	// printf("check2\n");

	find_nodes(&r1 -> to, dst_vs, p, 0);
	trim_nodes(g, dst_vs, &r2 -> to);
	// printf("%d\n", dst_vs -> count);
	// for (int i = 0; i < dst_vs -> count; ++i) printf("%d %d %d\n", dst_vs -> id[i], g -> vertex[dst_vs -> id[i]].x, g -> vertex[dst_vs -> id[i]].y);
	// for (int i = 0; i < dst_vs -> count; ++i) printf("%d\n", dst_vs -> id[i]);
	// printf("\n");
	// printf("check3\n");

    return dijkstra(g, h, src_vs, dst_vs, path);
}
