#ifndef ERROR_H
#define ERROR_H

enum errorType { allocation, undefined };

void setError(char *errorString, char *fileName, int lineNumber);
void setErrorType(enum errorType error, char *fileName, int lineNumber);
void printError();
void memoryError();

#endif
