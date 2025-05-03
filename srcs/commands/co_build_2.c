#include "../../includes/minishell.h"

int initialize_default_args(t_token **token_node, t_command *cmd)
{
    int i = 0;
    int num_args = count_arguments(*token_node);
    t_token *current = *token_node;

    cmd->args = malloc(sizeof(char *) * (num_args + 2));
    if (!cmd->args)
        return FAILURE;

    cmd->args[i++] = ft_strdup(cmd->command);
    while (current && (current->token_type == WORD || current->token_type == VAR))
    {
        cmd->args[i++] = ft_strdup(current->token_value);
        current = current->next;
    }
    cmd->args[i] = NULL;

    *token_node = current;
    return SUCCESS;
}
char *merge_adjacent_vars(t_token **token_node)
{
    t_token *temp;
    char *str;

    temp = *token_node;
    str = ft_strdup(temp->token_value);
    while (temp->token_type == VAR && temp->next->token_type == VAR && temp->next->join == true)
    {
        str = ft_strjoin(str, temp->next->token_value);
        temp = temp->next;
    }
    *token_node = temp;
    return (str);
}

static char **copy_args_into_array(int start_index, char **arg_array,
                                   t_command *last_cmd, t_token *tokens)
{
    int i = start_index;

    for (int j = 0; j < start_index; j++)
        arg_array[j] = last_cmd->args[j];

    while (tokens && (tokens->token_type == WORD || tokens->token_type == VAR))
    {
        if (tokens->join == true)
            arg_array[i++] = merge_adjacent_vars(&tokens);
        else
            arg_array[i++] = ft_strdup(tokens->token_value);
        tokens = tokens->next;
    }

    arg_array[i] = NULL;
    return arg_array;
}

int extend_echo_arguments(t_token **token_node, t_command *last_cmd)
{
    int existing_len = 0;
    int new_arg_count;
    char **new_args;
    t_token *temp;

    prune_empty_vars(token_node);
    temp = *token_node;
    new_arg_count = count_argument_tokens(temp);

    while (last_cmd->args && last_cmd->args[existing_len])
        existing_len++;

    new_args = malloc(sizeof(char *) * (existing_len + new_arg_count + 1));
    if (!new_args)
        return FAILURE;

    new_args = copy_args_into_array(existing_len, new_args, last_cmd, temp);

    free(last_cmd->args);
    last_cmd->args = new_args;

    while (temp && (temp->token_type == WORD || temp->token_type == VAR))
        temp = temp->next;

    *token_node = temp;
    return SUCCESS;
}

int count_argument_tokens(t_token *temp)
{
    int i;

    i = 0;
    while (temp && (temp->token_type == WORD || temp->token_type == VAR))
    {
        if (temp->token_type == VAR && temp->join == true)
        {
            while (temp->token_type == VAR && temp->join == true)
                temp = temp->next;
            i++;
        }
        else
        {
            i++;
            temp = temp->next;
        }
    }
    return (i);
}