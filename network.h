#define _CRT_SECURE_NO_WARNINGS 

void checkPing(const char* file, char* currentDns);

void modifyAdapter(char* adapter, char* currentDns);

void checkNet(const char* adapter, char* currentDns);

long extractVelocity(char* dns);