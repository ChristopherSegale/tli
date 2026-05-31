#include <stdio.h>

void printUsage(char *programName) {
  printf("Usage instructions for %s:\n", programName);
  puts("-c compile file to byte code");
  puts("-l load file(s)");
  puts("-i launch interpreter");
  puts("-h print usage flags");
  puts("Running the program with no arguments will launch the interpreter");
}

int main(int argc, char *argv[]) {
  printUsage(argv[0]);
  return 0;
}
