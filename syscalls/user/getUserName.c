/*
 * RCS Logs:
 * $Log: getUserName.c,v $
 * Revision 1.1  2023/03/24 13:29:16  praveen
 * Initial revision
 *
 */
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h> 

int main() {
char *buf;
//buf=(char *)malloc(10*sizeof(char));
buf=getlogin();
return printf("User: %s \n",buf);
}
