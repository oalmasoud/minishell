#include "../../includes/minishell.h"

static bool checkForOperatorErrors(t_token *node)
{
    t_token *prev = node->prev;

    // Skip spaces when looking backwards
    while (prev && prev->token_type == SPACES)
        prev = prev->prev;

    if (!prev)
        return false;

    printf("Checking consecutive operator errors between tokens '%s' and '%s'\n",
           prev->token_value, node->token_value);

    // Handle invalid consecutive operators (PIPE/TRUNC/APPEND/etc.)
    if ((node->token_type >= PIPE && node->token_type <= APPEND) &&
        (prev->token_type >= PIPE && prev->token_type <= APPEND))
        return true;

    if (node->token_type == END && prev->token_type >= PIPE)
        return true;

    return false;
}

static int processTokenList(t_token **list)
{
    t_token *current = *list;
    int i;
    char *errorContext;

    while (current != NULL)
    {
        printf("Processing token: %s\n", current->token_value);
        i = 0;
        while (current->token_value[i] != '\0')
        {
            if (current->token_value[i] == '$' && (!current->prev || current->prev->token_type != HEREDOC))
            {
                printf("Variable detected in token: %s\n", current->token_value);
                current->token_type = VAR;
                break;
            }
            i++;
        }

        if (checkForOperatorErrors(current))
        {

            errorContext = (current->token_type == END) ? "newline" : current->token_value;
            error_msg("Unexpected token", errorContext, true);
            return FAILURE;
        }
        current = current->next;
    }
    return SUCCESS;
}

int validateAndProcessVar(t_token **tokens)
{
    char *errorMsg;
    if (*tokens == NULL || (*tokens)->token_type == PIPE)
    {
        errorMsg = (*tokens && (*tokens)->token_value) ? (*tokens)->token_value : "UNKNOWN";
        error_msg("Unexpected token", errorMsg, true);
        return FAILURE;
    }

    return processTokenList(tokens);
}
