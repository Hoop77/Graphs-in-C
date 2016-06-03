#include "edge.h"


Edge *edge_new( int toVertexNum )
{
    Edge *edge = (Edge *) malloc( sizeof( Edge ));

    edge->toVertexNum = toVertexNum;
    edge->correspondingEdgeIterator = NULL;

    return edge;
}


void edge_destroy( Edge *edge )
{
    assert( edge != NULL );
    dListIterator_destroy( edge->correspondingEdgeIterator );

    free( edge );

    return;
}


int edge_getToVertexNum( Edge *edge )
{
    return edge->toVertexNum;
}


void edge_setCorrespondingEdgeIterator( Edge *edge, DListIterator *correspondingEdgeIterator )
{
    edge->correspondingEdgeIterator = correspondingEdgeIterator;
    return;
}


DListIterator *edge_getCorrespondingEdgeIterator( Edge *edge )
{
    return edge->correspondingEdgeIterator;
}


bool edge_equals( Edge *edge1, Edge *edge2 )
{
    return edge1->toVertexNum == edge2->toVertexNum;
}

