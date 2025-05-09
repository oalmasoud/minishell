#include "../../includes/minishell.h"

bool update_env_variable(t_shell_data *shell, char *key, char *value)
{
    int index;
    char *joined_value;
    char *full_entry;

    if (!key)
        return false;

    if (!value)
        value = "";

    index = find_environment_var_index(shell->env, key);
    joined_value = ft_strjoin("=", value);
    if (!joined_value)
        return false;

    full_entry = ft_strjoin(key, joined_value);
    free_safe_2(joined_value);
    if (!full_entry)
        return false;

    if (index != -1 && shell->env[index])
    {
        free_safe_2(shell->env[index]);
        shell->env[index] = full_entry;
    }
    else
    {
        index = var_count(shell->env);
        shell->env = resize_env_array(shell, index + 1);
        if (!shell->env)
        {
            free_safe_2(full_entry);
            return false;
        }
        shell->env[index] = full_entry;
    }

    return true;
}

static void sync_working_directories(t_shell_data *shell, char *new_pwd)
{
    char *old_pwd_env = fetch_environment_var_value(shell->env, "PWD");
    update_env_variable(shell, "OLDPWD", old_pwd_env);
    update_env_variable(shell, "PWD", new_pwd);

    if (shell->previous_directory)
    {
        free_safe_2(shell->previous_directory);
        shell->previous_directory = ft_strdup(shell->current_directory);
    }

    if (shell->current_directory)
    {
        free_safe_2(shell->current_directory);
        shell->current_directory = ft_strdup(new_pwd);
    }

    free_safe_2(new_pwd);
}

static bool report_chdir_error(const char *path)
{
    if (errno == ESTALE)
        errno = ENOENT;
    display_redirect_error("cd", path, strerror(errno), false);
    return (false);
}

static bool change_directory(t_shell_data *shell, const char *target_path)
{
    char cwd_buffer[PATH_MAX];
    char *resolved_path = NULL;
    char *temp = NULL;

    if (chdir(target_path) != 0)
        return report_chdir_error(target_path);

    resolved_path = getcwd(cwd_buffer, PATH_MAX);
    if (!resolved_path)
    {
        display_redirect_error(
            "cd: error retrieving current directory",
            "getcwd: cannot access parent directories",
            strerror(errno),
            false);

        resolved_path = ft_strjoin(shell->current_directory, "/");
        temp = resolved_path;
        resolved_path = ft_strjoin(temp, target_path);
        free_safe_2(temp);
    }
    else
    {
        resolved_path = ft_strdup(cwd_buffer);
    }

    sync_working_directories(shell, resolved_path);
    return true;
}

int handle_cd_builtin(t_shell_data *shell, char **arguments)
{
    char *target_path;

    if (!arguments || !arguments[1] ||
        ft_isspace(arguments[1][0]) ||
        arguments[1][0] == '\0' ||
        ft_strncmp(arguments[1], "--", 3) == 0)
    {
        target_path = fetch_environment_var_value(shell->env, "HOME");
        if (!target_path || *target_path == '\0' || ft_isspace(*target_path))
        {
            display_redirect_error("cd", NULL, "HOME not set", EXIT_FAILURE);
            return EXIT_FAILURE;
        }
        return (!change_directory(shell, target_path));
    }

    if (arguments[2])
    {
        display_redirect_error("cd", NULL, "too many arguments", EXIT_FAILURE);
        return EXIT_FAILURE;
    }

    if (ft_strncmp(arguments[1], "-", 2) == 0)
    {
        target_path = fetch_environment_var_value(shell->env, "OLDPWD");
        if (!target_path)
        {
            display_redirect_error("cd", NULL, "OLDPWD not set", EXIT_FAILURE);
            return EXIT_FAILURE;
        }
        return (!change_directory(shell, target_path));
    }

    return (!change_directory(shell, arguments[1]));
}
