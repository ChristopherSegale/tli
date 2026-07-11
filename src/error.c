#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

#define BUFFERSIZE 301

char *allocationError = "Failed to allocate memory.";
char *undefinedError = "INTERNAL ERROR: Undefined error.";

char errorMessage[BUFFERSIZE];

void setError(char *errorString, char *fileName, int lineNumber) {
  if(lineNumber > 0) {
    if(fileName)
      snprintf(errorMessage, sizeof(errorMessage), "%s: %d: %s", fileName, lineNumber, errorString);
    else
      snprintf(errorMessage, sizeof(errorMessage), "%d: %s", lineNumber, errorString);
  }
  else {
    if(strlen(errorString) < BUFFERSIZE)
      strcpy(errorMessage, errorString);
    else
      strcpy(errorMessage, "INTERNAL ERROR: Error message exceeded error buffer");
  }
}

void setErrorType(enum errorType error, char *fileName, int lineNumber) {
  switch (error) {
  case allocation:
    if(lineNumber > 0) {
      if(fileName)
        setError(allocationError, fileName, lineNumber);
      else
	setError(allocationError, NULL, lineNumber);
    }
    else
      strcpy(errorMessage, allocationError);
    break;
  case undefined:
    if(lineNumber > 0) {
      if(fileName)
	setError(undefinedError, fileName, lineNumber);
      else
	setError(undefinedError, NULL, lineNumber);
    }
    else
      strcpy(errorMessage, undefinedError);
    break;
  }
}

void printError() {
  printf("Error: %s\n", errorMessage);
}
