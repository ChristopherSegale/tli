#ifndef ERROR_H
#define ERROR_H

enum errorType { allocation, undefined };

void setError(char *errorString, int lineNumber);
void setErrorType(enum errorType error, int lineNumber);
void printError();

#endif
