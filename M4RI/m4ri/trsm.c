/*******************************************************************
*
*                 M4RI: Linear Algebra over GF(2)
*
*    Copyright (C) 2008 Clement Pernet <clement.pernet@gmail.com>
*
*  Distributed under the terms of the GNU General Public License (GPL)
*  version 2 or higher.
*
*    This code is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*    General Public License for more details.
*
*  The full text of the GPL is available at:
*
*                  http://www.gnu.org/licenses/
*
********************************************************************/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "trsm.h"
#include "strassen.h"
#include "packedmatrix.h"
#include "parity.h"
#include "stdio.h"

#define TRSM_THRESHOLD m4ri_radix

/*****************
 * UPPER RIGHT
 ****************/

/* 
 * This version assumes that the matrices are at an even position on
 * the m4ri_radix grid and that their dimension is a multiple of m4ri_radix.
 */

void _mzd_trsm_upper_right_even(mzd_t const *U, mzd_t *B, const int cutoff);

/*
 * Variant where U and B start at an odd bit position. Assumes that
 * U->ncols < 64
 */

void _mzd_trsm_upper_right_weird(mzd_t const *U, mzd_t *B);

void _mzd_trsm_upper_right_base(mzd_t const *U, mzd_t *B);

void mzd_trsm_upper_right(mzd_t const *U, mzd_t *B, const int cutoff) {
  if(U->nrows != B->ncols)
    m4ri_die("mzd_trsm_upper_right: U nrows (%d) need to match B ncols (%d).\n", U->nrows, B->ncols);
  if(U->nrows != U->ncols)
    m4ri_die("mzd_trsm_upper_right: U must be square and is found to be (%d) x (%d).\n", U->nrows, U->ncols);
  
  _mzd_trsm_upper_right(U, B, cutoff);
}

void _mzd_trsm_upper_right(mzd_t const *U, mzd_t *B, const int cutoff) {
  rci_t const nb = B->ncols;
  rci_t const mb = B->nrows;
  int const n1 = m4ri_radix-B->offset;
  if(nb <= n1) {
    _mzd_trsm_upper_right_weird(U, B);
    return;
  }
  /**
   \verbatim  
     _________ 
     \U00|   |
      \  |U01|
       \ |   |
        \|___|
         \U11|
          \  |
           \ |
            \|
      _______
     |B0 |B1 |
     |___|___|
   \endverbatim
     * \li U00 and B0 are possibly located at uneven locations.
   * \li Their column dimension is lower than 64.
   * \li The first column of U01, U11, B1 are aligned at words.
   */
  mzd_t *B0  = mzd_init_window (B,  0,  0, mb, n1);
  mzd_t *B1  = mzd_init_window (B,  0, n1, mb, nb);
  mzd_t const *U00 = mzd_init_window_const (U,  0,  0, n1, n1);
  mzd_t const *U01 = mzd_init_window_const (U,  0, n1, n1, nb);
  mzd_t const *U11 = mzd_init_window_const (U, n1, n1, nb, nb);
  
  _mzd_trsm_upper_right_weird (U00, B0);
  mzd_addmul (B1, B0, U01, cutoff);
  _mzd_trsm_upper_right_even (U11, B1, cutoff);
  
  mzd_free_window(B0);
  mzd_free_window(B1);
  
  mzd_free_window((mzd_t*)U00);
  mzd_free_window((mzd_t*)U01);
  mzd_free_window((mzd_t*)U11);

  __M4RI_DD_MZD(B);
}

void _mzd_trsm_upper_right_weird(mzd_t const *U, mzd_t *B) {
  rci_t const mb = B->nrows;
  rci_t const nb = B->ncols;
  int const offset = B->offset;
  
  for(rci_t i = 1; i < nb; ++i) {
    
    /* Computes X_i = B_i + X_{0..i-1} U_{0..i-1,i} */
    
    register word ucol = 0;
    for(rci_t k = 0; k < i; ++k) {
      if(__M4RI_GET_BIT(U->rows[k][0], i + U->offset))
	__M4RI_SET_BIT(ucol, k + offset);
    }
    /* doing 64 dotproducts at a time, to use the parity64 parallelism */
    rci_t giantstep;
    word tmp[64];
    for(giantstep = 0; giantstep + m4ri_radix < mb; giantstep += m4ri_radix) {
      for(int babystep = 0; babystep < m4ri_radix; ++babystep)
	tmp[babystep] = B->rows[giantstep + babystep][0] & ucol;

      word const dotprod = m4ri_parity64(tmp);
      
      for(int babystep = 0; babystep < m4ri_radix; ++babystep)
        if(__M4RI_GET_BIT(dotprod, babystep))
          __M4RI_FLIP_BIT(B->rows[giantstep + babystep][0], i + offset);
    }      
    for(int babystep = 0; giantstep + babystep < mb; ++babystep){
      tmp[babystep] = B->rows[giantstep + babystep][0] & ucol;
      
    }
    for(int babystep = mb - giantstep; babystep < 64; ++babystep){
      tmp[babystep] = 0;
    }
    
    word const dotprod = m4ri_parity64(tmp);
    
    for(int babystep = 0; giantstep + babystep < mb; ++babystep)
      if(__M4RI_GET_BIT(dotprod, babystep))
	__M4RI_FLIP_BIT(B->rows[giantstep + babystep][0], i + offset);
  }

  __M4RI_DD_MZD(B);
}

void _mzd_trsm_upper_right_even(mzd_t const *U, mzd_t *B, const int cutoff) {
  rci_t const mb = B->nrows;
  rci_t const nb = B->ncols;

  if(nb <= TRSM_THRESHOLD) {
    /* base case */
    _mzd_trsm_upper_right_base (U, B);
    return;
  }
  
  rci_t const nb1 = (((nb - 1) / m4ri_radix + 1) >> 1) * m4ri_radix;
  
  mzd_t *B0 = mzd_init_window(B,  0,     0,   mb, nb1);
  mzd_t *B1 = mzd_init_window(B,  0,   nb1,   mb, nb);
  mzd_t const *U00 = mzd_init_window_const(U, 0,     0, nb1, nb1);
  mzd_t const *U01 = mzd_init_window_const(U, 0,   nb1, nb1, nb);
  mzd_t const *U11 = mzd_init_window_const(U, nb1, nb1,  nb, nb);
  
  _mzd_trsm_upper_right_even (U00, B0, cutoff);
  mzd_addmul (B1, B0, U01, cutoff);
  _mzd_trsm_upper_right_even (U11, B1, cutoff);
  
  mzd_free_window(B0);
  mzd_free_window(B1);
  
  mzd_free_window((mzd_t*)U00);
  mzd_free_window((mzd_t*)U01);
  mzd_free_window((mzd_t*)U11);

  __M4RI_DD_MZD(B);
}

void _mzd_trsm_upper_right_base(mzd_t const *U, mzd_t *B) {
  rci_t const mb = B->nrows;
  rci_t const nb = B->ncols;

  for(rci_t i = 1; i < nb; ++i) {
    /* Computes X_i = B_i + X_{0..i-1} U_{0..i-1,i} */
    register word ucol = 0;
    for(rci_t k = 0; k < i; ++k) {
      if(__M4RI_GET_BIT(U->rows[k][0], i))
	__M4RI_SET_BIT(ucol, k);
    }
    
    /* doing 64 dotproducts at a time, to use the m4ri_parity64 parallelism */
    rci_t giantstep;
    word tmp[64];
    for(giantstep = 0; giantstep + m4ri_radix < mb; giantstep += m4ri_radix) {
      for(int babystep = 0; babystep < m4ri_radix; ++babystep)
	tmp[babystep] = B->rows[giantstep + babystep][0] & ucol;
      
      word const dotprod = m4ri_parity64(tmp);
      
      for(int babystep = 0; babystep < m4ri_radix; ++babystep)
	if(__M4RI_GET_BIT(dotprod, babystep))
	  __M4RI_FLIP_BIT(B->rows[giantstep + babystep][0], i);
    }
    
    for(int babystep = 0; giantstep + babystep < mb; ++babystep)
      tmp[babystep] = B->rows[giantstep + babystep][0] & ucol;
    for(int babystep = mb - giantstep; babystep < 64; ++babystep)
      tmp[babystep] = 0;
    
    word const dotprod = m4ri_parity64(tmp);
    for(int babystep = 0; giantstep + babystep < mb; ++babystep)
      if(__M4RI_GET_BIT(dotprod, babystep))
	__M4RI_FLIP_BIT(B->rows[giantstep + babystep][0], i);
  }

  __M4RI_DD_MZD(B);
}


/*****************
 * LOWER RIGHT
 ****************/

/*
 * Variant where L and B start at an odd bit position Assumes that
 * L->ncols < 64
 */
void _mzd_trsm_lower_right_weird(mzd_t const *L, mzd_t *B);

/*
 * Variant where L and B start at an even bit position Assumes that
 * L->ncols < 64
 */

void _mzd_trsm_lower_right_even(mzd_t const *L, mzd_t *B, const int cutoff);

void _mzd_trsm_lower_right_base(mzd_t const *L, mzd_t *B);

void mzd_trsm_lower_right(mzd_t const *L, mzd_t *B, const int cutoff) {
  if(L->nrows != B->ncols)
    m4ri_die("mzd_trsm_lower_right: L nrows (%d) need to match B ncols (%d).\n", L->nrows, B->ncols);
  if(L->nrows != L->ncols)
    m4ri_die("mzd_trsm_lower_right: L must be square and is found to be (%d) x (%d).\n", L->nrows, L->ncols);
  
  _mzd_trsm_lower_right (L, B, cutoff);
}

void _mzd_trsm_lower_right(mzd_t const *L, mzd_t *B, const int cutoff) {
  rci_t const nb = B->ncols;
  rci_t const mb = B->nrows;
  int const n1 = m4ri_radix-B->offset;
  if(nb <= n1)
    _mzd_trsm_lower_right_weird (L, B);
  else{
  /**
   \verbatim  
     |\
     | \
     |  \
     |L00\
     |____\
     |    |\
     |    | \
     |    |  \
     |L10 |L11\
     |____|____\
      _________
     |B0  |B1  |
     |____|____|
   \endverbatim
   * \li L00 and B0 are possibly located at uneven locations.
   * \li Their column dimension is lower than 64.
   * \li The first column of L10, L11, B1 are aligned to words.
   */
    mzd_t *B0  = mzd_init_window (B,  0,  0, mb, n1);
    mzd_t *B1  = mzd_init_window (B,  0, n1, mb, nb);
    mzd_t const *L00 = mzd_init_window_const (L,  0,  0, n1, n1);
    mzd_t const *L10 = mzd_init_window_const (L,  n1, 0, nb, n1);
    mzd_t const *L11 = mzd_init_window_const (L, n1, n1, nb, nb);
    
    _mzd_trsm_lower_right_even (L11, B1, cutoff);
    mzd_addmul (B0, B1, L10, cutoff);
    _mzd_trsm_lower_right_weird (L00, B0);
    
    mzd_free_window(B0);
    mzd_free_window(B1);
    
    mzd_free_window((mzd_t*)L00);
    mzd_free_window((mzd_t*)L10);
    mzd_free_window((mzd_t*)L11);
  }

  __M4RI_DD_MZD(B);
}

void _mzd_trsm_lower_right_weird(mzd_t const *L, mzd_t *B) {
  rci_t const mb = B->nrows;
  rci_t const nb = B->ncols;
  int const offset = B->offset;
  
  for(rci_t i = nb - 1; i >= 0; --i) {
    
    /* Computes X_i = B_i + X_{i+1,n} L_{i+1..n,i} */
    
    register word ucol = 0;
    for(rci_t k = i + 1; k < nb; ++k) {
      if(__M4RI_GET_BIT(L->rows[k][0], i + L->offset))
	__M4RI_SET_BIT(ucol, k + offset);
    }
    /* doing 64 dotproducts at a time, to use the m4ri_parity64 parallelism */
    rci_t giantstep;
    word tmp[64];
    for(giantstep = 0; giantstep + m4ri_radix < mb; giantstep += m4ri_radix) {
      for(int babystep = 0; babystep < m4ri_radix; ++babystep)
	tmp[babystep] = B->rows[giantstep + babystep][0] & ucol;
      
      word const dotprod = m4ri_parity64(tmp);
      
      for(int babystep = 0; babystep < m4ri_radix; ++babystep)
        if(__M4RI_GET_BIT(dotprod, babystep))
          __M4RI_FLIP_BIT(B->rows[giantstep + babystep][0], i + offset);
    }      
    for(int babystep = 0; giantstep + babystep < mb; ++babystep){
      tmp[babystep] = B->rows[giantstep + babystep][0] & ucol;
    }
    for(int babystep = mb - giantstep; babystep < 64; ++babystep){
      tmp[babystep] = 0;
    }
    
    word const dotprod = m4ri_parity64(tmp);
    
    for(int babystep = 0; giantstep + babystep < mb; ++babystep)
      if(__M4RI_GET_BIT(dotprod, babystep))
	__M4RI_FLIP_BIT(B->rows[giantstep + babystep ][0], i + offset);
  }

  __M4RI_DD_MZD(B);
}

void _mzd_trsm_lower_right_even(mzd_t const *L, mzd_t *B, const int cutoff) {
  rci_t const mb = B->nrows;
  rci_t const nb = B->ncols;
  
  if(nb <= TRSM_THRESHOLD){
    /* base case */
    _mzd_trsm_lower_right_base (L, B);
  }
  else {
    rci_t const nb1 = (((nb - 1) / m4ri_radix + 1) >> 1) * m4ri_radix;

    mzd_t *B0 = mzd_init_window(B,  0,     0,   mb, nb1);
    mzd_t *B1 = mzd_init_window(B,  0,   nb1,   mb, nb);
    mzd_t const *L00 = mzd_init_window_const(L, 0,     0, nb1, nb1);
    mzd_t const *L10 = mzd_init_window_const(L, nb1, 0, nb, nb1);
    mzd_t const *L11 = mzd_init_window_const(L, nb1, nb1,  nb, nb);

    _mzd_trsm_lower_right_even (L11, B1, cutoff);
    mzd_addmul (B0, B1, L10, cutoff);
    _mzd_trsm_lower_right_even (L00, B0, cutoff);

    mzd_free_window(B0);
    mzd_free_window(B1);

    mzd_free_window((mzd_t*)L00);
    mzd_free_window((mzd_t*)L10);
    mzd_free_window((mzd_t*)L11);
  }

  __M4RI_DD_MZD(B);
}

void _mzd_trsm_lower_right_base(mzd_t const *L, mzd_t *B) {
  rci_t const mb = B->nrows;
  rci_t const nb = B->ncols;

  for(rci_t i = nb - 1; i >= 0; --i) {
    
    /* Computes X_i = B_i + X_{i+1,n} L_{i+1..n,i} */
    
    register word ucol = 0;
    for(rci_t k = i + 1; k < nb; ++k) {
      if(__M4RI_GET_BIT(L->rows[k][0], i))
	__M4RI_SET_BIT(ucol, k);
    }
    
    /* doing 64 dotproducts at a time, to use the parity64 parallelism */
    rci_t giantstep;
    word tmp[64];
    for(giantstep = 0; giantstep + m4ri_radix < mb; giantstep += m4ri_radix) {
      for(int babystep = 0; babystep < m4ri_radix; ++babystep)
	tmp[babystep] = B->rows[giantstep + babystep][0] & ucol;
      
      word const dotprod = m4ri_parity64(tmp);
      
      for(int babystep = 0; babystep < m4ri_radix; ++babystep)
	if(__M4RI_GET_BIT(dotprod, babystep))
	  __M4RI_FLIP_BIT(B->rows[giantstep + babystep][0], i);
    }
    
    for(int babystep = 0; giantstep + babystep < mb; ++babystep)
      tmp[babystep] = B->rows[giantstep + babystep][0] & ucol;
    for(int babystep = mb - giantstep; babystep < 64; ++babystep)
      tmp[babystep] = 0;
    
    word const dotprod = m4ri_parity64(tmp);
    for(int babystep = 0; giantstep + babystep < mb; ++babystep)
      if(__M4RI_GET_BIT(dotprod, babystep))
	__M4RI_FLIP_BIT(B->rows[giantstep + babystep][0], i);
  }

  __M4RI_DD_MZD(B);
}


/*****************
 * LOWER LEFT
 ****************/

/*
 * Variant where U and B start at an odd bit position. Assumes that
 * L->ncols < 64
 */

void _mzd_trsm_lower_left_weird(mzd_t const *L, mzd_t *B);

/* 
 * This version assumes that the matrices are at an even position on
 * the m4ri_radix grid and that their dimension is a multiple of m4ri_radix.
 */

void _mzd_trsm_lower_left_even(mzd_t const *L, mzd_t *B, const int cutoff);

void mzd_trsm_lower_left(mzd_t const *L, mzd_t *B, const int cutoff) {
  if(L->ncols != B->nrows)
    m4ri_die("mzd_trsm_lower_left: L ncols (%d) need to match B nrows (%d).\n", L->ncols, B->nrows);
  if(L->nrows != L->ncols)
    m4ri_die("mzd_trsm_lower_left: L must be square and is found to be (%d) x (%d).\n", L->nrows, L->ncols);
  
  _mzd_trsm_lower_left (L, B, cutoff);
}

void _mzd_trsm_lower_left(mzd_t const *L, mzd_t *B, const int cutoff) {
  if(!L->offset)
    _mzd_trsm_lower_left_even(L, B, cutoff);
  else{
    rci_t const nb = B->ncols;
    rci_t const mb = B->nrows;
    int const m1 = m4ri_radix - L->offset;
    if(mb <= m1) {
      _mzd_trsm_lower_left_weird (L, B);
      return;
    }
    /**
    \verbatim  
    |\           ______
    | \         |      |
    |  \        |  B0  |
    |L00\       |      |
    |____\      |______|
    |    |\     |      |
    |    | \    |      |
    |    |  \   |  B1  |
    |L10 |L11\  |      |
    |____|____\ |______|
    \endverbatim 
    * \li L00 L10 B0 and B1 are possibly located at uneven locations.
    * \li Their column dimension is lower than 64.
    * \li The first column of L01, L11, B1 are aligned to words.
    */
      
    mzd_t *B0  = mzd_init_window (B,  0,  0, m1, nb);
    mzd_t *B1  = mzd_init_window (B,  m1, 0, mb, nb);
    mzd_t const *L00 = mzd_init_window_const (L,  0,  0, m1, m1);
    mzd_t const *L10 = mzd_init_window_const (L,  m1, 0, mb, m1);
    mzd_t const *L11 = mzd_init_window_const (L, m1, m1, mb, mb);
    
    _mzd_trsm_lower_left_weird (L00, B0);
    mzd_addmul (B1, L10, B0, cutoff);
    _mzd_trsm_lower_left_even (L11, B1, cutoff);
    
    mzd_free_window(B0);
    mzd_free_window(B1);
    
    mzd_free_window((mzd_t*)L00);
    mzd_free_window((mzd_t*)L10);
    mzd_free_window((mzd_t*)L11);
  }

  __M4RI_DD_MZD(B);
}

void _mzd_trsm_lower_left_weird(mzd_t const *L, mzd_t *B) {
  rci_t const mb = B->nrows;
  rci_t const nb = B->ncols;
  int const Boffset = B->offset;
  int const nbrest = (nb + Boffset) % m4ri_radix;
  if(nb + B->offset > m4ri_radix) {

    // Large B
    word const mask_begin = __M4RI_RIGHT_BITMASK(m4ri_radix-B->offset);
    word const mask_end = __M4RI_LEFT_BITMASK(nbrest);

    // L[0,0] = 1, so no work required for i=0
    for(rci_t i = 1; i < mb; ++i) {
      
      /* Computes X_i = B_i + L_{i,0..i-1} X_{0..i-1}  */
      /**
       * \todo needs to be optimized!
       **/ 
      word *Lrow = L->rows[i];
      word *Brow = B->rows[i];

      for(rci_t k = 0; k < i; ++k) {
	if(__M4RI_GET_BIT(Lrow[0], k + L->offset)) {
	  Brow[0] ^= B->rows[k][0] & mask_begin;
	  for(wi_t j = 1; j < B->width - 1; ++j)
	    Brow[j] ^= B->rows[k][j];
          Brow[B->width - 1] ^= B->rows[k][B->width - 1] & mask_end;
	}
      }
    }
  } else { // Small B
    word const mask = __M4RI_MIDDLE_BITMASK(nb, B->offset);
    for(rci_t i = 1; i < mb; ++i) {
      /* Computes X_i = B_i + L_{i,0..i-1} X_{0..i-1}  */
      /**
       * \todo needs to be optimized!
       **/ 
      word *Lrow = L->rows[i];
      word *Brow = B->rows[i];

      for(rci_t k = 0; k < i; ++k) {
	if(__M4RI_GET_BIT(Lrow[0], k + L->offset)) {
	  Brow[0] ^= B->rows[k][0] & mask;
	}
      }
    }
  }

  __M4RI_DD_MZD(B);
}

void _mzd_trsm_lower_left_even(mzd_t const *L, mzd_t *B, const int cutoff) {
  rci_t const mb = B->nrows;
  rci_t const nb = B->ncols;
  int const Boffset = B->offset;
  int const nbrest = (nb + Boffset) % m4ri_radix;

  if(mb <= m4ri_radix){
    /* base case */

    if(nb + B->offset > m4ri_radix) {
      // B is large
      word const mask_begin = __M4RI_RIGHT_BITMASK(m4ri_radix-B->offset);
      word const mask_end = __M4RI_LEFT_BITMASK(nbrest);

      for(rci_t i = 1; i < mb; ++i) {
	/* Computes X_i = B_i + L_{i,0..i-1} X_{0..i-1}  */
        /**
         * \todo needs to be optimized!
         **/ 
	word *Lrow = L->rows[i];
	word *Brow = B->rows[i];

	for(rci_t k = 0; k < i; ++k) {
	  if(__M4RI_GET_BIT(Lrow[0], k)){
	    Brow[0] ^= B->rows[k][0] & mask_begin;
	    for(wi_t j = 1; j < B->width - 1; ++j)
	      Brow[j] ^= B->rows[k][j];
	    Brow[B->width - 1] ^= B->rows[k][B->width - 1] & mask_end;
	  }
	}
      }
    } else { // B is small
      word const mask = __M4RI_MIDDLE_BITMASK(nb, B->offset);
      for(rci_t i = 1; i < mb; ++i) {
	/* Computes X_i = B_i + L_{i,0..i-1} X_{0..i-1}  */
	/** Need to be optimized !!! **/
	word *Lrow = L->rows [i];
	word *Brow = B->rows [i];

	for(rci_t k = 0; k < i; ++k) {
	  if(__M4RI_GET_BIT(Lrow[0], k)){
	    Brow[0] ^= B->rows[k][0] & mask;
	  }
	}
      }
    }
  } else {
    rci_t const mb1 = (((mb - 1) / m4ri_radix + 1) >> 1) * m4ri_radix;

    mzd_t *B0 = mzd_init_window(B,  0,     0,   mb1, nb);
    mzd_t *B1 = mzd_init_window(B, mb1,    0,   mb,  nb);
    mzd_t const *L00 = mzd_init_window_const(L, 0,     0, mb1, mb1);
    mzd_t const *L10 = mzd_init_window_const(L, mb1,   0, mb, mb1);
    mzd_t const *L11 = mzd_init_window_const(L, mb1, mb1, mb, mb);

    _mzd_trsm_lower_left_even (L00, B0, cutoff);

    mzd_addmul (B1, L10, B0, cutoff);

    _mzd_trsm_lower_left_even (L11, B1, cutoff);

    mzd_free_window(B0);
    mzd_free_window(B1);

    mzd_free_window((mzd_t*)L00);
    mzd_free_window((mzd_t*)L10);
    mzd_free_window((mzd_t*)L11);
  }

  __M4RI_DD_MZD(B);
}

/*****************
 * UPPER LEFT
 ****************/

/*
 * Variant where U and B start at an odd bit position
 * Assumes that U->ncols < 64
 */
void _mzd_trsm_upper_left_weird (mzd_t const *U, mzd_t *B);

void _mzd_trsm_upper_left_even(mzd_t const *U, mzd_t *B, const int cutoff);

void mzd_trsm_upper_left(mzd_t const *U, mzd_t *B, const int cutoff) {
  if(U->ncols != B->nrows)
    m4ri_die("mzd_trsm_upper_left: U ncols (%d) need to match B nrows (%d).\n", U->ncols, B->nrows);
  if(U->nrows != U->ncols)
    m4ri_die("mzd_trsm_upper_left: U must be square and is found to be (%d) x (%d).\n", U->nrows, U->ncols);
  
  _mzd_trsm_upper_left (U, B, cutoff);
}

void _mzd_trsm_upper_left(mzd_t const *U, mzd_t *B, const int cutoff) {
  if(!U->offset)
    _mzd_trsm_upper_left_even (U, B, cutoff);
  else{
    rci_t const nb = B->ncols;
    rci_t const mb = B->nrows;
    int const m1 = m4ri_radix - U->offset;
    if(mb <= m1) {
      _mzd_trsm_upper_left_weird (U, B);
      return;
    }
    /**
     \verbatim
     __________   ______
     \ U00|    | |      |
      \   |U01 | |      |
       \  |    | |  B0  |
        \ |    | |      |
         \|____| |______|
          \    | |      |
           \U11| |      |
            \  | |  B1  |
             \ | |      |
              \| |______|
     \endverbatim 
     * \li U00, B0 and B1 are possibly located at uneven locations.
     * \li Their column dimension is greater than 64
     * \li The first column of U01, U11, B0 and B1 are aligned to words.
     */
    
    mzd_t *B0  = mzd_init_window (B,  0,  0, m1, nb);
    mzd_t *B1  = mzd_init_window (B,  m1, 0, mb, nb);
    mzd_t const *U00 = mzd_init_window_const (U,  0,  0, m1, m1);
    mzd_t const *U01 = mzd_init_window_const (U,  0, m1, m1, mb);
    mzd_t const *U11 = mzd_init_window_const (U, m1, m1, mb, mb);
    
    _mzd_trsm_upper_left_even (U11, B1, cutoff);
    mzd_addmul (B0, U01, B1, cutoff);
    _mzd_trsm_upper_left_weird (U00, B0);
    
    mzd_free_window(B0);
    mzd_free_window(B1);
    
    mzd_free_window((mzd_t*)U00);
    mzd_free_window((mzd_t*)U01);
    mzd_free_window((mzd_t*)U11);
  }

  __M4RI_DD_MZD(B);
}

void _mzd_trsm_upper_left_weird (mzd_t const *U, mzd_t *B) {
  rci_t const mb = B->nrows;
  rci_t const nb = B->ncols;
  int const Boffset = B->offset;
  int const nbrest = (nb + Boffset) % m4ri_radix;
  if(nb + Boffset > m4ri_radix) {

    // Large B
    word const mask_begin = __M4RI_RIGHT_BITMASK(m4ri_radix-B->offset);
    word const mask_end = __M4RI_LEFT_BITMASK(nbrest);

    // U[mb-1,mb-1] = 1, so no work required for i=mb-1
    for(rci_t i = mb - 2; i >= 0; --i) {
      
      /* Computes X_i = B_i + U_{i,i+1..mb} X_{i+1..mb}  */
      word *Urow = U->rows[i];
      word *Brow = B->rows[i];

      for(rci_t k = i + 1; k < mb; ++k) {
	if(__M4RI_GET_BIT(Urow[0], k + U->offset)) {
	  Brow[0] ^= B->rows[k][0] & mask_begin;
	  for(wi_t j = 1; j < B->width - 1; ++j)
	    Brow[j] ^= B->rows[k][j];
	  Brow[B->width - 1] ^= B->rows[k][B->width - 1] & mask_end;
	}
      }
    }
  } else { // Small B
    word const mask = __M4RI_MIDDLE_BITMASK(nb, B->offset);
    // U[mb-1,mb-1] = 1, so no work required for i=mb-1
    for(rci_t i = mb - 2; i >= 0; --i) {
      /* Computes X_i = B_i + U_{i,i+1..mb} X_{i+1..mb}  */
      word *Urow = U->rows[i];
      word *Brow = B->rows[i];

      for(rci_t k = i + 1; k < mb; ++k) {
	if(__M4RI_GET_BIT(Urow[0], k + U->offset)) {
          Brow[0] ^= B->rows[k][0] & mask;
	}
      }
    }
  }

  __M4RI_DD_MZD(B);
}

void _mzd_trsm_upper_left_even(mzd_t const *U, mzd_t *B, const int cutoff) {
  rci_t const mb = B->nrows;
  rci_t const nb = B->ncols;
  int const Boffset = B->offset;
  int const nbrest = (nb + Boffset) % m4ri_radix;

  if(mb <= m4ri_radix) {
    /* base case */
    
    if(nb + B->offset > m4ri_radix) {
      // B is large
      word const mask_begin = __M4RI_RIGHT_BITMASK(m4ri_radix-B->offset);
      word const mask_end = __M4RI_LEFT_BITMASK(nbrest);
      
      // U[mb-1,mb-1] = 1, so no work required for i=mb-1
      for(rci_t i = mb - 2; i >= 0; --i) {
        
        /* Computes X_i = B_i + U_{i,i+1..mb} X_{i+1..mb}  */
        word *Urow = U->rows[i];
        word *Brow = B->rows[i];
        
        for(rci_t k = i + 1; k < mb; ++k) {
          if(__M4RI_GET_BIT(Urow[0], k)){
            Brow[0] ^= B->rows[k][0] & mask_begin;
            for(wi_t j = 1; j < B->width - 1; ++j)
              Brow[j] ^= B->rows[k][j];
            Brow[B->width - 1] ^= B->rows[k][B->width - 1] & mask_end;
          }
        }
      }
    } else { // B is small
      word const mask = __M4RI_MIDDLE_BITMASK(nb, B->offset);
      // U[mb-1,mb-1] = 1, so no work required for i=mb-1
      for(rci_t i = mb - 2; i >= 0; --i) {
        
	/* Computes X_i = B_i + U_{i,i+1..mb} X_{i+1..mb}  */
	word *Urow = U->rows [i];
	word *Brow = B->rows [i];

	for(rci_t k = i + 1; k < mb; ++k) {
	  if(__M4RI_GET_BIT(Urow[0], k)){
	    Brow[0] ^= B->rows[k][0] & mask;
	  }
	}
      }
    }
  } else if(mb <= __M4RI_MUL_BLOCKSIZE) {
    _mzd_trsm_upper_left_even_m4r(U, B, 0);
  } else {
    rci_t const mb1 = (((mb-1) / m4ri_radix + 1) >> 1) * m4ri_radix;

    mzd_t *B0 = mzd_init_window(B,  0,     0,   mb1, nb);
    mzd_t *B1 = mzd_init_window(B, mb1,    0,   mb,  nb);
    mzd_t const *U00 = mzd_init_window_const(U, 0,     0, mb1, mb1);
    mzd_t const *U01 = mzd_init_window_const(U, 0,   mb1, mb1, mb);
    mzd_t const *U11 = mzd_init_window_const(U, mb1, mb1, mb, mb);

    _mzd_trsm_upper_left_even (U11, B1, cutoff);

    _mzd_addmul (B0, U01, B1, cutoff);

    _mzd_trsm_upper_left_even (U00, B0, cutoff);

    mzd_free_window(B0);
    mzd_free_window(B1);

    mzd_free_window((mzd_t*)U00);
    mzd_free_window((mzd_t*)U01);
    mzd_free_window((mzd_t*)U11);
  }

  __M4RI_DD_MZD(B);
}
