#include "../../includes/minishell.h"

char **extend_argument_array(
    int existing_count, char **new_array, t_command *cmd, t_token **tk_node)
{
    int i = 0;
    t_token *current = *tk_node;

    while (i < existing_count)
    {
        new_array[i] = cmd->args[i];
        i++;
    }

    while (current && (current->token_type == WORD || current->token_type == VAR))
    {
        new_array[i++] = ft_strdup(current->token_value);
        current = current->next;
    }

    new_array[i] = NULL;
    return new_array;
}

void delete_token_node(t_token *node, void (*del)(void *))
{
    if (del && node && node->token_value)
    {
        del(node->token_value);
        node->token_value = NULL;
    }
    if (del && node && node->str_original)
    {
        del(node->str_original);
        node->str_original = NULL;
    }
    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;

    free_safe((void **)&node);
}

void prune_empty_vars(t_token **tokens)
{
    t_token *temp;

    temp = *tokens;
    while (temp->token_type == WORD || temp->token_type == VAR)
    {
        if (temp->token_type == VAR && temp->token_value[0] == '\0' && temp->variable_exists == false)
        {
            temp = temp->next;
            if (temp == (*tokens)->next)
                (*tokens) = (*tokens)->next;
            delete_token_node(temp->prev, free_safe_2);
        }
        else
            temp = temp->next;
    }
}
static int initialize_echo_args(t_token **token_node, t_command *last_cmd)
{
    int nb_args;
    t_token *temp;
    int i;

    prune_empty_vars(token_node);
    temp = *token_node;
    nb_args = count_argument_tokens(temp);
    last_cmd->args = malloc(sizeof(char *) * (nb_args + 2));
    if (!last_cmd->args)
        return (FAILURE);
    i = 0;
    last_cmd->args[i] = ft_strdup(last_cmd->command);
    i++;
    while (temp->token_type == WORD || temp->token_type == VAR)
    {
        if (temp->join == true)
            last_cmd->args[i] = merge_adjacent_vars(&temp);
        else
            last_cmd->args[i] = ft_strdup(temp->token_value);
        i++;
        temp = temp->next;
    }
    last_cmd->args[i] = NULL;
    *token_node = temp;
    return (SUCCESS);
}

int build_command_arguments(t_token **token_node, t_command *last_cmd)
{
    if (ft_strncmp(last_cmd->command, "echo", 5) == 0)
    {
        if (!(last_cmd->args))
            return (initialize_echo_args(token_node, last_cmd));
        else
            return (extend_echo_arguments(token_node, last_cmd));
    }
    else
    {
        if (last_cmd && !(last_cmd->args))
            return (initialize_default_args(token_node, last_cmd));
        else
            return (append_additional_arguments(token_node, last_cmd));
    }
    return (SUCCESS);
}