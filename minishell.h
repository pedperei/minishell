#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <termios.h>
# include <curses.h>
# include <term.h>
# include <sys/wait.h>
# include <dirent.h>


# define MAX_LINE 80 // Maximum length of user input
# define MAX_ARGS MAX_LINE/2 + 1 // Maximum number of arguments for a command
# define READ_END 0 // File descriptor for read end of a pipe
# define WRITE_END 1 // File descriptor for write end of a pipe
# define MAX_HISTORY 100 // Maximum number of commands to be stored in history

typedef struct s_history
{
    char *history[MAX_HISTORY]; // Array to store history of commands
    int history_count; // Current count of commands in history
}	t_history;

#endif