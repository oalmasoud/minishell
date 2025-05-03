#include "../../includes/minishell.h"

static char *find_env_value(t_shell_data *shell, char *key)
{
    int i = 0;
    int key_len = ft_strlen(key);

    while (shell->env[i])
    {
        if (ft_strncmp(shell->env[i], key, key_len) == 0 &&
            shell->env[i][key_len] == '=') // must match key + '='
        {
            return ft_strdup(shell->env[i] + key_len + 1);
        }
        i++;
    }
    return NULL;
}

static bool env_key_exists(t_shell_data *shell, char *key)
{
    int i = 0;
    int key_len = ft_strlen(key);

    while (shell->env[i])
    {
        if (ft_strncmp(shell->env[i], key, key_len) == 0 && shell->env[i][key_len] == '=')
            return true;
        i++;
    }
    return false;
}

int get_env_key_length(char *str)
{
    int i = 0;
    int len = 0;

    while (str[i] && str[i] != '$')
        i++;
    i++;

    if (str[i] == '?' || (str[i] >= '0' && str[i] <= '9'))
        return 1;

    while (str[i] && is_valid_var_char(str[i]))
    {
        len++;
        i++;
    }
    return len;
}

static char *extract_env_key(char *str)
{
    int start = 0;
    int len = get_env_key_length(str);
    char *key;

    while (str[start] && str[start] != '$')
        start++;

    key = ft_substr(str, start + 1, len); // Just get "HOME" not "HOME="
    if (!key)
        return NULL;

    printf("🔎 Extracted env key: %s\n", key); // 👈 Add it right here

    return key;
}

char *get_expanded_value(t_token *token, char *str, t_shell_data *shell)
{
    char *key = extract_env_key(str);
    char *value = NULL;

    if (!key)
        return NULL;

    if (key[0] == '?' && key[1] == '\0')
    {
        value = ft_itoa(g_code);
        printf("Expanding special variable '$?': g_code = %d, expanded = %s\n", g_code, value);
    }
    else if (env_key_exists(shell, key))
    {
        if (token)
            token->variable_exists = true;
        value = find_env_value(shell, key);
    }

    free_safe((void **)&key);
    return value;
}
