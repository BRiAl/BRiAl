/**
 * \file brilliantrussian.h
 * \brief M4RI and M4RM.
 *
 * \author Gregory Bard <bard@fordham.edu>
 * \author Martin Albrecht <M.R.Albrecht@rhul.ac.uk>
 *
 * \note For reference see Gregory Bard; Accelerating Cryptanalysis with
 * the Method of Four Russians; 2006;
 * http://eprint.iacr.org/2006/251.pdf
 */


#ifndef BRILLIANTRUSSIAN_H
#define BRILLIANTRUSSIAN_H
 /*******************************************************************
 *
 *                 M4RI:  Linear Algebra over GF(2)
 *
 *    Copyright (C) 2007, 2008 Gregory Bard <bard@fordham.edu>
 *    Copyright (C) 2008 Martin Albrecht <M.R.Albrecht@rhul.ac.uk>
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

#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "misc.h"
#include "packedmatrix.h"
#include "permutation.h"

/**
 * \brief Constructs all possible \f$2^k\f$ row combinations using the gray
 * code table.
 * 
 * \param M matrix to operate on
 * \param r the starting row
 * \param c the starting column (only exact up to block)
 * \param k
 * \param T prealloced matrix of dimension \f$2^k\f$ x m->ncols
 * \param L prealloced table of length \f$2^k\f$
 *
 * \wordoffset
 */

void mzd_make_table( packedmatrix *M, size_t r, size_t c, int k, packedmatrix *T, size_t *L);

/**
 * \brief The function looks up k bits from position i,startcol in each row
 * and adds the appropriate row from T to the row i. 
 *
 * This process is iterated for i from startrow to stoprow
 * (exclusive).
 *
 * \param M Matrix to operate on
 * \param startrow top row which is operated on
 * \param endrow bottom row which is operated on
 * \param startcol Starting column for addition
 * \param k M4RI parameter
 * \param T contains the correct row to be added
 * \param L Contains row number to be added
 *
 * \wordoffset
 */

void mzd_process_rows(packedmatrix *M, size_t startrow, size_t endrow, size_t startcol, int k, packedmatrix *T, size_t *L);

/**
 * \brief Same as mzd_process_rows but works with two Gray code tables
 * in parallel.
 *
 * \param M Matrix to operate on
 * \param startrow top row which is operated on
 * \param endrow bottom row which is operated on
 * \param startcol Starting column for addition
 * \param k M4RI parameter
 * \param T0 contains the correct row to be added
 * \param L0 Contains row number to be added
 * \param T1 contains the correct row to be added
 * \param L1 Contains row number to be added
 *
 * \wordoffset
 */

void mzd_process_rows2(packedmatrix *M, size_t startrow, size_t endrow, size_t startcol, int k, packedmatrix *T0, size_t *L0, packedmatrix *T1, size_t *L1);

/**
 * \brief Same as mzd_process_rows but works with three Gray code tables
 * in parallel.
 *
 * \param M Matrix to operate on
 * \param startrow top row which is operated on
 * \param endrow bottom row which is operated on
 * \param startcol Starting column for addition
 * \param k M4RI parameter
 * \param T0 contains the correct row to be added
 * \param L0 Contains row number to be added
 * \param T1 contains the correct row to be added
 * \param L1 Contains row number to be added
 * \param T2 contains the correct row to be added
 * \param L2 Contains row number to be added
 *
 * \wordoffset
 */

void mzd_process_rows3(packedmatrix *M, size_t startrow, size_t endrow, size_t startcol, int k, 
                       packedmatrix *T0, size_t *L0, packedmatrix *T1, size_t *L1,
                       packedmatrix *T2, size_t *L2);

/**
 * \brief Same as mzd_process_rows but works with four Gray code tables
 * in parallel.
 *
 * \param M Matrix to operate on
 * \param startrow top row which is operated on
 * \param endrow bottom row which is operated on
 * \param startcol Starting column for addition
 * \param k M4RI parameter
 * \param T0 contains the correct row to be added
 * \param L0 Contains row number to be added
 * \param T1 contains the correct row to be added
 * \param L1 Contains row number to be added
 * \param T2 contains the correct row to be added
 * \param L2 Contains row number to be added
 * \param T3 contains the correct row to be added
 * \param L3 Contains row number to be added
 *
 * \wordoffset
 */

void mzd_process_rows4(packedmatrix *M, size_t startrow, size_t endrow, size_t startcol, int k,
                       packedmatrix *T0, size_t *L0, packedmatrix *T1, size_t *L1,
                       packedmatrix *T2, size_t *L2, packedmatrix *T3, size_t *L3);

/**
 * \brief Matrix elimination using the 'Method of the Four Russians'
 * (M4RI).
 * 
 * \param M Matrix to be reduced.
 * \param full Return the reduced row echelon form, not only upper triangular form.
 * \param k M4RI parameter, may be 0 for auto-choose.
 * \param T Preallocated table, may be NULL for automatic creation.
 * \param L Preallocated lookup table, may be NULL for automatic creation.
 *
 * \example testsuite/test_elimination.c
 * \example testsuite/bench_elimination.c
 * 
 * \wordoffset
 */

int mzd_echelonize_m4ri(packedmatrix *M, int full, int k, packedmatrix *T, size_t *L);

/**
 * \brief Given a matrix in upper triangular form compute the reduced row
 * echelon form of that matrix.
 * 
 * \param M Matrix to be reduced.
 * \param k M4RI parameter, may be 0 for auto-choose.
 * \param T Preallocated table, may be NULL for automatic creation.
 * \param L Preallocated lookup table, may be NULL for automatic creation.
 *
 * \wordoffset
 */

void mzd_top_echelonize_m4ri(packedmatrix *M, int k, packedmatrix *T, size_t *L);

/**
 * \brief Invert the matrix M using Konrod's method. To avoid
 * recomputing the identity matrix over and over again, I may be
 * passed in as identity parameter.
 *
 * \param M Matrix to be reduced.
 * \param I Identity matrix.
 * \param k M4RI parameter, may be 0 for auto-choose.
 *
 * \wordoffset
 */

packedmatrix *mzd_invert_m4ri(packedmatrix *M, packedmatrix *I, int k);

/**
 * \brief Matrix multiplication using Konrod's method, i.e. compute C
 * such that C == AB. 
 * 
 * This is the convenient wrapper function, please see _mzd_mul_m4rm
 * for authors and implementation details.
 *
 * \param C Preallocated product matrix, may be NULL for automatic creation.
 * \param A Input matrix A
 * \param B Input matrix B
 * \param k M4RI parameter, may be 0 for auto-choose.
 *
 * \wordoffset
 */

packedmatrix *mzd_mul_m4rm(packedmatrix *C, packedmatrix *A, packedmatrix *B, int k);


/**
 * Set C to C + AB using Konrod's method.
 *
 * \param C Preallocated product matrix, may be NULL for zero matrix.
 * \param A Input matrix A
 * \param B Input matrix B
 * \param k M4RI parameter, may be 0 for auto-choose.
 *
 * \wordoffset
 */

packedmatrix *mzd_addmul_m4rm(packedmatrix *C, packedmatrix *A, packedmatrix *B, int k);

/**
 * \brief Matrix multiplication using Konrod's method, i.e. compute C such
 * that C == AB.
 * 
 * This is the actual implementation.
 *
 * \param C Preallocated product matrix.
 * \param A Input matrix A
 * \param B Input matrix B
 * \param k M4RI parameter, may be 0 for auto-choose.
 * \param clear clear the matrix C first
 *
 * \author Martin Albrecht -- initial implementation
 * \author William Hart -- block matrix implementation, use of several Gray code tables, general speed-ups
 *
 * \wordoffset
 */

packedmatrix *_mzd_mul_m4rm(packedmatrix *C, packedmatrix *A, packedmatrix *B, int k, int clear);

/**
 * \brief If defined 8 Gray code tables are used in parallel.
 */

#define M4RM_GRAY8

#endif //BRILLIANTRUSSIAN_H
