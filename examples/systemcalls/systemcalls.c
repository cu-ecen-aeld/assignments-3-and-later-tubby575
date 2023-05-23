#include "systemcalls.h"

#include <stdlib.h>
#include <unistd.h> 
#include <sys/wait.h>

#include <errno.h>
#include <string.h>
#include <syslog.h>
#include <signal.h>

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
    printf("\nStarting do_exec() on pid = %d\n", getpid());

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

    printf("\nForking on PID = %d\n", getpid());
    fflush(stdout);

    int execv_res = 0;
    int status;
    pid_t pid;
    pid = fork();

    if(pid == -1)
    {
        printf("\nFailed fork\n");
        return false;
    }
    else if(pid == 0)
    {
        char** rem_cmd = (char**)&command[1];
        printf("\nRunning Exec in child with PID = %d and command = %s %s %s\n", getpid(), command[0], rem_cmd[0], rem_cmd[1]);
        for(int i = 0; i < count;i++)
        {
            printf("\nrem_cmd[%d] = %s", i , rem_cmd[i]);
        }
        execv_res = execv(command[0],command);
        if(execv_res == -1)
        {
            printf("\nExec on PID = %d fail with %s\n" , getpid(), strerror(errno));
            kill(getpid(),SIGINT);
            return false;
        }
    }
    
    pid_t wait_res = waitpid(pid, &status, 0);
    if(wait_res == -1)
    {
        printf("\nWaitpid was -1!\n");
        return false;
    }
    else if(WIFEXITED(status) == true)
    {
        printf("\nStatus code on PID = %d is %d\n", getpid(), status);
        int exit_code = WEXITSTATUS(status);
        if(exit_code != 0)
        {
            printf("\nExec Fail on WEXITSTATUS with exit code = %d on PID = %d\n", exit_code, getpid());
            return false;
        }
        else
        {
            printf("\nExec Success on WEXITSTATUS with exit code = %d on PID = %d\n", exit_code, getpid());
            return true;
        }
    }
    else
    {
        printf("\nExec Fail on WIFEXITED with status = %d on PID = %d\n", status, getpid());
        return false;
    }

    printf("\nGot to end on PID = %d ...\n", getpid());
    return false;
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
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/

    va_end(args);

    return true;
}
