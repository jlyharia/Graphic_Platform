// textfile.h: interface for reading and writing text files
// www.lighthouse3d.com
//
// You may use these functions freely.
// they are provided as is, and no warranties, either implicit,
// or explicit are given
//////////////////////////////////////////////////////////////////////
#include <string>
char *textFileRead(char *);
int textFileWrite(char *, char *);
int printOglError(char *, int );
void printShaderInfoLog(GLuint );
void printProgramInfoLog(GLuint );
GLuint CreateShaders(std::string );
