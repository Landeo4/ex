#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

int ft_strlen(char *str)
{
    int i = 0;
    while (str[i])
      i++; 
    return (i);
}

int err(char *str, char *arg) 
{
    write(2, str, ft_strlen(str));
    if (arg)
      write(2, arg, ft_strlen(str));
    write(2, "\n", 1);
    return (1);
}

int cd(char **argv, int i) 
{
    if (i != 2)
        return (err("error: cd: bad arguments", NULL));
    else if (chdir(argv[1]) == -1)
        return (err("error: cd: cannot change directory to ", argv[1]));
    return 0;
}
#include "stdio.h"

int exec(char **argv, char **envp, int i) 
{
    int fd[2];
    int has_pipe = argv[i] && !strcmp(argv[i], "|");
    if (has_pipe && pipe(fd) == -1)
        return err("error: fatal", NULL);
    int pid = fork();
    if (pid == 0)
    {
        argv[i] = 0;
        if (has_pipe && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
            return err("error: fatal", NULL);
        execve(argv[0], argv, envp);
        return (err("error: cannot execute ", argv[0]));
    }
    waitpid(pid, 0, 0);
    if (has_pipe && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
        return (err("error: fatal", NULL));
    return (0);
}

int main(int argc, char **argv, char **envp) 
{
    int    i = 1;
    int    status = 0;

    if (argc > 1) 
    {
        while (argv && argv[i] && argv[i + 1]) 
        {
            argv += i;
            i = 0;
            while (argv[i] && strcmp(argv[i], "|") && strcmp(argv[i], ";"))
                i++;
            if (!strcmp(argv[0], "cd"))
                cd(argv, i);
            else if (i)
                status = exec(argv, envp, i);
			if (argv[i])
 	           i++;
        }
    }
    return (status);
}

//./microshell /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell