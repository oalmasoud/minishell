#include "../includes/minishell.h"

int g_code;

static bool validate_startup_args(t_shell_data *data_shell, int argc, char **argv)
{
    if (argc != 1 && argc != 3)
    {
        return (guide_message(false));
    }
    data_shell->is_active = (argc == 1);
    if (argc == 3)
    {
        if (ft_strncmp(argv[1], "-c", 2) != 0)
        {
            return (guide_message(false));
        }
        if (argv[2] == NULL || argv[2][0] == '\0')
        {
            return (guide_message(false));
        }
    }
    return (true);
}
static bool validate_data(t_shell_data *data_shell, char **env)
{
    if (!validate_env(data_shell, env))
    {
        simple_error_message("Failed to set up the shell environment properly");
        return (false);
    }
    if (!validate_working_directories(data_shell, env))
    {
        simple_error_message("Failed to set up the working directories");
        return (false);
    }

    // data_shell->parsed_tokens = NULL;
    // data_shell->input_line = NULL;
    // data_shell->cmd = NULL;
    // data_shell->pid = -1;
    g_code = 0;

    return (true);
}

void shell_activation(t_shell_data *data_shell)
{
    while (1)
    {
        configureInteractiveSignals();
        data_shell->input_line = readline(PROMPT);
        ignoreCommandSignals();
        if (process_input_command(data_shell) == true)
            printf("done1");
        else
            printf("done");

        break;
        //     g_code = execute(data);
        // else
        //     g_code = 1;
        // free_data(data, false);
    }
}

int main(int argc, char **argv, char **env)
{
    t_shell_data data_shell;
    ft_bzero(&data_shell, sizeof(t_shell_data));

    if (argc != 1)
    {
        ft_printf("Guid: Run './minishell' for interactive mode\n");
        exit(EXIT_FAILURE);
    }

    using_history();

    data_shell.is_active = (argc == 1);

    if (!validate_data(&data_shell, env))
    {
        ft_printf("Not valid\n");
        exit(EXIT_FAILURE);
    }
    data_shell.input_line = "echo $USER";

    if (tokenize_input(&data_shell, data_shell.input_line) == 0)
    {
        printf("Tokenization successful.\n");
    }
    else
    {
        printf("Tokenization failed.\n");
    }

    // Debug print all tokens
    t_token *current = data_shell.tokens;
    while (current != NULL)
    {
        printf("Token: %s, Type: %d\n", current->token_value, current->token_type);
        current = current->next;
    }

    // Validate and expand
    if (validateAndProcessVar(&data_shell.tokens) == FAILURE)
    {
        printf("Validation and processing of variables failed.\n");
    }
    else
    {
        printf("All tokens processed successfully.\n");
        expand_env_variables(&data_shell, &data_shell.tokens);
        process_token_quotes(&data_shell);
    }

    return 0;
}
// if (argc != 1)
// {
//     ft_printf("Guid: Run './minishell' for interactive mode\n");
//     exit(EXIT_FAILURE);
// }

// using_history();

// data_shell.is_active = (argc == 1); //! validate_startup_args(&data_shell, argc, argv) ||

// if (!validate_data(&data_shell, env))
// {
//     ft_printf("Not valid\n");
//     exit(EXIT_FAILURE);
// }
// if (data_shell.is_active)
//     shell_activation(&data_shell);

// return 0;
// }
