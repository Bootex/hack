#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
{
  char buf[16];
  if( argc != 2)
  	return -1;
  strncpy(buf, argv[1], strlen(argv[1]));//	← 버퍼 오버플로우
  return 0;
}
