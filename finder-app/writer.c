#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    openlog(NULL, 0, LOG_USER);


    if (argc < 3) {
        syslog(LOG_ERR, "Error: Insufficient arguments. Usage: %s <writefile> <writestr>", argv[0]);
        closelog();
        return 1;
    }

    const char *writefile = argv[1];
    const char *writestr  = argv[2];


    syslog(LOG_DEBUG, "Writing %s to %s", writestr, writefile);



    int fd = open(writefile, O_CREAT | O_WRONLY | O_TRUNC, 0664);
    if (fd == -1) {
        syslog(LOG_ERR, "Error opening file %s: %s", writefile, strerror(errno));
        closelog();
        return 1;
    }


    ssize_t bytes_written = write(fd, writestr, strlen(writestr));
    if (bytes_written == -1) {
        syslog(LOG_ERR, "Error writing to file %s: %s", writefile, strerror(errno));
        close(fd);
        closelog();
        return 1;
    }


    if (close(fd) == -1) {
        syslog(LOG_ERR, "Error closing file %s: %s", writefile, strerror(errno));
        closelog();
        return 1;
    }


    closelog();
    return 0;
}

