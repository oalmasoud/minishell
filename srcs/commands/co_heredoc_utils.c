#include "../../includes/minishell.h"

static char *find_env_variable(t_shell_data *data, char *variable)
{
    char *value;
    int idx;
    int length;

    idx = 0;
    length = ft_strlen(variable);
    while (data->env[idx])
    {
        if (ft_strncmp(data->env[idx], variable, length) == 0)
            break;
        idx++;
    }
    value = ft_strdup(data->env[idx] + length);
    return value;
}

static int check_var_presence(t_shell_data *data, char *variable)
{
    int idx;
    int length;

    idx = 0;
    length = ft_strlen(variable);
    while (data->env[idx])
    {
        if (ft_strncmp(data->env[idx], variable, length) == 0)
            return 0;
        idx++;
    }
    return 1;
}

static char *extract_var_name(char *input)
{
    char *variable;
    char *joined;
    int start_idx;
    int length;
    int i;

    i = 0;
    start_idx = 0;
    while (input[i])
    {
        if (input[i] == '$')
        {
            start_idx = i + 1;
            break;
        }
        i++;
    }
    length = get_env_key_length(input);
    variable = ft_substr(input, start_idx, length);
    if (!variable)
        return NULL;
    joined = ft_strjoin(variable, "=");
    free_safe_2(variable);
    variable = joined;
    return variable;
}

char *resolve_variable_value(t_token *token, char *input, t_shell_data *data)
{
    char *resolved_value;
    char *variable;

    variable = extract_var_name(input);
    if (variable && check_var_presence(data, variable) == 0)
    {
        if (token != NULL)
            token->variable_exists = true;
        resolved_value = find_env_variable(data, variable);
    }
    else if (variable && variable[0] == '?' && variable[1] == '=')
        resolved_value = ft_itoa(g_code);
    else
        resolved_value = NULL;
    free_safe_2(variable);
    return resolved_value;
}