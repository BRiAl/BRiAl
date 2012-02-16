/**
 * \file io.h
 * \brief Input/output routines for matrices
 *
 * \author Martin Albrecht <martinralbrecht@googlemail.com>
 */

#ifndef M4RI_IO_H
#define M4RI_IO_H

/*******************************************************************
*
*                M4RI: Linear Algebra over GF(2)
*
*    Copyright (C) 2011 Martin Albrecht <martinralbrecht@googlemail.com>
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

#include "m4ri_config.h"
#include "packedmatrix.h"

/**
 * \brief Print a matrix to stdout. 
 *
 * The output will contain colons between every 4-th column.
 *
 * \param M Matrix
 */

void mzd_print(mzd_t const *M);

/**
 * \brief Print compact information about the matrix to stdout. 
 *
 * Prints number of rows, number of columns, density (and rank).
 *
 * \param A Matrix
 * \param do_rank Also display the rank (expensive)
 */

void mzd_info(const mzd_t *A, int do_rank);

#if __M4RI_HAVE_LIBPNG

/**
 * \brief Read matrix from 1-bit PNG image.
 *
 * This function returns a matrix on success and NULL otherwise. 1-bit
 * Grayscale and 1-bit Palette images are supported.
 *
 * \param fn Filename
 * \param verbose Print error message to stdout if != 0
 */

mzd_t * mzd_from_png(const char *fn, int verbose);

/**
 * \brief Write matrix to 1-bit PNG image.
 *
 * This function returns zero on success and some value != 0
 * otherwise. The parameter compression_level takes a zlib compression
 * level, i.e., an integer betweeen -1 and 9 (inclusive) such that
 *
\verbatim
#define Z_NO_COMPRESSION         0
#define Z_BEST_SPEED             1
#define Z_BEST_COMPRESSION       9
#define Z_DEFAULT_COMPRESSION  (-1)
\endverbatim
 *
 * The optional comment string is written as a PNG comment. 
 *
 * 
 * \param A Matrix
 * \param fn Filename (must have write permission)
 * \param compression_level Zlib compression level (see above)
 * \param comment Optional comment (may be NULL)
 * \param verbose Print error message to stdout if != 0
 */

int mzd_to_png(const mzd_t *A, const char *fn, int compression_level, const char *comment, int verbose);

#endif //__M4RI_HAVE_LIBPNG


/**
 * \brief Read matrix from ASCII file in JCF format.
 *
 * The format is as follows:
\verbatim
nrows ncols modulus
nonzero_entries_upper_bound
column_index
\endverbatim
 *
 * where a negative column_index indicates a row increase by one.
 *
 * \param fn Filename
 * \param verbose Print error message to stdout if != 0
 */

mzd_t *mzd_from_jcf(const char *fn, int verbose);

#endif //M4RI_IO_H
