/**
 * \file pls_mmpf.h
 * \brief PLS and PLUQ factorization using Gray codes.
 *
 * \author Martin Albrecht <M.R.Albrecht@rhul.ac.uk>
 * 
 * \example testsuite/test_pluq.c
 */

#ifndef M4RI_PLS_MMPF_H
#define M4RI_PLS_MMPF_H

 /*******************************************************************
 *
 *                 M4RI:  Linear Algebra over GF(2)
 *
 *    Copyright (C) 2008-2010 Martin Albrecht <M.R.Albrecht@rhul.ac.uk>
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

#include "packedmatrix.h"
#include "permutation.h"

/**
 * \brief PLS matrix decomposition of A using Gray codes.
 *
 * Returns (P,L,S,Q) satisfying PLS = A where P is a permutation matrix
 * of dimension m x m, L is m x r unit lower triangular and S is an r
 * x n matrix which is upper triangular except that its columns are
 * permuted, that is S = UQ for U r x n upper triangular and Q is a n
 * x n permutation matrix. The matrix L and S are stored in place over
 * A.
 *
 * \param A Matrix.
 * \param P Preallocated row permutation.
 * \param Q Preallocated column permutation.
 * \param k Size of Gray code tables.
 *
 * \wordoffset
 *
 * \return Rank of A.
 */

rci_t _mzd_pls_mmpf(mzd_t *A, mzp_t *P, mzp_t *Q, int k);

/**
 * \brief PLUQ matrix decomposition of A using Gray codes.
 *
 * Returns (P,L,U,Q) satisfying PLUQ = A where P and Q are two
 * permutation matrices, of dimension respectively m x m and n x n, L
 * is m x r unit lower triangular and U is r x n upper triangular.
 *
 * \param A Matrix.
 * \param P Preallocated row permutation.
 * \param Q Preallocated column permutation.
 * \param k Size of Gray code tables.
 *
 * \wordoffset
 *
 * \return Rank of A.
 */

rci_t _mzd_pluq_mmpf(mzd_t *A, mzp_t *P, mzp_t *Q, int k);


/**
 * \brief PLE matrix decomposition of a submatrix for up to k columns
 * starting at (r,c).
 *
 * Updates P and Q and modifies A in place. The buffer done afterwards
 * holds how far a particular row was already eliminated.
 *
 * \param A Matrix.
 * \param start_row Row Offset.
 * \param stop_row Up to which row the matrix should be processed (exclusive).
 * \param start_col Column Offset.
 * \param k Size of Gray code tables.
 * \param P Preallocated row permutation.
 * \param Q Preallocated column permutation.
 * \param pivots which column holds the i-th pivot
 * \param done Preallocated temporary buffer.
 * \param done_row Stores the last row which is already reduced processed after function execution.
 * \param splitblock First block which is not considered by this function.
 *
 * \retval knar rank of the considered submatrix
 */

int _mzd_pls_submatrix(mzd_t *A, 
                       rci_t const start_row, rci_t const stop_row, 
                       rci_t const start_col, int const k, 
                       mzp_t *P, mzp_t *Q, rci_t *pivots, rci_t *done, rci_t *done_row,
                       wi_t const splitblock);

#endif // M4RI_PLS_MMPF_H
