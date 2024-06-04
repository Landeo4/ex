#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int cd (char **argv, int i)
{
	if (i != 2)
		return (err("error: cd: bad arguments", NULL));
    else if (chdir(argv[1]) == -1)
        return (err("error: cd: cannot change directory to ", argv[1]));
	return (0);
}

int ft_strlen(char *str)
{
	int i = 0;
	while (str[i])
		i++;
	return (i);
}

int error(char *str, char *arg)
{
	write(2, str, ft_strlen(str));
	if (arg)
		write(2, arg, ft_strlen(arg));
	write(2, "\n", 2);
	return (1);
}

int exec(char **argv, int i, char **envp)
{
	int fd[2];
	int is_pipe = argv[i] && !strcmp(argv[i], "|");

	if (is_pipe && pipe(fd) == -1)
		return (error);
	int pid = fork();
	if (pid == 0)
	{
		argv = 0;
		if (is_pipe && (dup2(fd[1], 1) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
			return (error);
		execve(argv[0], argv, envp);
		return (error);
	}
	waitpid(pid, 0, 0);
	if (is_pipe && (dup2(fd[0], 0) == -1 || close(fd[0]) == -1 || close(fd[1]) == -1))
			return (error);
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	int i = 0;
	int status = 0;

	if (argc > 1)
	{
		while (argv[i])
		{
			argv += i;
			i = 0;
			while (argv[i] && (!strcmp(argv[i], "|") || !strcmp(argv[i], ";")))
				i++;
			if (!strcmp(argv[0], "cd"))
				cd(argv, i);
			else if (i)
				status = exec(argv, i, envp);
			if (argv[i])
				i++;
		}
	}
	return (status);
}
