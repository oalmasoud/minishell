#include "../../includes/minishell.h"

void free_safe(void **ptr)
{
    if (ptr && *ptr)
    {
        free(*ptr);
        *ptr = NULL;
    }
}
bool is_valid_var_char(char c)
{
    return (ft_isalnum(c) || c == '_');
}
bool is_special_char_after_dollar(char c)
{
    return (c == '$' || c == ' ' || c == '=' || c == '\0' || c == '"' || c == '\'' || c == '/' || c == '\\');
}

bool is_dollar_enclosed_by_quotes(const char *str, int index)
{
    int i = 0;
    int quote_state = DEFAULT;

    while (str[i] && i < index)
    {
        if (str[i] == '\'' && quote_state == DEFAULT)
            quote_state = SQUOTE;
        else if (str[i] == '\'' && quote_state == SQUOTE)
            quote_state = DEFAULT;
        else if (str[i] == '"' && quote_state == DEFAULT)
            quote_state = DQUOTE;
        else if (str[i] == '"' && quote_state == DQUOTE)
            quote_state = DEFAULT;
        i++;
    }
    return (quote_state == SQUOTE);
}
void track_quote_state(t_token **node, char c)
{
    if (c == '\'' && (*node)->execution_status == DEFAULT)
        (*node)->execution_status = SQUOTE;
    else if (c == '\"' && (*node)->execution_status == DEFAULT)
        (*node)->execution_status = DQUOTE;
    else if (c == '\'' && (*node)->execution_status == SQUOTE)
        (*node)->execution_status = DEFAULT;
    else if (c == '\"' && (*node)->execution_status == DQUOTE)
        (*node)->execution_status = DEFAULT;
}