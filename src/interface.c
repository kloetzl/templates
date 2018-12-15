/***** interface.c ************************************************
 * Description: Routine for gathering arguments from the command
 *              line.
 * Author: Bernhard Haubold, haubold@evolbio.mpg.de
 * Date: Sun Jun 20 13:12:10 2004.
 *****************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "interface.h"
#include "eprintf.h"

Args *newArgs() {
  Args *args = (Args *)emalloc(sizeof(Args));
  args->h = 0;
  args->v = 0;
  args->e = 0;
  args->U = 0;
  args->t = 0;
  args->u = DEFAULT_U;
  args->c = DEFAULT_C;
  args->l = 0;
  args->E = DEFAULT_E;
  args->L = NULL;
  args->al = (int *)emalloc(3 * sizeof(int));
  args->nl = 0;
  return args;
}

void freeArgs(Args *args) {
  if(args->L != NULL)
    free(args->L);
  free(args->al);
  free(args);
}

void extractLevels(Args *args) {
  char *c;
  c = strtok(args->L, ",");

  int i = atoi(c);
  if(i != 2) {
    args->nl++;
    args->al[args->nl] = 2;
  }
  args->nl++;
  args->al[args->nl] = i;
  while((c = strtok(NULL, ",")) != NULL){
    i = atoi(c);
    args->nl++;
    args->al = (int *)erealloc(args->al, (args->nl + 1) * sizeof(int));
    args->al[args->nl] = i;
  }
}

Args *getArgs(int argc, char *argv[]){
  int c;
  char *optString = "hvUtu:l:L:c:E:";

  Args *args = newArgs();
  c = getopt(argc, argv, optString);
  while(c != -1){
    switch(c){
      break;
    case 'l':                           /* sequence length */
      args->l = atoi(optarg);
      break;
    case 'L':                           /* preset levels */
      args->L = estrdup(optarg);
      break;
    case 'E':                           /* levels of exhaustive search */
      args->E = atoi(optarg);
      break;
    case 'U':                           /* unfolded */
      args->U = 1;
      break;
    case 't':                           /* testing? */
      args->t = 1;
      break;
    case 'u':
      args->u = atof(optarg);           /* mutation rate */
      break;
    case 'c':
      args->c = atof(optarg);           /* minimum logLik change for acceptance of new level */
      break;
    case '?':                           /* fall-through is intentional */
    case 'h':                           /* print help */
      args->h = 1;
      break;
    case 'v':                           /* print version */
      args->v = 1;
      break;
    default:
      printf("# unknown argument: %c\n",c);
      args->e = 1;
      return args;
    }
    c = getopt(argc, argv, optString);
  }
  args->inputFiles = argv + optind;
  args->numInputFiles = argc - optind;
  if(args->L)
    extractLevels(args);
  return args;
}


void printUsage(){
  printf("Usage: %s [options] [inputFiles]\n", progname());
  printf("<DESCRIPTION>\n");
  printf("Example: ./%s \n", progname());
  printf("Options:\n");
  printf("\t[-h print this help message and exit]\n");
  printf("\t[-v print version & program information and exit]\n");
  exit(0);
}

void printSplash(){
  printf("%s ", progname());
  int l = strlen(VERSION);
  for(int i = 0; i < l - 1; i++)
    printf("%c", VERSION[i]);
  printf(", %s\n", DATE);
  printf("Written by Bernhard Haubold.\n");
  printf("Distributed under the GNU General Public License.\n");
  printf("Please send bug reports to haubold@evolbio.mpg.de\n");
  exit(0);
}
