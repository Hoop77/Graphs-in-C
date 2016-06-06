/**
 * @file path.h
 * @author Philipp Badenhoop
 * @date 6 Jun 2016
 * @brief A path of vertices of a graph.
 */


#ifndef PATH
#define PATH


#include "basic.h"
#include "dlist.h"
#include "pathelement.h"


/**
 * @brief This is just a container for a list of path elements which simply represent the vertices.
 */
typedef struct
{
    DList *elements;    /**< The list of path elements. */
} Path;


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Allocates and initializes the path.
 * @return  The pointer to the new path.
 */
Path *path_new( void );


/**
 * @brief Simply frees the pointer to the path.
 * @param path
 */
void path_destroy( Path *path );


/**
 * @brief Frees the pointer to the path and destroys all its elements too.
 * @param path
 */
void path_destroyAll( Path *path );


/**
 * @param path
 * @return The list of path elements.
 */
DList *path_getElements( Path *path );


/**
 * @brief Appends a path element with the specified vertex number to the path.
 * @param path
 * @param vertexNum
 */
void path_append( Path *path, int vertexNum );


#ifdef __cplusplus
}
#endif


#endif // PATH

