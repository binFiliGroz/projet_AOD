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
            fprintf(stderr, "Number of elements in the dictionary: %ld\n", n);         
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

  freqFile = fopen(argv[2] , "r" );
  if (freqFile==NULL) {fprintf (stderr, "!!!!! Error opening originalFile !!!!!\n"); exit(EXIT_FAILURE);}

  // TO BE COMPLETED 
  fclose(freqFile);


  return 0;
}
