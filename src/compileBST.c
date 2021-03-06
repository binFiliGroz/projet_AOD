/*! \file compileBST.c
 *  \brief	   This implements the applyPatch program.
 *  \author    Lucie Pansart
 *  \author    Jean-Louis Roch
 *  \version   1.0
 *  \date      30/9/2016
 *  \warning   Usage: compileBST n originalFile 
 *  \copyright GNU Public License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <time.h>

struct noeud {
    long rg; // fils gauche
    long rd; // fils droit
};

typedef struct noeud NOEUD;
typedef NOEUD* ARBRE;

/**
 * pArbre function
 * \brief calculates the optimal tree
 * \param i  starting index of the tree
 * \param j  end index of the tree
 * \param freqAddTab  cumulative frequencies
 * \param pTab  average depths
 * \param rTab  roots
 * \returns { average depth of the tree }
 */
long pArbre(long i, long j, long *freqAddTab, long **pTab, long **rTab) {
    long k, km, pm, p, start, end;

    if (i > j) 
	return 0;
    
    // renvoi de la valeur en mémoire si calcul déjà réalisé
    if (pTab[i][j] != -1)
	return pTab[i][j];

    if (i != j) {
	pm = -1;

	// restriction des bornes de recherche
	if (rTab[i][j-1] == -1)
	    start = i;
	else 
	    start = rTab[i][j-1];

	if (rTab[i+1][j] == -1)
	    end = j;
	else
	    end = rTab[i+1][j];

	// recherche de la racine qui minimise la profondeur moyenne
	for(k = start; k <= end; k++) {
	    p = pArbre(i, k-1, freqAddTab, pTab, rTab)
		+ pArbre(k+1, j, freqAddTab, pTab, rTab);

	    if (pm >= p || pm == -1) {
		km = k;
		pm = p;
	    }
	}

	pm += freqAddTab[j+1] - freqAddTab[i];
    }

    else {
	pm = freqAddTab[i+1] - freqAddTab[i];
	km = i;
    }

    // stockage de la valeur en mémoire
    rTab[i][j] = km;
    pTab[i][j] = pm;

    return pm;
}

/**
 * reconstruitArbre function
 * \brief create tree structure
 * \param i starting index of the tree
 * \param j end index of the tree
 * \param rTab  roots
 * \param a tree structure
 * \returns {}
 */
void reconstruitArbre(long i, long j, long **rTab, ARBRE a) {
    long root = rTab[i][j], rg = -1, rd = -1;

    root = rTab[i][j];

    if (root != i) {
        rg = rTab[i][root-1];
        reconstruitArbre(i, root-1, rTab, a);
    }

    if (root != j) {
        rd = rTab[root+1][j];
        reconstruitArbre(root+1, j, rTab, a);
    }

    (a+root)->rg = rg;
    (a+root)->rd = rd;
}

/**
 * afficheArbre function
 * \brief print tree structure
 * \param n number of elements
 * \param a tree structure
 * \returns {}
 */
void afficheArbre(long n, ARBRE a) {
    long i;

    printf("static int BSTtree[%ld][2] = {\n", n);
    for (i = 0; i < n-1; ++i) {
	printf("{%ld, %ld}, \n", (a+i)->rg, (a+i)->rd);
    }
    printf("{%ld, %ld} };\n", (a+i)->rg, (a+i)->rd);
}


/**
 * Main function
 * \brief Main function
 * \param argc  A count of the number of command-line arguments
 * \param argv  An argument vector of the command-line arguments.
 * \warning Must be called with a positive long integer, n,  and a filename, freqFile, as commandline parameters and in the given order.
 * \returns { 0 if succeeds and prints C code implementing an optimal ABR on stdout; exit code otherwise}
 */
int main (int argc, char *argv[]) {
  long n = 0 ; // Number of elements in the dictionary
  FILE *freqFile = NULL ; // File that contains n positive integers defining the relative frequence of dictinary elements 
  clock_t start, stop;
  double duration;
  long i, j;
  long freqSum, freqTmp;
  long *freqAddTab;
  long **pTab, **rTab;
  ARBRE a;
  
  if(argc != 3){
    fprintf(stderr, "!!!!! Usage: ./compileBST n  originalFile !!!!!\n");
      exit(EXIT_FAILURE); /* indicate failure.*/
  }

  { // Conversion of parameter n in a long 
    int codeRetour = EXIT_SUCCESS;
    char *posError;
    long resuLong;
    n = atol(argv[1] ) ;
   
    assert(argc >= 2);
    // Conversion of argv[1] en long
    resuLong = strtol(argv[1], &posError, 10);
    // Traitement des erreurs
    switch (errno)
    {
      case EXIT_SUCCESS :
         // Conversion du long en int
         if (resuLong > 0)
         {
            n = (long)resuLong;
            //fprintf(stderr, "Number of elements in the dictionary: %ld\n", n);         
         }
         else
         {
            (void)fprintf(stderr, "%s cannot be converted into a positive integer matching the number of elements in the dicionary.\n", argv[1]) ; 
            codeRetour = EXIT_FAILURE;
         }
      break;
      
      case EINVAL :
         perror(__func__);
         (void)fprintf(stderr, "%s does not match a long integer value. \n", argv[1]);
         codeRetour = EXIT_FAILURE;
      break;
      
      case ERANGE :
         perror(__func__);
         (void)fprintf(stderr, "%s does not fit in a long int :\n" "out of bound [%ld;%ld]\n",
                       argv[1], LONG_MIN, LONG_MAX);
         codeRetour = EXIT_FAILURE;
      break;
      default :
         perror(__func__);
         codeRetour = EXIT_FAILURE;
    } // switch (errno)
    if  (codeRetour != EXIT_SUCCESS) return codeRetour ;
  }

  // ouverture du fichier
  freqFile = fopen(argv[2] , "r" );
  if (freqFile==NULL) {fprintf (stderr, "!!!!! Error opening originalFile !!!!!\n"); exit(EXIT_FAILURE);}

  // lecture des fréquences 
  // puis mise en mémoire sous forme cumulée
  freqAddTab = malloc(sizeof(*freqAddTab)*(n+1));
  freqAddTab[0] = 0;
  freqSum = 0;
  for(i=1; i<=n; i++) {
      fscanf(freqFile, "%ld ", &freqTmp);
      freqSum += freqTmp;
      freqAddTab[i] = freqSum;
  }

  // fermture du fichier
  fclose(freqFile);

  // allocation du tableau des profondeurs moyennes
  pTab = malloc(n*sizeof(*pTab));
  *pTab = malloc(n*n*sizeof(*pTab));
  for(i=1; i<n; i++)
      pTab[i] = pTab[i-1] + n;
  for(i=0; i<n; i++)
      for(j=0; j<n; j++)
	  pTab[i][j] = -1;

  // allocation du tableau des racines
  rTab = malloc(n*sizeof(*rTab));
  *rTab = malloc(n*n*sizeof(*rTab));
  for(i=1; i<n; i++)
      rTab[i] = rTab[i-1] + n;
  for(i=0; i<n; i++)
      for(j=0; j<n; j++)
	  rTab[i][j] = -1;

  a = malloc(n*sizeof(*a));

  start = clock();

  printf("static long BSTdepth = %ld; // pour info. Non demandé\n", pArbre(0, n-1, freqAddTab, pTab, rTab));
  printf("static int BSTroot = %ld;\n", rTab[0][n-1]);

  reconstruitArbre(0, n-1, rTab, a);
  afficheArbre(n, a);


/*
  printf("\nAffichage des profondeurs moyennes :\n");
  for(i=0; i<n; i++) {
      for(j=0; j<n; j++)
	  printf(" %ld", pTab[i][j]);
      printf("\n");
  }

  printf("\nAffichage des racines :\n");
  for(i=0; i<n; i++) {
      for(j=0; j<n; j++)
	  printf(" %ld", rTab[i][j]);
      printf("\n");
  }
*/

  stop = clock();
  duration = (double)(stop - start) / CLOCKS_PER_SEC;
  fprintf(stderr, "%3.6f secondes\n", duration);

  free(*pTab);
  free(pTab);
  free(*rTab);
  free(rTab);
  free(freqAddTab);
  free(a);

  return 0;
}
