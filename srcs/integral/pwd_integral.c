#include "../../includes/minishell.h"

int handle_pwd_builtin(t_shell_data *shell, char **arguments)
{
    char cwd_buffer[PATH_MAX];
    char *resolved_path;

    (void)arguments;

    if (shell->current_directory)
    {
        ft_putendl_fd(shell->current_directory, STDOUT_FILENO);
        return EXIT_SUCCESS;
    }

    resolved_path = getcwd(cwd_buffer, PATH_MAX);
    if (resolved_path)
    {
        ft_putendl_fd(resolved_path, STDOUT_FILENO);
        return EXIT_SUCCESS;
    }

    display_redirect_error("pwd", NULL, strerror(errno), false); // ✅ fixed
    return EXIT_FAILURE;
}