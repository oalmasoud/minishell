#include "../../includes/minishell.h"

static void print_echo_arguments(char **arguments, bool suppress_newline, int start_index)
{
    int i = start_index;

    while (arguments[i])
    {
        ft_putstr_fd(arguments[i], STDOUT_FILENO);

        if (arguments[i + 1])
            ft_putchar_fd(' ', STDOUT_FILENO);

        i++;
    }

    if (!suppress_newline)
        ft_putchar_fd('\n', STDOUT_FILENO);
}

static bool is_valid_n_option(const char *arg)
{
    int index = 0;

    if (arg[index] != '-')
        return false;

    index++;
    while (arg[index] == 'n')
        index++;

    return arg[index] == '\0';
}

int handle_echo_builtin(t_shell_data *shell, char **arguments)
{
    int arg_index = 1;
    bool suppress_newline = false;

    (void)shell;

    while (arguments[arg_index] && is_valid_n_option(arguments[arg_index]))
    {
        suppress_newline = true;
        arg_index++;
    }

    print_echo_arguments(arguments, suppress_newline, arg_index);
    return EXIT_SUCCESS;
}
