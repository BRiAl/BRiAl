#include <strings.h>
#include "m4ri.h"

//good 36586 = 1.338535 billion
//bad  36687 = 1.345936 billion
// 1338 million BITs is "safe".

int verbose, silent, dimension, number;
packedmatrix *identitymatrix;

unsigned long long tcc,twc;


/****************************************************************/
int experiment(int size) {
  int rank;
  packedmatrix *mymatrix, *bigmatrix1, *bigmatrix2;
  clock_t russiantime;
 
  printf("/**************************************/\n");
  printf("Starting experiment.\n");

  printf("Allocating.\n");
  mymatrix=createPackedMatrix(size,size);

  printf("Filling.\n");
  fillRandomlyPacked(mymatrix);

  //printf("Concating.\n");
  bigmatrix1=clonePacked(mymatrix);//CONCATMATRIX(mymatrix, identitymatrix);
  bigmatrix2=clonePacked(mymatrix);// CONCATMATRIX(mymatrix, identitymatrix);

  if(verbose) 
    lazyPrint(bigmatrix2);

  printf("Executing 4 Russians.\n");
  startWatch();
  rank = simpleFourRussiansPackedFlex(bigmatrix1, YES, 8);
  stopWatch();
  russiantime=getWatch();

  printf("Russian rank %d\n",rank);

  if(verbose)
    lazyPrint(bigmatrix1);

  printf("The Russian and alg took %d ms.\n",
	   (int)russiantime/1000);
  destroyPackedMatrix(mymatrix);
  destroyPackedMatrix(bigmatrix1);
  destroyPackedMatrix(bigmatrix2);

  return NO;
}

/****************************************************************/
void processArgs( int argc, char **argv ) {
  int i;

  verbose=0; silent=0;

  /* printf("There are %d args.\n", argc-1);
  for (i=0; i<argc; i++) {
    printf("Arg %d: %s\n", i, argv[i]);
    }*/

  if (argc<3) die("Usage: %s DIMENSION EXPERIMENTS [-s -v]");

  dimension=atoi(argv[1]);
  number=atoi(argv[2]);

  for (i=3; i<argc; i++) {
    if (strcasecmp(argv[i], "-v")==0) verbose=YES;
    else if (strcasecmp(argv[i], "-s")==0) silent=YES;
    else { printf("Unknown arg: \"%s\"\n", argv[i]); die("Bad Args");}
  }

  printf("Dim=%d, Num=%d, Silent=%d, Verbose=%d.\n", dimension, number, silent, verbose);

  if (dimension<1) die("Bad Dimension.");
  if (number<1) die("Bad Number.");

  if ((verbose==YES) && (silent==YES)) die("Verbose and Silnet are incompatible.\n");
}

/****************************************************************/


void printGrayTableFlex(int k) {
  int i;
 for(i=0; i< 1<<k; i++) {
    printBitString(codebook[k]->ord[i],k);
  }

};

int main ( int argc, char **argv) {
  int i;
  int count=0;

  tcc = 0;
  twc = 0;

  clock();

  processArgs(argc, argv);

  setupPackingMasks(); 
  seedWatch(); 
  buildAllCodes(); 

  identitymatrix=createPackedMatrix(dimension, dimension);
  makeIdentityPacked(identitymatrix);

  clearLogs();

  seedWatch();
  
  for (i=0; i<number; i++) {
    count+=experiment(dimension);
  }

  destroyPackedMatrix(identitymatrix);
  destroyAllCodes();

  printf("cycles: %lld, words: %lld, cycles/word: %f\n",tcc,twc,tcc/(double)twc);

  return 0;
}





