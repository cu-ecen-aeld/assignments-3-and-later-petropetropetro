#include "systemcalls.h"

/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

    if (cmd == NULL)
        return false;
    
    int result = system(cmd);
    
    if (result != 0)
        return false;

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
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    command[count] = command[count];

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/
    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return false;
    }
    else if(pid == 0)
    {
        if(execv(command[0], command) == -1)
        {
            perror("execv");
            exit(EXIT_FAILURE);
        }
    }

    int child_p_status = -1;

    if (waitpid(pid, &child_p_status, 0) == -1)
    {
        perror("waitpid");
        return false;
    }

    if(WIFEXITED(child_p_status) && WEXITSTATUS(child_p_status) != 0)
        return false;

    va_end(args);

    return true;
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


    pid_t pid = fork();

    if (pid == -1)
    {
        perror("fork");
        return false;
    }

    else if(pid == 0)
    {
        int fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE); // Exit child process on error
        }

        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("dup2");
            close(fd);
            exit(EXIT_FAILURE); // Exit child process on error
        }

        close(fd);

        if(execv(command[0], command) == -1)
        {
            perror("execv");
            return false;
        }
    }

    int child_p_status = -1;

    if (waitpid(pid, &child_p_status, 0) == -1)
    {
        perror("waitpid");
        return false;
    }

    if(WIFEXITED(child_p_status) && WEXITSTATUS(child_p_status) != 0)
        return false;

    va_end(args);

    return true;
}
