#include "../../includes/minishell.h"

static char *extract_env_key(char *str)
{
    int start = 0;
    int len = get_env_key_length(str);
    char *key = NULL;
    char *key_eq = NULL;

    while (str[start] && str[start] != '$')
        start++;

    key = ft_substr(str, start + 1, len);
    if (!key)
        return NULL;

    key_eq = ft_strjoin(key, "=");
    free_safe((void **)&key);
    return key_eq;
}
static char *rebuild_token_string(t_token **node, char *old, char *value, int index)
{
    int new_len = ft_strlen(old) - get_env_key_length(old + index) + ft_strlen(value);
    char *new_str = malloc(new_len + 1);
    int i = 0, j = 0;

    if (!new_str)
        return NULL;

    while (old[i])
    {
        if (old[i] == '$' && i == index)
        {
            copy_var_value(new_str, value, &j);
            i += get_env_key_length(old + index) + 1;
            if (!old[i])
                break;
        }
        new_str[j++] = old[i++];
    }
    new_str[j] = '\0';

    free_safe((void **)&(*node)->token_value);
    (*node)->token_value = new_str;
    return new_str;
}

static int remove_variable_reference(t_token **node, char *str, int index)
{
    int i = 0, j = 0, len;
    char *new_str;

    len = ft_strlen(str) - get_env_key_length(str + index);
    new_str = malloc(len + 1);
    if (!new_str)
        return 1;

    while (str[i])
    {
        if (str[i] == '$' && i == index)
        {
            i += get_env_key_length(str + index) + 1;
            if (!str[i])
                break;
        }
        new_str[j++] = str[i++];
    }
    new_str[j] = '\0';

    free_safe((void **)&(*node)->token_value);
    (*node)->token_value = new_str;
    return 0;
}
static int substitute_variable_value(t_token **node, char *replacement, int pos)
{
    if (!replacement)
    {
        if (remove_variable_reference(node, (*node)->token_value, pos))
        {
            free_safe((void **)&replacement);
            return 1;
        }
    }
    else
    {
        if (!rebuild_token_string(node, (*node)->token_value, replacement, pos))
        {
            free_safe((void **)&replacement);
            return 1;
        }
    }
    free_safe((void **)&replacement);
    return 0;
}

int expand_env_variables(t_shell_data *data_shell, t_token **tokens)
{
    t_token *node = *tokens;
    int i;

    while (node)
    {
        if (node->token_type == VAR)
        {
            i = 0;
            while (node->token_value[i])
            {
                track_quote_state(&node, node->token_value[i]);
                if (node->token_value[i] == '$' &&
                    !is_special_char_after_dollar(node->token_value[i + 1]) &&
                    !is_dollar_enclosed_by_quotes(node->token_value, i) &&
                    (node->execution_status == DEFAULT || node->execution_status == DQUOTE))
                {
                    char *expanded_value = get_expanded_value(node, node->token_value + i, data_shell);

                    // 🐛 Debug print:
                    printf("Expanding: %s → %s\n", node->token_value, expanded_value ? expanded_value : "(null)");

                    substitute_variable_value(&node, expanded_value, i);
                    printf("Token after expansion: %s\n", node->token_value);
                }
                else
                    i++;
            }
        }
        node = node->next;
    }
    return 0;
}