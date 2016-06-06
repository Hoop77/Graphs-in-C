//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// PROGRAM DESCRIPTION
// -------------------
//
// Author: Philipp Badenhoop
// Description: This program finds an Eulerian path on a given input graph.
// C-standard: C89,C90
//
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


#include "string.h"
#include "basic.h"
#include "edge.h"
#include "vertex.h"
#include "graph.h"
#include "path.h"
#include "pathelement.h"
#include "dlistiterator.h"
#include "comparator.h"


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// DEFINITIONS
// -----------


#define NDEBUG


// Just a simple flag which should indicate if a vertex number is not valid.
#define VERTEX_UNDEFINED	-1


typedef enum
{
    GRAPH_TYPE_NO_VERTICES_WITH_UNEVEN_DEGREE,
    GRAPH_TYPE_TWO_VERTICES_WITH_UNEVEN_DEGREE,
    GRAPH_TYPE_MORE_THAN_TWO_VERTICES_WITH_UNEVEN_DEGREE,
    GRAPH_TYPE_ALL_VERTICES_WITH_ZERO_DEGREE
} GraphType;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// DATA STRUCTURES
// ---------------

typedef struct
{
    GraphType graphType;
    int startOrEndVertexNum1;
    int startOrEndVertexNum2;
    int vertexWithMaxDegree;
} GraphInformation;


typedef struct
{
    bool exists;
    Path *eulerianCycle;
} EulerianCycleResult;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
// FUNCTION DECLARATION
// --------------------

#ifdef __cplusplus
extern "C" {
#endif

Graph *					loadGraph( char * );
GraphInformation 		checkVertexDegrees( Graph * );
void 					convertEulerianPathToCycle( Graph *, GraphInformation );
EulerianCycleResult 	findEulerianCycle( Graph *, GraphInformation );
Path * 					extractSubCircle( Graph *, int );

void 					printEulerianCycle( Path * );
void					printEulerianPath( Path *, int );

#ifdef __cplusplus
}
#endif

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


int main( int argc, char *argv[] )
{
    Graph *graph;

    // Check argument size: we specified that the first argument is the filename to our graph data.
    if( argc >= 2 )
    {
        // load graph
        graph = loadGraph( argv[1] );
    }
    else
    {
        fprintf( stderr, "Error: One argument for input file expected!\n" );
        return 0;
    }

    // We check if the graph was successfully loaded.
    if( graph == NULL )
        return 0;

    // Now that we have the graph in memory, we check if we canNOT find an eulerian path.
    // It has no eulerian path if more than two vertices have an uneven degree.
    GraphInformation graphInfo = checkVertexDegrees( graph );
    bool proceed = true;

    if( graphInfo.graphType != GRAPH_TYPE_NO_VERTICES_WITH_UNEVEN_DEGREE &&
        graphInfo.graphType != GRAPH_TYPE_TWO_VERTICES_WITH_UNEVEN_DEGREE )
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

        proceed = false;
    }

    // Now there's potential for an eulerian cycle but we cannot be completely sure because the graph must also be connected.
    // However, there might be some vertices that do not have any edges - which is valid. Our algorithm handles that.
    if( proceed )
    {
        EulerianCycleResult eulerianCycleResult = findEulerianCycle( graph, graphInfo );

        // Outputting.
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

            // Destroy complete the path.
            path_destroyAll( eulerianCycleResult.eulerianCycle );
        }
        else
        {
            printf( "An eulerian path does not exist.\nThe graph is disconnected.\n." );
            path_destroyAll( eulerianCycleResult.eulerianCycle );
        }
    }

    // Destroy the complete graph.
    graph_destroyAll( graph );

    return 0;
}


Graph *loadGraph( char *filename )
{
    // Opening the file.
    FILE *f = fopen( filename, "r" );
    if( f == NULL )
    {
        fprintf( stderr, "Could not read file!\n" );
        return NULL;
    }

    // We start by reading the number of vertices.
    int vertexCount;
    if( fscanf( f, "%d\n", &vertexCount ) != 1 )
    {
        fprintf( stderr, "Error: Input format incorrect!\n" );
        return NULL;
    }

    // Create the graph.
    Graph *graph = graph_new( vertexCount );

    // Now we are creating the edges by reading the file.
    // An edge in a file is a line with two deciaml numbers seperated by a whitespace.
    //
    // Make sure we read exactly 2 decimal in each line.
    int vertexNum1, vertexNum2;
    while( fscanf( f, "%d %d\n", &vertexNum1, &vertexNum2 ) == 2 )
    {
        if( vertexNum1 >= vertexCount || vertexNum2 >= vertexCount )
        {
            fprintf( stderr, "Error: Input format incorrect!\n" );
            return NULL;
        }

        // Create one edge going from vertex 1 to vertex 2
        // and one edge going from vertex 2 to vertex 1.
        graph_addEdgePair( graph, vertexNum1, vertexNum2 );
    }

    fclose( f );

    return graph;
}


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
    int maxDegree = 0;
    int vertexWithMaxDegree = VERTEX_UNDEFINED;

    // Loop through every vertex
    int currVertexNum;
    for( currVertexNum = 0; currVertexNum < graph_getVertexCount( graph ); currVertexNum++ )
    {
        int degree = vertex_getDegree( graph_getVertex( graph, currVertexNum ));

        // The vertex has an uneven degree?
        if( (degree % 2) > 0 )
        {
            // Now that we've encountered a vertex with uneven degree,
            // it must be a start or end vertex (as long as there're only two of these vertices).
            unevenDegreeCount++;

            // Found the first vertex with uneven degrree?
            if( unevenDegreeCount == 1 )
            {
                graphInfo.graphType = GRAPH_TYPE_TWO_VERTICES_WITH_UNEVEN_DEGREE;

                // This vertex will be a possible start or end vertex of the eulerian path.
                graphInfo.startOrEndVertexNum1 = currVertexNum;
            }
            // Found the second vertex with uneven degrree?
            else if( unevenDegreeCount == 2 )
            {
                // This vertex will be another possible start or end vertex of the eulerian path.
                graphInfo.startOrEndVertexNum2 = currVertexNum;
            }
            // Found more than 2 vertices with uneven degree?
            else
            {
                graphInfo.graphType = GRAPH_TYPE_MORE_THAN_TWO_VERTICES_WITH_UNEVEN_DEGREE;
                // We reset these values because they wouldn't make sense now.
                graphInfo.startOrEndVertexNum1 = VERTEX_UNDEFINED;
                graphInfo.startOrEndVertexNum2 = VERTEX_UNDEFINED;
                break;
            }
        }

        // Update maximum degree.
        if( degree > maxDegree )
        {
            maxDegree = degree;
            vertexWithMaxDegree = currVertexNum;
        }
    }

    // It could happen that we get a graph with no edges at all.
    if( maxDegree <= 0 )
        graphInfo.graphType = GRAPH_TYPE_ALL_VERTICES_WITH_ZERO_DEGREE;
    else
        graphInfo.vertexWithMaxDegree = vertexWithMaxDegree;

    return graphInfo;
}


void convertEulerianPathToCycle( Graph *graph, GraphInformation graphInfo )
{
    graph_addVertex( graph );

    // Add the edges between the possible start or end vertices and the new vertex.
    int addedVertexNum = graph_getVertexCount( graph ) - 1;
    graph_addEdgePair( graph, addedVertexNum, graphInfo.startOrEndVertexNum1 );
    graph_addEdgePair( graph, addedVertexNum, graphInfo.startOrEndVertexNum2 );

    return;
}


EulerianCycleResult findEulerianCycle( Graph *graph, GraphInformation graphInfo )
{
    // If the graph has two vertices with uneven degree, we add an edge between those so that we can find an eulerian cycle.
    if( graphInfo.graphType == GRAPH_TYPE_TWO_VERTICES_WITH_UNEVEN_DEGREE )
        convertEulerianPathToCycle( graph, graphInfo );

    // The path's first element will be the vertex with the max degree.
    int mergingVertexNum = graphInfo.vertexWithMaxDegree;

    // We start by extracting the first sub-circle of the graph as the base of our path.
    Path *path = extractSubCircle( graph, mergingVertexNum );

    // We go through each element of the current path and extract sub-circle-paths.
    // Note that when we obtain a sub-circle, all edges where that path came along will be removed,
    // so the graph keeps 'shrinking'.
    // Every sub-circle-path is merged with the current path after we obtained it.
    // We do this until we reached the end of our extending path and
    // are not able to extract any further sub-circles.
    //
    // First create an iterator to go through the elements of the extending path.
    DList *pathElements = path_getElements( path );
    DListIterator *pathIterator = dListIterator_new( pathElements );
    dListIterator_toFirst( pathIterator );

    // Until we reached the end of the path.
    while( !dListIterator_isAtEnd( pathIterator ))
    {
        PathElement *currElement = (PathElement *) dListIterator_get( pathIterator );

        // Get vertex number and degree of the vertex of the path where we want to merge the next sub-circle-path.
        int mergingVertexNum = pathElement_getVertexNum( currElement );
        int degree = vertex_getDegree( graph_getVertex( graph, mergingVertexNum ));

        // We skip this obviously if there're no (more) edges at the current vertex.
        if( degree > 0 )
        {
            // Extract a sub-circle-path.
            Path *subCircle = extractSubCircle( graph, mergingVertexNum );

            // Merge the current path with the sub-circle-path.
            dListIterator_merge( pathIterator, path_getElements( subCircle ));

            // Since we have merged the paths, we can get rid of the sub-circle instance.
            path_destroy( subCircle );
        }

        dListIterator_increment( pathIterator );
    }

    // Assume that we found a path.
    EulerianCycleResult result = { true, path };

    // Now we have to check if there're no more edges inside the graph.
    // This is important because it could be a disconnected graph
    // which we haven't been able to recognize yet.
    if( graph_hasEdges( graph ))
    {
        result.exists = false;
        result.eulerianCycle = NULL;
    }

    // Clean up.
    dListIterator_destroy( pathIterator );

    return result;
}


Path *extractSubCircle( Graph *graph, int startVertexNum )
{
    Path *subCircle = path_new();

    // We append the first vertex to the sub-circle.
    path_append( subCircle, startVertexNum );

    // Setting start values.
    int currVertexNum = startVertexNum;
    Vertex *currVertex = graph_getVertex( graph, startVertexNum );

    // We only stop if we're back on the start vertex and there are no more edges left.
    while( !( currVertexNum == startVertexNum && vertex_getDegree( currVertex ) == 0 ))
    {
        // Due to the condition that we had a graph whos vertices have an even degree, we can assert
        // that the current vertex has at least one edge leading us to the next vertex.
        assert( vertex_getDegree( currVertex ) > 0 );

        DList *edges = vertex_getEdges( currVertex );
        Edge *firstEdge = (Edge *) dList_get( edges, 0 );

        // Simply select the first edge to get to the next vertex.
        int nextVertexNum = edge_getToVertexNum( firstEdge );

        // Append the next vertex to the sub-circle.
        path_append( subCircle, nextVertexNum );

        // And now we remove the edges between the vertices so we won't take it again.
        graph_removeEdgePair( graph, currVertexNum, nextVertexNum );

        // Finally we update the current vertex.
        currVertexNum = nextVertexNum;
        currVertex = graph_getVertex( graph, nextVertexNum );
    }

    return subCircle;
}


void printEulerianCycle( Path *eulerianCycle )
{
    DListIterator *pathIterator = dListIterator_new( path_getElements( eulerianCycle ));

    dList_foreach( pathIterator )
    {
        PathElement *currElement = (PathElement *) dListIterator_get( pathIterator );

        printf( "%d ", currElement->vertexNum );
    }

    // Clean up.
    dListIterator_destroy( pathIterator );

    return;
}


void printEulerianPath( Path *eulerianCylce, int addedVertexNum )
{
    // Since we got an eulerian cyclce the first and the last elements are equal.
    // However this cannot occur on a graph which has 2 vertices with uneven degree.
    // So we will simply remove the last element of the path.
    DListIterator *pathIterator = dListIterator_new( path_getElements( eulerianCylce ));
    dListIterator_toLast( pathIterator );
    dListIterator_removeAndDestroy( pathIterator );

    // We have to find the added vertex element in the path.
    PathElement searchElement = { addedVertexNum };
    Comparator *comparator = comparator_new( &searchElement, (CompareFunction) pathElement_equals );

    dListIterator_toFirst( pathIterator );
    PathElement *addedVertexElement = (PathElement *) dListIterator_search( pathIterator, comparator );
    assert( addedVertexElement != NULL );

    // Now we are going to print out the path.
    while( true )
    {
        dListIterator_increment( pathIterator );

        // If we're at the end of the path, we jump back to the beginning.
        if( dListIterator_isAtEnd( pathIterator ))
            dListIterator_toFirst( pathIterator );

        PathElement *currElement = (PathElement *) dListIterator_get( pathIterator );

        // Stop if find the added vertex which we do NOT want to print obviously.
        if( currElement == addedVertexElement )
            break;

        printf( "%d ", pathElement_getVertexNum( currElement ));
    }

    // Clean up.
    comparator_destroy( comparator );
    dListIterator_destroy( pathIterator );

    return;
}
