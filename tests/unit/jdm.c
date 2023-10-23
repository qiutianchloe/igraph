/*
   IGraph library.
   Copyright (C) 2023  The igraph development team <igraph@igraph.org>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <igraph.h>
#include "test_utilities.h"

int main (void) {
    // Structures that need to be destroyed
    // TODO: Add edge cases (loops and multigraphs) and null graph
    // Include graphs with loops in the tests - count them once or twice?
    //
    igraph_t g;
    igraph_t g_dir;
    igraph_t g_null;
    igraph_matrix_int_t jdm;
    igraph_matrix_int_t jdm_dir;
    igraph_matrix_int_t jdm_null;
    igraph_vector_int_t weights;

    igraph_small(&g, 5, false,
                 0, 1, 0, 2, 0, 4,
                 1, 4,
                 2, 3, 2, 4,
                 3, 4,
                 -1);

    igraph_small(&g_dir, 5, true,
                 0, 1, 0, 2, 0, 4,
                 1, 0,
                 3, 2, 3, 4,
                 4, 0, 4, 1, 4, 2,
                 -1);

    // Null graph
    igraph_empty(&g_null, 0, IGRAPH_UNDIRECTED);

    // Initialize a matrix
    igraph_matrix_int_init(&jdm, 1, 1);
    igraph_matrix_int_init(&jdm_dir, 1, 1);
    igraph_matrix_int_init(&jdm_null, 1, 1);

    // Initialize weight vector
    igraph_vector_int_init(&weights, 0);
    igraph_vector_int_print(&weights);

    igraph_construct_jdm(&g, &jdm, 4, 4, NULL);
    igraph_construct_jdm(&g_dir, &jdm_dir,4, 4, NULL);
    igraph_construct_jdm(&g_null, &jdm_null, 4, -1, NULL);

    igraph_matrix_int_print(&jdm);
    igraph_matrix_int_print(&jdm_dir);
    igraph_matrix_int_print(&jdm_null);


    // Clean up
    igraph_destroy(&g);
    igraph_destroy(&g_dir);
    igraph_destroy(&g_null);
    igraph_matrix_int_destroy(&jdm);
    igraph_matrix_int_destroy(&jdm_dir);
    igraph_matrix_int_destroy(&jdm_null);
    igraph_vector_int_destroy(&weights);
    VERIFY_FINALLY_STACK();

    return 0;
}