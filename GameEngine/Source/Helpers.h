#ifndef __Helpers_H__
#define __Helpers_H__

void LogError(...);

bool CheckForGLErrors(const char* file = __FILE__, int line = __LINE__);

char* LoadCompleteFile(const char* filename);
double MyGetSystemTime();

#endif //__Helpers_H__
