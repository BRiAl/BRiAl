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

#include "brilliantrussian.h"

#define min(x,y) ((x < y)?x:y)
#define TWOPOW(i) (1<<(i))

int forceNonZero(packedmatrix *m, int xstart, int xstop, int y) {
  int i;

  for (i=xstart; i<=xstop; i++) {
    if (readCell(m, i, y)==1) {
      if (i!=xstart) rowSwap(m, i, xstart);
      return YES;
    }
  }
  return NO;
}


int prep(packedmatrix *m, int ai, int k) {
  int pc; /* pivot column */
  int tr; /* target row */
  int good;

  int rank = 0;

  for (pc=ai; pc<min(ai+k,m->ncols); pc++) {
    /* Step one, find a pivot row in this column.*/
    good=forceNonZero(m, pc, min( ai+k*3-1, m->nrows-1 ), pc);

    if (good==NO) return rank;

    for (tr=ai; tr<min(ai+k*3, m->nrows); tr++) {
      /* Step two, add this pivot row to other rows as needed. */
      if (tr==pc) continue;
      
      if (readCell(m, tr, pc)==0) continue;

      rowAddOffset(m, pc, tr, ai);
    }
    rank++;
  }

  return rank;
}

void combine( packedmatrix * s1, int row1, int startblock1, 
	      packedmatrix * s2, int row2, int startblock2,
	      packedmatrix * dest, int row3, int startblock3 ) {
  int wide=s1->width - startblock1;
  int i;

  word *b1_ptr = s1->values + startblock1 + s1->rowswap[row1];
  word *b2_ptr = s2->values + startblock2 + s2->rowswap[row2];
  word *b3_ptr;

  /* this is a quite likely case, and we treat is specially to ensure
   * cache register friendlyness. (Keep in mind that the x86 has only
   * four general purpose registers)
   */
  if( dest == s1 && row1 == row3 && startblock1 == startblock3) {
    /* A fair amount of time is spent in iterating i, thus we lower
     * the burden a bit here.
     */
    if(wide%2==0) {
      for(i = wide>>1 ; i > 0 ; i--) {
	*b1_ptr++ ^= *b2_ptr++;
	*b1_ptr++ ^= *b2_ptr++;
      }
      return;

    } else {
      for(i = wide ; i > 0 ; i--) {
	*b1_ptr++ ^= *b2_ptr++;
      }
      return;
    }

  } else {
    b3_ptr = dest->values + startblock3 + dest->rowswap[row3];

    for(i = 0 ; i < wide ; i++) {
      *b3_ptr++ = *b1_ptr++ ^ *b2_ptr++;
    }
    return;
  }
}

void makeTable( packedmatrix *m, int ai, int k,
			  packedmatrix *tablepacked, int *lookuppacked, int full) {
  int homeblock= full ? 0 : ai/RADIX;
  int i, rowneeded, id;
  int twokay= TWOPOW(k);

  lookuppacked[0]=0;

  for (i=1; i<twokay; i++) {
    rowneeded=codebook[k]->inc[i-1]+ai;

    id=codebook[k]->ord[i];

    lookuppacked[id]=i;

    combine( m,            rowneeded, homeblock, 
	     tablepacked,  i-1,       homeblock, 
	     tablepacked,  i,         homeblock);
  }
}


static INLINE int getBits(packedmatrix *m, int x, int y, int k) {
  int truerow = m->rowswap[x];
  int block;
  int spot;

  word temp;

  word *values = m->values;

  /* there are two possible situations. Either all bits are in one
   * word or they are spread across two words. */

  if ( (y%RADIX + k -1 ) < RADIX ) {
    /* everything happens in one word here */
    temp =  values[ y / RADIX + truerow ]; // get the value
    temp <<= y%RADIX; // clear upper bits
    temp >>= RADIX - k; // clear lower bits and move to correct position.
    return (int)temp;

  } else { 
    /* two words are affected */
    block = y / RADIX + truerow; // correct block
    spot = (y + k ) % RADIX; // correct offset
    // make room by shifting spot times to the right, and add stuff from the second word
    temp = (values[block] << spot) | ( values[block + 1] >> (RADIX - spot) ); 
    return ((int)temp & ((1<<k)-1)); // clear upper bits and return
   }
}

void processRow(packedmatrix *m, int row, int homecol, int k, packedmatrix *tablepacked, int *lookuppacked) {
  int blocknum = homecol/RADIX;

  int value = getBits(m, row, homecol, k);

  int tablerow = lookuppacked[value];

  combine(m,                row, blocknum,
	  tablepacked, tablerow, blocknum,
	  m,                row, blocknum);
}

void process(packedmatrix *m, int startrow, int stoprow, int startcol, int k, packedmatrix *tablepacked, int *lookuppacked) {
  int i;
  int blocknum=startcol/RADIX;
  int value;
  int tablerow;
  word *b1_ptr,*b2_ptr;

  // for optimization reasons we distinguish several cases here.

  switch(m->width - startcol/RADIX) {

  case 1:
    // no loop needed as only one block is operated on.
    for (i=startrow; i<=stoprow; i++) {
      value = getBits(m, i, startcol, k);
      tablerow = lookuppacked[value];
      b1_ptr = m->values + blocknum + m->rowswap[i];
      b2_ptr = tablepacked->values + blocknum + tablepacked->rowswap[tablerow];
      *b1_ptr ^= *b2_ptr;
    }
    break;

  case 2:
    // two blocks, no loop
    for (i=startrow; i<=stoprow; i++) {
      value = getBits(m, i, startcol, k);
      tablerow = lookuppacked[value];
      b1_ptr = m->values + blocknum + m->rowswap[i];
      b2_ptr = tablepacked->values + blocknum + tablepacked->rowswap[tablerow];
      *b1_ptr++ ^= *b2_ptr++;
      *b1_ptr ^= *b2_ptr;
    }
    break;

  default:
    // the real deal more than two blocks.
    for (i=startrow; i<=stoprow; i++) {
      processRow(m, i, startcol, k, tablepacked, lookuppacked);
    }
    break;
  }
}

int stepM4RI(packedmatrix *m, int full, int k, int ai, 
	     packedmatrix *tablepacked, int *lookuppacked) {
  int submatrixrank;
  
  /*
   * Stage 1: Denote the first column to be processed in a given
   * iteration as ai. Then, perform Gaussian elimination on the
   * first 3k rows after and including the i-th row to produce an
   *
   * identity matrix in $a_{(i,i)} ... a_{(i+k-1),(i+k-1)},$ 
   *
   * and zeroes in $a_{(i+k),i} ... a_{(i+3k-1),(i+k-1)}$.
   */

  submatrixrank=prep(m, ai, k);


  if (submatrixrank!=k) return submatrixrank;

  /*
   * Stage 2: Construct a table consisting of the 2k binary strings of
   * length k in a Gray Code.  Thus with only 2k vector additions, all
   * possible linear combinations of these k rows have been
   * precomputed.
   */

  makeTable(m, ai, k, tablepacked, lookuppacked, 0);


  /*
   * Stage 3: One can rapidly process the remaining rows from i + 3k
   * until row m (the last row) by using the table. For example,
   * suppose the jth row has entries $a_{(j,i)} ... a_{(j,i+k-1)}$ in
   * the columns being processed. Selecting the row of the table
   * associated with this k-bit string, and adding it to row j will
   * force the k columns to zero, and adjust the remaining columns
   * from i + k to n in the appropriate way, as if Gaussian
   * Elimination had been performed.
  */

  process(m, ai+k*3, m->nrows-1, ai, k,
	  tablepacked, lookuppacked);

  /* While the above form of the algorithm will reduce a system of
   * boolean linear equations to unit upper triangular form, and thus
   * permit a system to be solved with back substitution, the M4RI
   * algorithm can also be used to invert a matrix, or put the system
   * into reduced row echelon form (RREF). Simply run Stage 3 on rows
   * 0 ... i - 1 as well as on rows i + 3k · · · m. This only affects
   * the complexity slightly, changing the 2.5 coeffcient to 3
   */

  if (full==YES) process(m, 0, ai-1, ai, k, 
			 tablepacked, lookuppacked);

  return submatrixrank;
}

int reduceM4RI(packedmatrix *m, int full, int k, packedmatrix *tablepacked, int *lookuppacked) {
  int i, submatrixrank;
  int stop = min(m->nrows, m->ncols); 

  int rank = 0;
  int simple = 0;

  if (k == 0) {
    k = optK(m->nrows, m->ncols, 0);
  }

  if (tablepacked == NULL && lookuppacked == NULL) {
    simple = 1;
    tablepacked = createMatrix( TWOPOW(k), m->ncols );
    lookuppacked = (int *)safeCalloc( TWOPOW(k), sizeof(int) );
  }
  
  // main loop
  for (i=0; i<stop; i+=k) {
    // not enough room for M4RI left.
    if ( ((i+k*3) > m->nrows) || ((i+k) > m->ncols) ) {
      rank += reduceGaussianDelayed(m, i, full);
      break;
    }
    
    submatrixrank=stepM4RI(m, full, k, i, tablepacked, lookuppacked);

    if (submatrixrank!=k) {
      // not full rank, use Gaussian elimination :-(
      rank += reduceGaussianDelayed(m, i, full);
      break;
    }
    rank += submatrixrank;
  }

  if (simple) {
    free(lookuppacked);
    destroyMatrix(tablepacked);
  }
  
  return rank; 
}

void topReduceM4RI(packedmatrix *m, int k, packedmatrix *tablepacked, int *lookuppacked) {
  int i,j,  submatrixrank;
  int stop = min(m->nrows, m->ncols); 
  int simple = 0;
  
  if (k == 0) {
    k = optK(m->nrows, m->ncols, 0);
  }
  
  /* setup tables */
  if (tablepacked == NULL && lookuppacked == NULL) {
    simple = 1;
    tablepacked = createMatrix( TWOPOW(k), m->ncols );
    lookuppacked = (int *)safeCalloc( TWOPOW(k), sizeof(int) );
  }
  
  /* main loop */
  for (i=0; i<stop; i+=k) {
    if ( (i+k > m->nrows) || (i+k > m->ncols) ) {
      reduceGaussianDelayed(m, i, 1);
      break;
    }
    
    submatrixrank = prep(m, i, k);
    
    if (submatrixrank==k) {
      makeTable(m, i, k, tablepacked, lookuppacked, 0);
      process(m, 0, i-1, i, k, tablepacked, lookuppacked);
    } else {
      reduceGaussianDelayed(m, i, 1);
      break;
    }
  }
  
  /* clear tables */
  if (simple) {
    free(lookuppacked);
    destroyMatrix(tablepacked);
  }
}

packedmatrix *invertM4RI(packedmatrix *m, 
			 packedmatrix *identity, int k) {
  packedmatrix *big=concat(m, identity);
  int size=m->ncols;
  if (k == 0) {
    k = optK(m->nrows, m->ncols, 0);
  }
  int twokay=TWOPOW(k);
  int i;
  packedmatrix *mytable=createMatrix(twokay, size*2);
  int *mylookups=(int *)safeCalloc(twokay, sizeof(int));
  packedmatrix *answer;
  
  reduceM4RI(big, YES, k, mytable, mylookups);
  
  for(i=0; i < size; i++) {
    if (!readCell(big, i,i )) {
      answer=NULL;
      break;
    }
  }
  if (i == size)
    answer=copySubMatrix(big, 0, size, size-1, size*2-1);
  
  free(mylookups);
  destroyMatrix(mytable);
  destroyMatrix(big);
  
  return answer;
}

packedmatrix *multiplyM4RMTranspose(packedmatrix *A, packedmatrix *B, int k) {
  packedmatrix *AT, *BT, *CT, *C;
  
  if(A->ncols != B->nrows) die("A cols need to match B rows");
  
  AT = transpose(A);
  BT = transpose(B);
  
  CT = multiplyM4RM(BT,AT,k, NULL, NULL);
  
  destroyMatrix(AT);
  destroyMatrix(BT);

  C = transpose(CT);
  destroyMatrix(CT);
  return C;
}

packedmatrix *multiplyM4RM(packedmatrix *A, packedmatrix *B, int k, packedmatrix *tablepacked, int *lookuppacked) {
  int i,j,  a,b,c, simple;
  unsigned int x;
  packedmatrix *C;
  
  if(A->ncols != B->nrows) 
    die("A cols need to match B rows");
  
  a = A->nrows;
  b = A->ncols;
  c = B->ncols;

  if (k == 0) {
    k = optK(a,b,c);
  }

  if (tablepacked == NULL && lookuppacked == NULL) {
    simple = 1;
    tablepacked =createMatrix(TWOPOW(k), c);
    lookuppacked = (int *)safeCalloc(TWOPOW(k), sizeof(int));
  }

  C = createMatrix(a, c);

  for(i=0; i < b/k; i++) {

    /* Make a Gray Code table of all the $2^k$ linear combinations of
     * the $k$ rows of $B_i$.  Call the $x$-th row $T_x$.
     */
    makeTable( B, i*k, k, tablepacked, lookuppacked, 1 );
    
    for(j = 0; j<a; j++) {

      /* Read the entries 
       *
       *  $aj,(i-1)k+1, aj,(i-1)k+2 , ... , aj,(i-1)k+k.$
       *
       * Let $x$ be the $k$ bit binary number formed by the
       * concatenation of $aj,(i-1)k+1 , ... , aj,ik$.
       */

      x = lookuppacked[getBits(A, j, i*k, k)];

      /* for h = 1, 2, . . . , c do
       *   Calculate Cjh = Cjh + Txh.
       */
      combine( C,j,0,  tablepacked,x,0,  C,j,0 );
    }
  }

  //handle rest
  if (b%k) {
    makeTable( B, b/k * k , b%k, tablepacked, lookuppacked, 1);
    
    for(j = 0; j<a; j++) {
      x = lookuppacked[getBits(A, j, i*k, b%k)];
      combine(C,j,0, tablepacked,x,0,  C,j,0);
    }
  }
  if (simple) {
    destroyMatrix(tablepacked);
    free(lookuppacked);
  }
  return C;
}
