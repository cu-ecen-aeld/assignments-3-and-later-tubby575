#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int main(int argc, char* argv[])
{

    openlog("Writer", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_USER);
    
    if(argc < 3)
    {
        syslog(LOG_ERR, "Invalid number of parameters given! Exiting program...");
        exit(1);
    }

    char* full_file_path = argv[1];
    char* write_str = argv[2];

    FILE* new_file = fopen(full_file_path,"w");
    if(new_file == NULL)
    {
        syslog(LOG_ERR , "Failed to Create file! Exiting program...");
        exit(1);
    }

    syslog(LOG_DEBUG, "Writing %s to %s", write_str, full_file_path);
    int res = fputs(write_str,new_file);
    if(res == EOF)
    {
        syslog(LOG_ERR, "Failed to write to file!");
    }

    fclose(new_file);
    closelog();
    return 0;
}