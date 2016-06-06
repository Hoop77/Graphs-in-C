/**
 * @file basic.h
 * @author Philipp Badenhoop
 * @date 6 Jun 2016
 * @brief Just some arbitrary definitions for the library.
 */


#ifndef BASIC
#define BASIC


#include "assert.h"
#include "stdlib.h"
#include "stdio.h"


/**
 * @brief Typedefing 'void *' to Data for better readablility.
 */
typedef void *Data;


// Custom bool definition for C.
#ifndef __cplusplus
typedef enum
{
    false = 0,
    true = !false
} bool;
#endif


#endif // BASIC

