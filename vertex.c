#include "vertex.h"


Vertex *vertex_new( void )
{
    Vertex *vertex = (Vertex *) malloc( sizeof( Vertex ));
    assert( vertex != NULL );

    vertex->edges = dList_new( sizeof( Edge ), (DestroyFunction) edge_destroyAll );

    return vertex;
}


void vertex_destroy( Vertex *vertex )
{
    free( vertex );
    return;
}


void vertex_destroyAll( Vertex *vertex )
{
    dList_destroyAll( vertex->edges );
    free( vertex );
    return;
}


DList *vertex_getEdges( Vertex *vertex )
{
    return vertex->edges;
}


int vertex_getDegree( Vertex *vertex )
{
    return dList_getSize( vertex->edges );
}


bool vertex_hasEdges( Vertex *vertex )
{
    return !dList_isEmpty( vertex->edges );
}
