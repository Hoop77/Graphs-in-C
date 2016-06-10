#include "graph.h"


Graph *graph_new( int vertexCount )
{
    Graph *graph = (Graph *) malloc( sizeof( Graph ));
    assert( graph != NULL );

    graph->vertexCount = vertexCount;
    graph->vertices = (Vertex **) malloc( vertexCount * sizeof( Vertex * ));

    // Creating and initializing the individual vertices.
    int vertexNum;
    for( vertexNum = 0; vertexNum < vertexCount; vertexNum++ )
    {
        Vertex *vertex = vertex_new();
        graph->vertices[ vertexNum ] = vertex;
    }

    return graph;
}


void graph_destroy( Graph *graph )
{
    free( graph );
    return;
}


void graph_destroyAll( Graph *graph )
{
    // Destroy array of vertices of the graph.
    int vertexNum;
    for( vertexNum = 0; vertexNum < graph->vertexCount; vertexNum++ )
    {
        vertex_destroyAll( graph->vertices[ vertexNum ] );
    }

    free( graph->vertices );
    free( graph );
    return;
}


Vertex *graph_getVertex( Graph *graph, int vertexNum )
{
    return graph->vertices[ vertexNum ];
}


int graph_getVertexCount( Graph *graph )
{
    return graph->vertexCount;
}


void graph_addVertex( Graph *graph )
{
    int oldVertexCount = graph->vertexCount;

    // Expand the vertex array with 1 vertex.
    int newVertexCount = oldVertexCount + 1;
    Vertex **extendedVertices = (Vertex **) malloc( newVertexCount * sizeof( Vertex * ));
    assert( extendedVertices != NULL );

    // Copy old vertices to the new vertex array.
    int vertexNum;
    for( vertexNum = 0; vertexNum < oldVertexCount; vertexNum++ )
    {
        extendedVertices[ vertexNum ] = graph->vertices[ vertexNum ];
    }

    // Create the new vertex and add it to the array.
    Vertex *addedVertex = vertex_new();
    int addedVertexNum = oldVertexCount;
    extendedVertices[ addedVertexNum ] = addedVertex;

    // We can now free the old array and assign the new vertices to the graph.
    free( graph->vertices );
    graph->vertices = extendedVertices;
    graph->vertexCount = newVertexCount;

    return;
}


void graph_addEdgePair( Graph *graph, int vertexNum1, int vertexNum2 )
{
    // Get the vertices which stores the lists of edges.
    Vertex *vertex1 = graph->vertices[ vertexNum1 ];
    Vertex *vertex2 = graph->vertices[ vertexNum2 ];

    // Get the lists of edges.
    DList *edges1 = vertex_getEdges( vertex1 );
    DList *edges2 = vertex_getEdges( vertex2 );

    // Create an edge which points to vertex 2 and append it to the list.
    Edge *edge1 = edge_new( vertexNum2 );
    dList_append( edges1, edge1 );

    // Create an iterator which stands at the appended edge.
    DListIterator *iterator1 = dListIterator_getIteratorFromList( edges1 );
    dListIterator_toLast( iterator1, edges1 );

    // Create an edge which points to vertex 1 and append it to the list.
    Edge *edge2 = edge_new( vertexNum1 );
    dList_append( edges2, edge2 );

    // Create an iterator which stays at the appended edge.
    DListIterator *iterator2 = dListIterator_getIteratorFromList( edges2 );
    dListIterator_toLast( iterator2, edges2 );

    // Set up the corresponding edge with the iterators.
    edge_setCorrespondingEdgeIterator( edge1, iterator2 );
    edge_setCorrespondingEdgeIterator( edge2, iterator1 );

    return;
}


bool graph_removeEdgePair( Graph *graph, int vertexNum1, int vertexNum2 )
{
    // Get the vertices which store the lists of edges.
    Vertex *vertex1 = graph->vertices[ vertexNum1 ];
    Vertex *vertex2 = graph->vertices[ vertexNum2 ];
    DList *edges1 = vertex_getEdges( vertex1 );
    DList *edges2 = vertex_getEdges( vertex2 );

    if( !vertex_hasEdges( vertex1 ))
        return false;

    // Create an iterator which will get us the edge going from vertex 1 to vertex 2.
    DListIterator *edgeIterator1 = dListIterator_getIteratorFromList( edges1 );
    dListIterator_toFirst( edgeIterator1, edges1 );

    // Prepare a comparator for searching.
    Edge searchEdge = { vertexNum2, NULL };
    Comparator *comparator = comparator_new( &searchEdge, (CompareFunction) edge_equals );

    // Search the edge.
    Edge *edge1 = (Edge *) dListIterator_search( &edgeIterator1, edges1, comparator );

    // If we haven't found the edge, we stop here.
    if( edge1 == NULL )
        return false;

    // Now that we have edge 1, we get edge 2 immediately with the corresponding edge iterator.
    DListIterator *edgeIterator2 = edge_getCorrespondingEdgeIterator( edge1 );

    // Remove the edges.
    dListIterator_destroy( edgeIterator1, edges1 );
    dListIterator_destroy( edgeIterator2, edges2 );

    // Clean up.
    comparator_destroy( comparator );

    return true;
}


bool graph_hasEdges( Graph *graph )
{
    int vertexNum;
    for( vertexNum = 0; vertexNum < graph->vertexCount; vertexNum++ )
    {
        if( vertex_hasEdges( graph->vertices[ vertexNum ] ))
            return true;
    }

    return false;
}
