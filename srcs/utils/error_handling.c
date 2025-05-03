#include "../../includes/minishell.h"

bool guide_message(bool value)
{
    ft_putendl_fd("Guid: Run './minishell' for interactive mode", 2);
    ft_putendl_fd("Guid: Run './minishell -c \"command\"' to execute a specific command", 2);
    return (value);
}
void simple_error_message(char *error_message)
{
    ft_putendl_fd(error_message, STDERR_FILENO); // Using ft_putendl_fd to print to the standard error file descriptor
}
void error_msg(const char *error, const char *context, bool newline)
{
    if (newline)
    {
        ft_printf("%s '%s'\n", error, context);
    }
    else
    {
        ft_printf("%s '%s'", error, context);
    }
}

void display_redirect_error(const char *filename, const char *prefix, const char *message, bool is_fatal)
{
    char *full_msg;
    char *temp;

    if (prefix)
    {
        temp = ft_strjoin(prefix, ": ");
        full_msg = ft_strjoin(temp, filename);
        free_safe_2(temp);
    }
    else
        full_msg = ft_strdup(filename);

    temp = ft_strjoin(full_msg, ": ");
    free_safe_2(full_msg);
    full_msg = ft_strjoin(temp, message);
    free_safe_2(temp);

    ft_putendl_fd(full_msg, STDERR_FILENO);
    free_safe_2(full_msg);

    if (is_fatal)
        exit(EXIT_FAILURE);
}
void free_safe_2(void *ptr)
{
    if (ptr)
    {
        free(ptr);
        ptr = NULL;
    }
}