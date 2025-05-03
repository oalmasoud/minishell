#include "../../includes/minishell.h"

int is_separator(char current_char, char next_char)
{
    if (isspace(current_char))
        return SPACES;

    if (current_char == '<' && next_char == '<')
        return HEREDOC;
    if (current_char == '>' && next_char == '>')
        return APPEND;
    if (current_char == '<')
        return INPUT;
    if (current_char == '>')
        return TRUNC;
    if (current_char == '|')
        return PIPE;

    if (current_char == '\0')
        return END;

    return NOT_SEPARATOR;
}

t_token *create_new_token(char *content, int type, int status)
{
    t_token *new_token = malloc(sizeof(t_token));
    if (!new_token)
        return NULL;

    new_token->token_value = ft_strdup(content);
    new_token->str_original = ft_strdup(content);
    new_token->variable_exists = false;
    new_token->token_type = type;
    new_token->execution_status = status;
    new_token->join = false;
    new_token->prev = new_token->next = NULL;

    printf("Created Token: %s, Type: %d, Status: %d\n", content, type, status); // Debugging output
    return new_token;
}

void add_token_to_list(t_token **list, t_token *new_token)
{
    t_token *current = *list;
    if (current == NULL)
    {
        *list = new_token;
    }
    else
    {
        while (current->next != NULL)
            current = current->next;
        current->next = new_token;
        new_token->prev = current;
    }
    printf("Added Token to List: %s\n", new_token->token_value); // Debugging output
}