/* 
 * Praveen Prasad (dt07Aug2023At23:42hrs(approx),Mydesk,home,bangalore)
 * About syscall getpwnam() and getpwnam_r() and usage in code.
 * RCS Logs:
 * $Log: sc_getpwnam_r.c,v $
 * Revision 1.2  2023/08/07 18:59:30  praveen
 * This version v1.2 contains changes for syscall getpwnam() and shows the usage
 *
 * Revision 1.1  2023/08/07 18:27:43  praveen
 * Initial revision
 *
 */
/*
 * Infos:
 *        The passwd structure is defined in <pwd.h> as follows:

           struct passwd {
               char   *pw_name;       // username 
               char   *pw_passwd;     // user password
               uid_t   pw_uid;        // user ID 
               gid_t   pw_gid;        // group ID 
               char   *pw_gecos;      // user information 
               char   *pw_dir;        // home directory 
               char   *pw_shell;      // shell program 
           };
*/

#include <pwd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

       int
       main(int argc, char *argv[])
       {
           struct passwd *pwd;
           struct passwd *result;
           char *buf;
           size_t bufsize;
           int s;

           if (argc != 2) {
               fprintf(stderr, "Usage: %s username\n", argv[0]);
               exit(EXIT_FAILURE);
           }

           bufsize = sysconf(_SC_GETPW_R_SIZE_MAX);
           if (bufsize == -1)          /* Value was indeterminate */
               bufsize = 16384;        /* Should be more than enough */

           buf = malloc(bufsize);
           if (buf == NULL) {
               perror("malloc");
               exit(EXIT_FAILURE);
           }

           result = getpwnam(argv[1]);
           if (result == NULL) {
               fprintf(stderr, "getpwnam: Error\n");
               exit(EXIT_FAILURE);
           }
           pwd = result;
           printf("Name: %s; UID: %jd\n", pwd->pw_gecos,
                   (intmax_t) pwd->pw_uid);
           exit(EXIT_SUCCESS);
       }
