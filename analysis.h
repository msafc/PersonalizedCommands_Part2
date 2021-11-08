#define _CRT_SECURE_NO_WARNINGS 


bool findInFile(const char* path, const char* word, char* output);

long extractValue(char* line, const char* delimiter);

void showFile(const char* file);

int leecad(char* cad, int n);

char* extractString(char* line, char* delimiter, char* output);