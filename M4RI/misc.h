#ifndef MATRIX_H
#define MATRIX_H

/******************************************************************************
*
*            M4RI: Method of the Four Russians Inversion
*
*       Copyright (C) 2007 Gregory Bard <gregory.bard@ieee.org> 
*
*  Distributed under the terms of the GNU General Public License (GPL)
*
*    This code is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    General Public License for more details.
*
*  The full text of the GPL is available at:
*
*                  http://www.gnu.org/licenses/
******************************************************************************/

#include "grayflex.h"

/** code up: addToCell for XOR, like readCell and writeCell. **/

#define YES 1
#define NO 0

typedef unsigned char /* renamed as */ BIT;

struct matrixstruct {
  BIT *cells;
  int *rowswap;
  int *colswap;
  int nrows;
  int ncols;
};

typedef struct matrixstruct /* renamed as */ matrix;

extern BIT *table;
extern int *lookup;
extern int numcols;

void die(char *errormessage);

int min ( int a, int b);

/* MEMLEAK, use free */
void *safeCalloc( int count, int size );

/* MEMLEAK, use free */
void *safeMalloc( int count, int size );

BIT coinFlip();

#endif //MATRIX_H
