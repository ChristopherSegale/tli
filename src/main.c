#include <stdio.h>
#include <stdlib.h>
#include "argParse.h"
#include "error.h"

void printUsage(char *programName) {
  printf("Usage instructions for %s:\n", programName);
  puts("-c compile file to byte code");
  puts("-l load file(s)");
  puts("-i launch interpreter");
  puts("-h print usage flags");
  puts("Running the program with no arguments will launch the interpreter");
}

void printWIP(char *fn) {
  printf("Feature %s is still being worked on.\n", fn);
}

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printWIP("interpreter");
    return 0;
  }
  enum failure f = noError;
  struct argParse *flags = parseArgs(&f, argc, argv);
  if(f == noDash) {
    setError("First argument should start with a '-' character.", NULL, 0);
    printError();
    return 0;
  }
  if(flags->compFlag) {
    printWIP("compile");
  }
  if(flags->loadFlag) {
    printWIP("load");
  }
  if(flags->interFlag) {
    printWIP("interpreter");
  }
  if(flags->helpFlag) {
    printUsage(argv[0]);
  }
  return 0;
}
