#ifndef ARGPARSE_H
#define ARGPARSE_H

struct argParse {
  int compFlag;
  int loadFlag;
  int interFlag;
  int helpFlag;
};

enum failure { noError, noDash, error };

struct argParse* initArgs();

struct argParse* parseArgs(enum failure *f, int argc, char **argv);

#endif
