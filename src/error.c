#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

#define BUFFERSIZE 101

char *allocationError = "Failed to allocate memory.";
char *undefinedError = "Undefined error. ERROR function requires a string as an argument.";

char errorMessage[BUFFERSIZE];

void setError(char *errorString, int lineNumber) {
  if(lineNumber > 0)
    snprintf(errorMessage, sizeof(errorMessage), "%d: %s", lineNumber, errorString);
  else {
    if(strlen(errorString) < BUFFERSIZE)
      strcpy(errorMessage, errorString);
    else
      strcpy(errorMessage, "INTERNAL ERROR: Error message exceeded error buffer");
  }
}

void setErrorType(enum errorType error, int lineNumber) {
  switch (error) {
  case allocation:
    if(lineNumber > 0)
      snprintf(errorMessage, sizeof(errorMessage), "%d: %s", lineNumber, allocationError);
    else
      strcpy(errorMessage, allocationError);
    break;
  case undefined:
    if(lineNumber > 0)
      snprintf(errorMessage, sizeof(errorMessage), "%d: %s", lineNumber, undefinedError);
    else
      strcpy(errorMessage, undefinedError);
    break;
  }
}

void printError() {
  printf("Error: %s\n", errorMessage);
}
