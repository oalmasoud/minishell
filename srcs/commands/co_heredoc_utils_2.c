#include "../../includes/minishell.h"

char *join_string_array(char **array)
{
    char *result;
    char *temp;
    int idx;

    idx = -1;
    result = NULL;
    while (array[++idx])
    {
        temp = result;
        if (idx == 0)
            result = ft_strdup(array[0]);
        else
        {
            result = ft_strjoin(temp, array[idx]);
            free_safe_2(temp);
        }
        if (array[idx + 1])
        {
            temp = result;
            result = ft_strjoin(temp, " ");
            free_safe_2(temp);
        }
    }
    free_string_array(array);
    return (result);
}

char *create_new_token_string(char *original, char *variable_value, int total_size, int dollar_index)
{
    int i = 0;
    int j = 0;
    char *new_string;

    new_string = malloc(sizeof(char) * total_size);
    if (!new_string)
        return NULL;

    while (original[i])
    {
        if (original[i] == '$' && i == dollar_index)
        {
            copy_var_value(new_string, variable_value, &j);
            i += get_env_key_length(original + dollar_index) + 1;
            if (original[i] == '\0')
                break;
        }
        new_string[j++] = original[i++];
    }
    new_string[j] = '\0';
    return new_string;
}

static char *replace_variable_in_string(t_token **token_node, char *input, char *variable_value, int dollar_index)
{
    char *new_string;
    int calculated_size;

    calculated_size = ft_strlen(input) - get_env_key_length(input + dollar_index) + ft_strlen(variable_value);
    new_string = create_new_token_string(input, variable_value, calculated_size, dollar_index);

    if (token_node && *token_node)
    {
        free_safe_2((*token_node)->token_value);
        (*token_node)->token_value = new_string;
    }
    return new_string;
}

char *expand_variable_in_heredoc(char *input, char *variable_value, int dollar_index)
{
    char *temp;

    temp = NULL;
    if (variable_value == NULL)
        *input = '\0';
    else
    {
        temp = input;
        input = replace_variable_in_string(NULL, input, variable_value, dollar_index);
        free_safe_2(temp);
    }
    free_safe_2(variable_value);
    return input;
}