#include "../../includes/minishell.h"

static int env_counter(char **env)
{
    int i;

    i = 0;
    while (env && env[i])
        i++;

    return (i);
}
void close_unused_pipes(t_command *cmd_list, t_command *exclude)
{
    while (cmd_list)
    {
        if (cmd_list != exclude && cmd_list->pipe_fd)
        {
            close(cmd_list->pipe_fd[0]);
            close(cmd_list->pipe_fd[1]);
        }
        cmd_list = cmd_list->next;
    }
}
bool configure_pipe_redirection(t_command *cmd_list, t_command *current)
{
    if (!current)
        return false;

    if (current->prev && current->prev->pipe_output)
        dup2(current->prev->pipe_fd[0], STDIN_FILENO);

    if (current->pipe_output)
        dup2(current->pipe_fd[1], STDOUT_FILENO);

    close_unused_pipes(cmd_list, current);
    return true;
}

bool validate_env(t_shell_data *data_shell, char **env)
{
    int i;

    data_shell->env = ft_calloc(env_counter(env) + 1, sizeof *data_shell->env);
    if (!data_shell->env)
        return (false);
    i = 0;
    while (env[i])
    {
        data_shell->env[i] = ft_strdup(env[i]);
        if (!data_shell->env[i])
            return (false);
        i++;
    }
    return (true);
}

bool validate_working_directories(t_shell_data *data_shell)
{
    char buff[PATH_MAX];
    char *wd;

    wd = getcwd(buff, PATH_MAX);
    data_shell->current_directory = ft_strdup(wd);
    if (!data_shell->current_directory)
        return (false);
    if (find_environment_var_index(data_shell->env, "OLDPWD") != -1)
    {
        data_shell->previous_directory = ft_strdup(fetch_environment_var_value(data_shell->env,
                                                                               "OLDPWD"));
        if (!data_shell->previous_directory)
            return (false);
    }
    else
    {
        data_shell->previous_directory = ft_strdup(wd);
        if (!data_shell->previous_directory)
            return (false);
    }
    return (true);
}