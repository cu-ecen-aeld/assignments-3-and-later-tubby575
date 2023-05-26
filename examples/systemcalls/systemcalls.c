#include "systemcalls.h"

#include <stdlib.h>
#include <unistd.h> 
#include <sys/wait.h>

#include <errno.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>
#include <fcntl.h>

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/

    int system_result = system(cmd);

    /** cmd was NULL and system() returned 0 indicating no shell available */
    if(cmd == NULL && system_result == 0)
    {
        return false;
    }

    /** Only system() return value that is explicitly called an error in man pages, error creating or retrieving child process */
    if(system_result == -1)
    {
        return false;
    }

    /** Child execution error check*/
    bool child_res = WIFEXITED(system_result);
    if(child_res == true)
    {
        int child_exit_code = WEXITSTATUS(system_result);
        if(child_exit_code == 127)
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    return true;
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/

bool do_exec(int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

    va_end(args);

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/

    fflush(stdout);

    int status;
    pid_t pid;
    pid = fork();

    if(pid == -1)
    {
        printf("Failed to fork\n");
        return false;
    }
    else if(pid == 0)
    {
        int execv_res = execv(command[0],command);
        if(execv_res == -1)
        {
            printf("Exec on PID = %d failed with %s\n" , getpid(), strerror(errno));
            kill(getpid(),SIGINT);
            return false;
        }
    }
    
    pid_t wait_res = waitpid(pid, &status, 0);
    if(wait_res == -1)
    {
        printf("Failed Waitpid with result -1!\n");
        return false;
    }
    else if(WIFEXITED(status) == true)
    {
        int exit_code = WEXITSTATUS(status);
        if(exit_code != 0)
        {
            printf("Exec Fail on WEXITSTATUS with exit code = %d on PID = %d\n", exit_code, getpid());
            return false;
        }
        else
        {
            return true;
        }
    }
    else
    {
        printf("Exec Fail on WIFEXITED with status = %d on PID = %d\n", status, getpid());
        return false;
    }
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;

/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/

    va_end(args);

    int status;
    pid_t kidpid;
    int fd = open(outputfile, O_WRONLY|O_TRUNC|O_CREAT, 0644);

    if(fd < 0) 
    {
        printf("Failed to open file!");
        return false;
    }

    switch (kidpid = fork()) 
    {
    case -1: 
        perror("Failed fork"); 
        return false;
    case 0:
        if (dup2(fd, 1) < 0) 
        { 
            perror("Failed on dup2");
            return false;
        }
        close(fd);
        int execv_res = execv(command[0],command);
        if(execv_res == -1)
        {
            printf("Exec on PID = %d failed with %s\n" , getpid(), strerror(errno));
            kill(getpid(),SIGINT);
            return false;
        }
    default:
        close(fd);
        pid_t wait_res = waitpid(kidpid, &status, 0);
        if(wait_res == -1)
        {
            printf("Failed Waitpid with result -1!\n");
            return false;
        }
        else if(WIFEXITED(status) == true)
        {
            int exit_code = WEXITSTATUS(status);
            if(exit_code != 0)
            {
                printf("Exec Fail on WEXITSTATUS with exit code = %d on PID = %d\n", exit_code, getpid());
                return false;
            }
            else
            {
                return true;
            }
        }
        else
        {
            printf("Exec Fail on WIFEXITED with status = %d on PID = %d\n", status, getpid());
            return false;
        }
    }
}
