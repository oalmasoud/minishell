#include "../../includes/minishell.h"

char *ft_strjoin_with_slash(char *prefix, char *suffix)
{
    char *joined;
    size_t len = ft_strlen(prefix);

    if (prefix[len - 1] == '/')
        return ft_strjoin(prefix, suffix);
    else
    {
        char *temp = ft_strjoin(prefix, "/");
        joined = ft_strjoin(temp, suffix);
        free_safe_2(temp);
        return joined;
    }
}

static char *resolve_valid_command_path(char *cmd_name, char **directories)
{
    int i = 0;
    char *combined_path = NULL;

    while (directories[i])
    {
        combined_path = ft_strjoin_with_slash(directories[i], cmd_name);
        if (!combined_path)
        {
            display_redirect_error("malloc", NULL, "unexpected allocation failure", false);
            return NULL;
        }
        if (access(combined_path, F_OK | X_OK) == 0)
            return combined_path;
        free_safe_2(combined_path);
        i++;
    }
    return NULL;
}

static char **extract_env_path_list(t_shell_data *shell)
{
    if (find_environment_var_index(shell->env, "PATH") == -1)
        return NULL;

    return ft_split(fetch_environment_var_value(shell->env, "PATH"), ':');
}

char *resolve_command_path(t_shell_data *shell, char *command)
{
    char **path_dirs;
    char *resolved_path;

    if (!command)
        return NULL;

    path_dirs = extract_env_path_list(shell);
    if (!path_dirs)
        return NULL;

    resolved_path = resolve_valid_command_path(command, path_dirs);
    free_string_array(path_dirs);
    return resolved_path;
}
