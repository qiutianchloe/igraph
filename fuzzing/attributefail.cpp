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

#include "igraph.h"
#include <cstdio>

extern "C"
int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    igraph_set_error_handler(igraph_error_handler_ignore);
    igraph_error_handler_t *handler = igraph_set_error_handler(igraph_error_handler_ignore); 
    igraph_set_attribute_table(&igraph_cattribute_table);

    igraph_vector_ptr_t *attr1 = (igraph_vector_ptr_t* )malloc(sizeof(igraph_vector_ptr_t));
    igraph_vector_ptr_init(attr1, 1);

    igraph_vector_t* vec_a1 = (igraph_vector_t* )malloc(sizeof(igraph_vector_t));
    igraph_vector_init_int(vec_a1, 3, 1, 2, 3);

    igraph_attribute_record_t* a1 = (igraph_attribute_record_t* )malloc(sizeof(igraph_attribute_record_t));
    a1->name = "any";
    a1->type = IGRAPH_ATTRIBUTE_NUMERIC;
    a1->value = &vec_a1;

    igraph_vector_ptr_set(attr1, 0, a1); 


    if (Size % 2 == 1 || Size > 65280) {
        return 0;
    }

    check_err(igraph_vector_int_init(&edges, Size));
    for (size_t i=0; i < Size; ++i) {
        VECTOR(edges)[i] = Data[i];
    }

    if (! igraph_create(&graph, &edges, 3, IGRAPH_DIRECTED)) {

        igraph_add_vertices(&graph, 2, attr1); 

    }

    igraph_vector_int_destroy(&edges);

    IGRAPH_ASSERT(IGRAPH_FINALLY_STACK_EMPTY);

    return 0;  // Non-zero return values are reserved for future use.
}
