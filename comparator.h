/**
 * @file comparator.h
 * @author Philipp Badenhoop
 * @date 4 Jun 2016
 * @brief An object to compare data. As an example, it can be used to search data in a list.
 */


#ifndef COMPARATOR
#define COMPARATOR


#include "basic.h"


/**
 * @brief A function comparing data.
 * It returns true, if the data is matching the implemented compare criteria.
 */
typedef bool (*CompareFunction)( Data, Data );


/**
 * @brief An object to compare data.
 */
typedef struct
{
    Data            data;                   /**< Data stored, to compare with other data. */
    CompareFunction compareFunction;        /**< The compare function to evaluate if the stored data and some input data is matching the implemented compare criteria. */
} Comparator;


#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief Allocates and initializes a new comparator.
 * @param data
 * @param compareFunction
 * @return The pointer to the new comparator.
 */
Comparator *comparator_new( Data data, CompareFunction compareFunction );


/**
 * @brief Simply frees the comparator.
 */
void comparator_destroy( Comparator * );


/**
 * @brief Compares the stored data with input data.
 * @return true, if the stored and input data match the implemented compare criteria.
 */
bool comparator_compare( Comparator *, Data );


#ifdef __cplusplus
}
#endif


#endif // COMPARATOR

