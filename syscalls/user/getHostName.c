/*
 * RCS Logs:
 * $Log: getHostName.c,v $
 * Revision 1.1  2023/03/24 13:29:50  praveen
 * Initial revision
 *
 */
#include <stdio.h>
#include <unistd.h>

#define HOST_NAME_LEN   20

int main() {
   char *buildUser, hostName[HOST_NAME_LEN];
   int retVal = gethostname(hostName, HOST_NAME_LEN - 1);
   buildUser = getlogin();
   if (buildUser != NULL)
     printf("User: %s\n", buildUser);
   if (0 == retVal)
     printf("Host: %s\n", hostName);
   return retVal;
}
