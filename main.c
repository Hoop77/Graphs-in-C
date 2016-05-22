//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// PROGRAM DESCRIPTION
// -------------------
//
// Author: Philipp Badenhoop
// Description: This program finds an Eulerian path on a given input graph.
// C-standard: C89,C90
//
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// HEADERS
// -------

#include "assert.h"
//#include "errno.h"
//#include "float.h"
//#include "limits.h"
//#include "locale.h"
//#include "math.h"
//#include "setjmp.h"
//#include "signal.h"
//#include "stdarg.h"
//#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
//#include "time.h"

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// DEFINITIONS
// -----------

//
// A debug flag to print out information when debuggin.
// This will be outcommentet on the release version.
//
#define DEBUG


#define VERTEX_UNDEFINED	-1


//
// We define the boolean type.
//
typedef enum
{
    FALSE = 0,
    TRUE = !FALSE
} bool;

//
// We define have different types of graphs.
//
typedef enum
{
    GRAPH_TYPE_NO_VERTICES_WITH_UNEVEN_DEGREE,
    GRAPH_TYPE_TWO_VERTICES_WITH_UNEVEN_DEGREE,
    GRAPH_TYPE_MORE_THAN_TWO_VERTICES_WITH_UNEVEN_DEGREE,
    GRAPH_TYPE_ALL_VERTICES_WITH_ZERO_DEGREE
} GraphType;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// DATAURES
// ---------------

//
// Aure for one-directional edges.
// TODO: Comment this!!!
//
typedef struct Edge Edge;
struct Edge
{
    Edge *prevEdge;
    Edge *nextEdge;

    int toVertexNum;
    Edge *otherEdge;
};

//
// Aure for vertices.
// Acts as a holder for a linked list of edges. The degree of a vertex is the number of edges it contains. This value has to be
// updated  whenever an edges is added or removed.
typedef struct
{
    Edge *firstEdge;
    Edge *lastEdge;
    int degree;
} Vertex;

//
// Aure for graphs.
// It simply holds an array of vertices.
typedef struct
{
    Vertex **vertices;
    int vertexCount;
} Graph;

//
// A path element which stores its corresponding vertex and represents a node in a linked list and therefore it holds a next pointer
// to the next element in the path.
//
typedef struct PathElement PathElement;
struct PathElement
{
    int vertexNum;
    PathElement *next;
    PathElement *prev;
};

//
// A Path that acts as a holder for a linked list of path elements. It stores the first and the last element of the path.
//
typedef struct
{
    PathElement *firstElement;
    PathElement *lastElement;
} Path;

//
// Information about a graph.
//
typedef struct
{
    GraphType graphType;
    int startOrEndVertexNum1;
    int startOrEndVertexNum2;
    int vertexWithMaxDegree;
} GraphInformation;

//
// Result data of findEulerianCycle().
//
typedef struct
{
    bool exists;
    Path *eulerianCycle;
} EulerianCycleResult;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// FUNCTION DECLARATION
// --------------------

Graph 					*loadGraph( char *filename );
GraphInformation 		checkVertexDegrees( Graph *graph );
void 					convertEulerianPathToCycle( Graph *graph, GraphInformation graphInfo );
EulerianCycleResult 	findEulerianCycle( Graph *graph, GraphInformation information );
Path 					*extractSubCircle( Graph *graph, int startVertexNum );

void 					printEulerianCycle( Path *eulerianCycle );
void					printEulerianPath( Path *EulerianCylce, int addedVertexNum );

void					addEdge( Graph *graph, int vertexNum1, int vertexNum2 );
Edge 					*addSingleEdge( Graph *graph, int fromVertexNum, int toVertexNum );

void 					removeEdge( Graph *graph, int vertexNum1, int vertexNum2 );
void 					removeSingleEdge( Vertex *vertex, Edge **edge );
bool 					graphHasEdges( Graph *Graph );
void					appendPathElement( Path *path, int vertexNum );
void 					mergePaths( Path *path1, Path *path2, PathElement **mergingElement );

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// FUNCTION DEFINITION
// -------------------


//------------------------------------------------------------------------------------------
// Description: This is the entry of the program.
//------------------------------------------------------------------------------------------
int main( int argc, char *argv[] )
{
    bool proceed = TRUE;
    Graph *graph;
    GraphInformation graphInfo;
    EulerianCycleResult eulerianCycleResult;

    // Check argument size: we specified that the first argument is the filename to our graph data.
    if( argc >= 2 )
    {
        // load graph
        graph = loadGraph( argv[1] );
    }
    else
    {
        fprintf( stderr, "Error: One argument for input file expected!" );
        return 0;
    }

    // We check if the graph was successfully loaded.
    if( graph == NULL )
    {
        fprintf( stderr, "Could not load graph." );
        return 0;
    }

    //
    // Now that we have the graph in memory, we check if we canNOT find an eulerian path.
    // It has no eulerian path if more than two vertices have an uneven degree.
    //
    graphInfo = checkVertexDegrees( graph );

    if( graphInfo.graphType == GRAPH_TYPE_NO_VERTICES_WITH_UNEVEN_DEGREE
     || graphInfo.graphType == GRAPH_TYPE_TWO_VERTICES_WITH_UNEVEN_DEGREE )
    {
        // everything's alright - we can go on...
        proceed = TRUE;
    }
    else
    {
        // Outputting.
        printf( "An eulerian path does not exist.\n" );

        if( graphInfo.graphType == GRAPH_TYPE_MORE_THAN_TWO_VERTICES_WITH_UNEVEN_DEGREE )
        {
            printf( "More than two vertices with uneven degree.\n" );
        }
        else
        {
            printf( "All vertices with zero degree.\n" );
        }

        proceed = FALSE;
    }

    //
    // Now there's potential for an eulerian cycle but we cannot be completely sure because the graph must also be connected.
    // However, there might be some vertices that do not have any edges - which is valid. Our algorithm handles that.
    //
    if( proceed )
    {
        eulerianCycleResult = findEulerianCycle( graph, graphInfo );

        //
        // Outputting.
        //
        if( eulerianCycleResult.exists )
        {
            if( graphInfo.graphType == GRAPH_TYPE_TWO_VERTICES_WITH_UNEVEN_DEGREE )
            {
                // We have to specify the vertex number of the added vertex which is always the last vertex.
                printEulerianPath( eulerianCycleResult.eulerianCycle, graph->vertexCount - 1 );
            }
            else
            {
                printEulerianCycle( eulerianCycleResult.eulerianCycle );
            }

            printf( "\n" );
        }
        else
        {
            printf( "An eulerian path does not exist.\nThe graph is disconnected.\n." );
        }
    }

    return 0;
}


//------------------------------------------------------------------------------------------
// Description: Loads a graph from a file.
//------------------------------------------------------------------------------------------
Graph *loadGraph( char *filename )
{
    Graph *graph = NULL;			// the result graph
    FILE *f;						// file pointer
    int decimalsRead;				// number of decimals that we read from a line
    int vertexCount = 0;			// vertex count
    int currVertexNum;				// current vertex number
    int vertexNum1;					// first vertex number in a line
    int vertexNum2;					// second vertex number in a line
    Vertex *vertex;

    // Opening the file.
    f = fopen( filename, "r" );
    if( f == NULL )
    {
        return NULL;
    }

    // We start by reading the number of vertices.
    decimalsRead = fscanf( f, "%d\n", &vertexCount );
    if( decimalsRead != 1 )
    {
        return NULL;
    }

    // Create the graph.
    graph = malloc( sizeof( Graph ));
    assert( graph != NULL );

    graph->vertexCount = vertexCount;

    // Create the vertex array.
    graph->vertices = malloc( vertexCount * sizeof( Vertex * ));
    assert( graph->vertices != NULL );

    // Creating and initializing the individual vertices.
    for( currVertexNum = 0; currVertexNum < vertexCount; currVertexNum++ )
    {
        // TODO create something like a function vertex_new()
        vertex = malloc( sizeof( Vertex ));
        assert( vertex != NULL );

        vertex->degree = 0;
        vertex->firstEdge = NULL;
        vertex->lastEdge = NULL;

        graph->vertices[ currVertexNum ] = vertex;
    }

    //
    // Now we are building the edges.
    //
    decimalsRead = fscanf( f, "%d %d\n", &vertexNum1, &vertexNum2 );		// reading the first pair of edges
    while( decimalsRead == 2 )
    {
        addEdge( graph, vertexNum1, vertexNum2 );							// add edge between the vertices

        decimalsRead = fscanf( f, "%d %d\n", &vertexNum1, &vertexNum2 );	// reading the next pair of edges
    }

    // closing the file
    fclose( f );

    return graph;
}


//------------------------------------------------------------------------------------------
// Description: Checks if an eulerian path cannot exist by checking whether the graph has
// 				more than two vertices with uneven degree.
//------------------------------------------------------------------------------------------
GraphInformation checkVertexDegrees( Graph *graph )
{
    // We're optimistic and assume that two or less vertices with uneven degree at the beginning.
    GraphInformation graphInfo = {
        GRAPH_TYPE_NO_VERTICES_WITH_UNEVEN_DEGREE,
        VERTEX_UNDEFINED,
        VERTEX_UNDEFINED,
        VERTEX_UNDEFINED
    };
    int unevenDegreeCount = 0;
    int currVertexNum;
    int degree;
    int maxDegree = 0;
    int vertexWithMaxDegree = VERTEX_UNDEFINED;

    for( currVertexNum = 0; currVertexNum < graph->vertexCount; currVertexNum++ )			// loop through every vertex
    {
        degree = graph->vertices[ currVertexNum ]->degree;

        if( (degree % 2) > 0 )																// uneven degree?
        {
            //
            // Now that we've encountered a vertex with uneven degree, it must be a start or end vertex (as long as there're only two of those vertices).
            //
            unevenDegreeCount++;

            if( unevenDegreeCount == 1 )													// first vertex with uneven degrree
            {
                graphInfo.graphType = GRAPH_TYPE_TWO_VERTICES_WITH_UNEVEN_DEGREE;
                graphInfo.startOrEndVertexNum1 = currVertexNum;								// this vertex will be a possible start or end vertex of the eulerian path
            }
            else if( unevenDegreeCount == 2 )												// second vertex with uneven degrree
            {
                graphInfo.graphType = GRAPH_TYPE_TWO_VERTICES_WITH_UNEVEN_DEGREE;
                graphInfo.startOrEndVertexNum2 = currVertexNum;								// this vertex will be another possible start or end vertex of the eulerian path
            }
            else 																			// found more than 2 vertices with uneven degree?
            {
                graphInfo.graphType = GRAPH_TYPE_MORE_THAN_TWO_VERTICES_WITH_UNEVEN_DEGREE;
                graphInfo.startOrEndVertexNum1 = VERTEX_UNDEFINED;							// we reset these values because they wouldn't make sense now
                graphInfo.startOrEndVertexNum2 = VERTEX_UNDEFINED;
                break;																		// break loop
            }
        }

        // Update maximum degree.
        if( degree > maxDegree )
        {
            maxDegree = degree;
            vertexWithMaxDegree = currVertexNum;
        }
    }

    //
    // It could happen that we get a graph with no edges at all.
    //
    if( maxDegree <= 0 )
    {
        graphInfo.graphType = GRAPH_TYPE_ALL_VERTICES_WITH_ZERO_DEGREE;
    }
    else
    {
        graphInfo.vertexWithMaxDegree = vertexWithMaxDegree;
    }

    return graphInfo;
}

//------------------------------------------------------------------------------------------
// Description: Reduces the problem of finding an eulerian path to the problem of finding an
//				eulerian cycle by adding another vertex to the graph and connect the vertices with
//				uneven degree to that added vertex.
//------------------------------------------------------------------------------------------
void convertEulerianPathToCycle( Graph *graph, GraphInformation graphInfo )
{
    Vertex **extendedVertices;	// the new vertex array of the graph
    Vertex *addedVertex;		// the vertex that connects the two vertices with uneven degree
    int addedVertexNum;			// the number of the new vertex
    int oldVertexCount;
    int newVertexCount;
    int currVertexNum;

    oldVertexCount = graph->vertexCount;

    // Expend the vertex array with 1 vertex.
    newVertexCount = oldVertexCount + 1;
    extendedVertices = malloc( newVertexCount * sizeof( Vertex * ));
    assert( extendedVertices != NULL );

    // Copy old vertices to the new vertex array.
    for( currVertexNum = 0; currVertexNum < oldVertexCount; currVertexNum++ )
    {
        extendedVertices[ currVertexNum ] = graph->vertices[ currVertexNum ];
    }

    // Create the new vertex.
    addedVertex = malloc( sizeof(Vertex));
    assert( addedVertex != NULL );

    // Initializing the new vertex.
    addedVertex->degree = 0;
    addedVertex->firstEdge = NULL;
    addedVertex->lastEdge = NULL;

    // Add the new vertex to the array.
    addedVertexNum = oldVertexCount;
    extendedVertices[ addedVertexNum ] = addedVertex;

    // We can now free the old array and assign the new vertices to the graph.
    free( graph->vertices );
    graph->vertices = extendedVertices;
    graph->vertexCount = newVertexCount;

    // Finally add the edges between the possible start or end vertices and the new vertex.
    addEdge( graph, addedVertexNum, graphInfo.startOrEndVertexNum1 );
    addEdge( graph, addedVertexNum, graphInfo.startOrEndVertexNum2 );

    return;
}


//------------------------------------------------------------------------------------------
// Description: This function outputs an eulerian path. For the problem it uses the following reduction:
//				We actually only know how to find an eulerian cycle. But then our graph must have no vertices with uneven degree.
//				So in case we have two vertices with uneven degree (the minimum requirement for an eulerian path) we could simply
//				connect these vertices with an edge and calculate an eulerian cylce. After that we fit the path so that start and end vertices
//				are those two vertices with uneven degree. That's it.#
//
// Return:		EulerianCycleResult dataure which stores a flag whether a eulerian cycle exists and the path itself.
//------------------------------------------------------------------------------------------
EulerianCycleResult findEulerianCycle( Graph *graph, GraphInformation graphInfo )
{
    EulerianCycleResult result = { FALSE, NULL };					// the result data -> by defualt we assume that an eulerian path doesn't exist
    int mergingVertexNum = VERTEX_UNDEFINED;						// the vertex where the current path and the sub-circles are merged
    int degree;														// the degree of some vertex
    Path *currPath;													// the current path which will make up to the final result path
    PathElement *currElement;										// the current path element of the current path which looks for the next merging vertex
    Path *subCircle;												// a sub-circle of the graph

    // If the graph has two vertices with uneven degree, we add an edge between those so that we can find an eulerian cycle.
    if( graphInfo.graphType == GRAPH_TYPE_TWO_VERTICES_WITH_UNEVEN_DEGREE )
    {
        convertEulerianPathToCycle( graph, graphInfo );
    }

    // Set the first element of the path.
    mergingVertexNum = graphInfo.vertexWithMaxDegree;

    // We start by extracting a sub-circle as the base of our current path.
    currPath = extractSubCircle( graph, mergingVertexNum );
    assert( currPath != NULL );

    //
    // We go through through each element of the current path and extract sub-circle-paths from the graph which means we're deleting the edges
    // of where our path is walking on. These sub-circles are merged with the current path.
    // We do this until we reached the end of our self growing path and cannot add any sub-circle-paths.
    //
    currElement = currPath->firstElement;
    while( currElement != NULL )
    {
        // Get vertex number and degree of the vertex of the path where we want to merge the next sub-circle-path.
        mergingVertexNum = currElement->vertexNum;
        degree = graph->vertices[ mergingVertexNum ]->degree;

        // Only if the degree is greater than 0 we go on.
        if( degree > 0 )
        {
            // Extract a sub-circle-path.
            subCircle = extractSubCircle( graph, mergingVertexNum );
            assert( subCircle != NULL );

            // Merge the sub-circle-path with the current path.
            mergePaths( currPath, subCircle, &currElement );
        }

        currElement = currElement->next;
    }

    //
    // Now we have to check if there're no more edges inside the graph. This is important because it could be a disconnected
    // graph which we haven't been able to recognize yet.
    //
    if( !graphHasEdges( graph ))
    {
        result.exists = TRUE;
        result.eulerianCycle = currPath;
    }

    return result;
}


//------------------------------------------------------------------------------------------
// Description: This function returns a sub-circle path of the graph were startVertex is the start element of the path.
//				The edges of the sub-cricle will be removed on the given graph.
//
// Return:		The sub-circle path.
//------------------------------------------------------------------------------------------
Path *extractSubCircle( Graph *graph, int startVertexNum )
{
    Path *subCircle = NULL;					// the result sub-circle path
    Vertex *currVertex = NULL;				// the current vertex that we add to our path
    int currVertexNum;						// the number of the current vertex that we add to our path
    int nextVertexNum;						// the number of the next vertex
    Edge *edge = NULL;						// the first edge we find on the current vertex

    // Allocating memory to the path.
    subCircle = malloc( sizeof( Path ));
    assert( subCircle != NULL );

    // Initializing the path.
    subCircle->firstElement = NULL;
    subCircle->lastElement = NULL;

    // We append the first vertex to the sub-circle.
    appendPathElement( subCircle, startVertexNum );

    // Setting start values.
    currVertexNum = startVertexNum;
    currVertex = graph->vertices[ startVertexNum ];
    //
    // We only stop if we're back on the start vertex and there are no more edges left.
    //
    while( !( currVertexNum == startVertexNum && currVertex->degree == 0 ))
    {
        //
        // Due to the condition that we had a graph whos vertices have an even degree, we can assert
        // that the current vertex has at least one edge leading us to the next vertex.
        //
        assert( currVertex->degree > 0 );

        // We grab the first edge we can find.
        edge = currVertex->firstEdge;

        //
        // Now we're doing some optimization:
        // Our goal is to do as few path-merges as possible so we want to find the biggest possible sub-circle from our start vertex.
        // At this point we have to decide what edge we're taking to get to the next vertex.
        // If we're about to grab the starting vertex but the current vertex has still another vertex to jump on,
        // we won't take the start vertex because we want to keep expanding the sub-circle.
        //
        if( edge->toVertexNum == startVertexNum && currVertex->degree > 1 )		// we are grabbing the start vertex and still have other vertices to go to?
        {
            nextVertexNum = edge->nextEdge->toVertexNum;						// we do NOT take the start vertex because we want to keep extending
        }
        else 																	// in all the other cases...
        {
            nextVertexNum = edge->toVertexNum;									// ... we're just fine with first vertex we find
        }

        // Append the next vertex to the sub-circle.
        appendPathElement( subCircle, currVertexNum );

        // And now we remove the edges between the vertices so we won't take it again.
        removeEdge( graph, currVertexNum, nextVertexNum );

        // Finally we update the current vertex.
        currVertexNum = nextVertexNum;
        currVertex = graph->vertices[ nextVertexNum ];
    }

    return subCircle;
}


//------------------------------------------------------------------------------------------
// Description: Prints an eulerian cycle.
//------------------------------------------------------------------------------------------
void printEulerianCycle( Path *eulerianCycle )
{
    PathElement *currElement;

    currElement = eulerianCycle->firstElement;
    while( currElement != NULL )
    {
        printf( "%d ", currElement->vertexNum );

        currElement = currElement->next;
    }

    printf( "\n" );

    return;
}


//------------------------------------------------------------------------------------------
// Description: Prints an eulerian path.
//------------------------------------------------------------------------------------------
void printEulerianPath( Path *eulerianCylce, int addedVertexNum )
{
    PathElement *currElement;
    PathElement *addedVertexElement;

    //
    // Since we got an eulerian cyclce the first and the last elements are equal.
    // However this cannot occur on a graph which has 2 vertices with uneven degree.
    // So we will simply remove the last element of the path.
    //
    currElement = eulerianCylce->lastElement;
    assert( currElement->prev != NULL );
    currElement->prev->next = NULL;
    free( currElement );

    //
    // We have to find the added vertex element in the path.
    //
    currElement = eulerianCylce->firstElement;
    while( currElement != NULL )
    {
        if( currElement->vertexNum == addedVertexNum )
        {
            addedVertexElement = currElement;
            break;
        }

        currElement = currElement->next;
    }

    assert( addedVertexElement != NULL );

    //
    // Now we are going to print out the path.
    //
    while( TRUE )
    {
        currElement = currElement->next;

        // If we're at the end of the path, we jump back to the beginning.
        if( currElement == NULL )
        {
            currElement = eulerianCylce->firstElement;
        }

        // Stop if find the added vertex which we do NOT want to print obviously.
        if( currElement == addedVertexElement )
        {
            break;
        }

        printf( "%d ", currElement->vertexNum );
    }

    return;
}

//------------------------------------------------------------------------------------------
// Description: Adds a bidirectional edge between two vertices. Since we're working with undirectional graphs, we actually create
//				two edges given vertices v1 and v2: one from v1 to v2 and from v2 to v1.
//
// Return:		TRUE if we could successfully add an edge to a vertex else FALSE.
//------------------------------------------------------------------------------------------
void addEdge( Graph *graph, int vertexNum1, int vertexNum2 )
{
    Edge *edge1;
    Edge *edge2;

    edge1 = addSingleEdge( graph, vertexNum1, vertexNum2 );
    assert( edge1 != NULL );

    edge2 = addSingleEdge( graph, vertexNum2, vertexNum1 );
    assert( edge2 != NULL );

    // We tell each edge who its other corresponding edge is so we can easily remove them later without searching for it.
    edge1->otherEdge = edge2;
    edge2->otherEdge = edge1;

    return;
}


//------------------------------------------------------------------------------------------
// Description: Adds a single edge to a vertex which points to a specified vertex.
//------------------------------------------------------------------------------------------
Edge *addSingleEdge( Graph *graph, int fromVertexNum, int toVertexNum )
{
    Edge *edge;
    Vertex *fromVertex = graph->vertices[ fromVertexNum ];

    // Creating the edge.
    edge = malloc( sizeof( Edge ));
    assert( edge != NULL );

    // Set the vertex the edge points to.
    edge->toVertexNum = toVertexNum;

    //
    // Append the edge to the list of edges.
    //
    edge->nextEdge = NULL;
    edge->prevEdge = fromVertex->lastEdge;

    if( fromVertex->degree == 0 )
    {
        fromVertex->firstEdge = edge;
    }
    else
    {
        fromVertex->lastEdge->nextEdge = edge;
    }

    fromVertex->lastEdge = edge;
    fromVertex->degree++;

    return edge;
}


//------------------------------------------------------------------------------------------
// removeEdge()
//
// Description: Removes an edge between two vertices.
//
// Return:		TRUE if both onedirectional edges could be removed, FALSE if at least one could not be removed.
//------------------------------------------------------------------------------------------
void removeEdge( Graph *graph, int vertexNum1, int vertexNum2 )
{
    Vertex *vertex1;												// first vertex data pointer
    Vertex *vertex2;												// second vertex data pointer
    Edge *currEdge;													// the current edge we're looking at
    Edge *targetEdge;												// the edge to be removed (goes from vertex1 to vertex2)
    Edge *otherEdge;												// the edge that goes from vertex2 to vertex1

    vertex1 = graph->vertices[ vertexNum1 ];
    vertex2 = graph->vertices[ vertexNum2 ];

    //
    // We start by removing the edge from vertex1 to vertex2.
    // To do this we look through each edge until we find the one going to vertex2.
    // Then we set the next pointer of the previous edge to the next next pointer of the current edge
    // and free the current edge.
    //
    targetEdge = NULL;
    currEdge = vertex1->firstEdge;
    while( currEdge != NULL )
    {
        if( currEdge->toVertexNum == vertexNum2 )
        {
            targetEdge = currEdge;
            break;
        }

        currEdge = currEdge->nextEdge;
    }

    //
    // If we haven't find the given vertex in the edge list we simply stop.
    //
    assert( targetEdge != NULL );
    if( targetEdge == NULL )
    {
        return;
    }

    otherEdge = targetEdge->otherEdge;

    removeSingleEdge( vertex1, &targetEdge );
    removeSingleEdge( vertex2, &otherEdge );

    return;
}


//------------------------------------------------------------------------------------------
//
// Description:
//------------------------------------------------------------------------------------------
void removeSingleEdge( Vertex *vertex, Edge **edge )
{
    Edge *theEdge;

    assert( edge != NULL );

    theEdge = *edge;

    assert( theEdge != NULL );

    if( theEdge->prevEdge != NULL && theEdge->nextEdge != NULL )
    {
        theEdge->prevEdge->nextEdge = theEdge->nextEdge;
        theEdge->nextEdge->prevEdge = theEdge->prevEdge;
    }
    else if( theEdge->prevEdge == NULL && theEdge->nextEdge != NULL )
    {
        vertex->firstEdge = theEdge->nextEdge;
        theEdge->nextEdge->prevEdge = NULL;
    }
    else if( theEdge->prevEdge != NULL && theEdge->nextEdge == NULL )
    {
        theEdge->prevEdge->nextEdge = NULL;
        vertex->lastEdge = theEdge->prevEdge;
    }
    else
    {
        vertex->firstEdge = NULL;
        vertex->lastEdge = NULL;
    }

    free( theEdge );

    *edge = NULL;

    vertex->degree--;

    return;
}


//------------------------------------------------------------------------------------------
// graphHasEdges()
//
// Return:		TRUE if the given graph has edges, else FALSE.
//------------------------------------------------------------------------------------------
bool graphHasEdges( Graph *graph )
{
    int vertexNum;

    for( vertexNum = 0; vertexNum < graph->vertexCount; vertexNum++ )
    {
        if( graph->vertices[ vertexNum ]->degree > 0 )
        {
            return TRUE;
        }
    }

    return FALSE;
}


//------------------------------------------------------------------------------------------
// appendPathElement()
//
// Description:	Appends a path element at the end of a path (given a vertex as the value of the new element).
//
// Return:		Returns the appended path element.
//------------------------------------------------------------------------------------------
void appendPathElement( Path *path, int vertexNum )
{
    PathElement *newElement = NULL;

    newElement = malloc( sizeof( PathElement ));
    assert( newElement != NULL );

    newElement->vertexNum = vertexNum;
    newElement->next = NULL;

    // Connect it to the previous element.
    newElement->prev = path->lastElement;

    // Connect the previous element to it.
    if( path->firstElement == NULL )
    {
        path->firstElement = newElement;
    }
    else
    {
        path->lastElement->next = newElement;
    }

    // Update the last element pointer.
    path->lastElement = newElement;
}


//------------------------------------------------------------------------------------------
// mergePaths()
//
// Description:	Inserts all elements of a given path p2 inside a path p1 at a given vertex.
//------------------------------------------------------------------------------------------
void mergePaths( Path *path1, Path *path2, PathElement **mergingElement )
{
    PathElement *theMergingElement;

    assert( mergingElement != NULL );

    // Get the actual pointer to the data.
    theMergingElement = *mergingElement;

    assert( path1 != NULL && path2 != NULL && theMergingElement != NULL );

    // The merging element is neither the first nor the last element of path1.
    if( theMergingElement->prev != NULL && theMergingElement->next != NULL )
    {
        path2->firstElement->prev = theMergingElement->prev;
        theMergingElement->prev->next = path2->firstElement;

        path2->lastElement->next = theMergingElement->next;
        theMergingElement->next->prev = path2->lastElement;
    }
    // The merging element is the first element in path1.
    else if( theMergingElement->prev == NULL && theMergingElement->next != NULL )
    {
        path1->firstElement = path2->firstElement;

        path2->lastElement->next = theMergingElement->next;
        theMergingElement->next->prev = path2->lastElement;

    }
    // The merging element is the last element in path1.
    else if( theMergingElement->prev != NULL && theMergingElement->next == NULL )
    {
        path2->firstElement->prev = theMergingElement->prev;
        theMergingElement->prev->next = path2->firstElement;

        path1->lastElement = path2->lastElement;
    }
    // The merging element is the only element in path1.
    else
    {
        path1->firstElement = path2->firstElement;
        path1->lastElement = path2->lastElement;
    }

    // Free the merging element.
    free( theMergingElement );

    // Now we must update the input.
    *mergingElement = path2->firstElement;

    return;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

