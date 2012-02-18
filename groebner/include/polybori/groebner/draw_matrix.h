// -*- c++ -*-
//*****************************************************************************
/** @file draw_matrix.h 
 *
 * @author Michael Brickenstein
 * @date 2011-07-01
 *
 * This file includes the definition of the function @c draw_matrix.
 *
 * @par Copyright:
 *   (c) 2006-2011 by The PolyBoRi Team
 *
**/
//*****************************************************************************

#ifndef polybori_groebner_draw_matrix_h_
#define polybori_groebner_draw_matrix_h_

// include basic definitions
#include "groebner_defs.h"

#ifdef PBORI_HAVE_GD
#define DRAW_MATRICES
#endif
#ifdef DRAW_MATRICES
#include <stdio.h>
#include <gd.h>
#endif

BEGIN_NAMESPACE_PBORIGB


inline void
draw_matrix(mzd_t* mat, const char* filename){
    #ifdef DRAW_MATRICES
    int i,r,c,j;
    c=mat->ncols;
    r=mat->nrows;
    gdImagePtr im = gdImageCreate(c, r) ;
     FILE * out = fopen(filename, "wb") ;
 int black = gdImageColorAllocate(im, 0, 0, 0) ;
 int white = gdImageColorAllocate(im, 255, 255, 255); 
 gdImageFilledRectangle(im, 0, 0, c-1, r-1, white) ;
 
 for(i=0;i<r;i++){
     for(j=0;j<c;j++){
         if (mzd_read_bit(mat, i, j))
             gdImageSetPixel(im, j, i, black );
     }
 }
  

 gdImagePng(im, out);
 gdImageDestroy(im);
 fclose(out);
 #else
 std::cerr<<"warning: for drawing matrices compile with GD";
 #endif
}

END_NAMESPACE_PBORIGB

#endif /* polybori_groebner_draw_matrix_h_ */
