/*
   IGraph library.
   Copyright (C) 2021-2022  The igraph development team

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301 USA
*/

#include <igraph.h>
#include <cstdlib>

inline void check_err(int err) {
    if (err)
        abort();
}

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    igraph_t graph;
    igraph_vector_int_t edges;

    igraph_set_error_handler(igraph_error_handler_ignore);
    igraph_set_warning_handler(igraph_warning_handler_ignore);

    /* We work with small, up-to 16-vertex graphs, as the algorithms
     * tested here can be slow. Each byte is interpreted as an edge.
     * A simple graph can have at most 120 edges, but we allow up
     * to 240, as the fuzzer usually generates multigraphs. */

    if (Size > 240) {
        return 0;
    }

    check_err(igraph_vector_int_init(&edges, 2*Size));
    size_t j = 0;
    for (size_t i=0; i < Size; ++i) {
        VECTOR(edges)[j++] = Data[i] / 16;
        VECTOR(edges)[j++] = Data[i] % 16;
    }

    if (! igraph_create(&graph, &edges, 0, IGRAPH_UNDIRECTED)) {
        igraph_integer_t conn;
        igraph_st_vertex_connectivity(&graph, &conn, 0, 1, IGRAPH_VCONN_NEI_ERROR);

        igraph_destroy(&graph);
    }

    igraph_vector_int_destroy(&edges);

    IGRAPH_ASSERT(IGRAPH_FINALLY_STACK_EMPTY);

    return 0;  // Non-zero return values are reserved for future use.
}
