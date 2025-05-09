#include "../../includes/minishell.h"

static bool out_range(int neg, unsigned long long num, bool *error)
{
    if ((neg == 1 && num > LONG_MAX) || (neg == -1 && num > (unsigned long)LONG_MAX + 1))
        *error = true;
    return *error;
}

static int atoi_long(const char *str, bool *error)
{
    unsigned long long num = 0;
    int neg = 1;
    int i = 0;

    while (ft_isspace(str[i]))
        i++;
    if (str[i] == '+')
        i++;
    else if (str[i] == '-')
    {
        neg = -1;
        i++;
    }
    while (ft_isdigit(str[i]))
    {
        num = num * 10 + (str[i] - '0');
        if (out_range(neg, num, error))
            break;
        i++;
    }
    return (int)(num * neg);
}

int resolve_exit_code(char *arg, bool *error)
{
    int i = 0;

    *error = false;
    if (!arg)
        return g_code;

    while (ft_isspace(arg[i]))
        i++;

    if (arg[i] == '\0')
        return (*error = true, 2);

    if (arg[i] == '+' || arg[i] == '-')
        i++;

    if (!ft_isdigit(arg[i]))
        return (*error = true, 2);

    while (ft_isdigit(arg[i]))
        i++;

    while (ft_isspace(arg[i]))
        i++;

    if (arg[i] != '\0')
        return (*error = true, 2);

    int result = atoi_long(arg, error);
    return (unsigned char)result;
}

static bool is_part_of_pipeline(t_shell_data *shell)
{
    t_command *current_cmd = shell->cmd;

    if (!current_cmd)
        return false;

    return (current_cmd->next || current_cmd->prev);
}

int handle_exit_builtin(t_shell_data *shell, char **arguments)
{
    int code;
    bool has_error;
    bool is_quiet;

    is_quiet = is_part_of_pipeline(shell);
    has_error = false;

    if (!is_quiet && shell->is_active)
        ft_putendl_fd("exit", STDERR_FILENO);

    if (!arguments || !arguments[1])
    {
        code = g_code;
    }
    else
    {
        code = resolve_exit_code(arguments[1], &has_error);
        if (has_error)
        {
            display_redirect_error("exit", arguments[1], "numeric argument required", false);
            code = 2;
        }
        else if (arguments[2])
        {
            display_redirect_error("exit", NULL, "too many arguments", false);
            return 1;
        }
    }

    terminate_shell(shell, code);
    return 2;
}
