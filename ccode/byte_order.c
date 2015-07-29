#include <stdio.h>
#include <arpa/inet.h>

int main(){

	unsigned char a[4];

	a[0] = 0x11;
	a[0] = 0x22;
	a[0] = 0x33;
	a[0] = 0x44;

	unsigned char* pc = (unsigned char*)a;
	printf("%x\n", *pc);

	unsigned short* ps = (unsigned short*)a;
	printf("%x\n", *ps);

	unsigned int* pi =(unsigned int*)a;
	printf("%x\n", ntohl(*pi)); //network 바이트오더를 호스트파일 바이오더로 변경?
}
