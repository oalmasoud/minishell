#ifndef MINISHELL_H
#define MINISHELL_H

#include "../libft/libft.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/wait.h>
#include <limits.h>
#include <errno.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <readline/history.h>

#define PROMPT "\001\e[45m\002>>> \001\e[0m\e[33m\002 Minishell>$ \001\e[0m\002"
#define HEREDOC_NAME "/tmp/.minishell_heredoc_"

#define SUCCESS 0
#define FAILURE 1

// NEW

#define CMD_NOT_FOUND 127
#define CMD_NOT_EXECUTABLE 126

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

// NEW

extern int g_code;

typedef struct s_token
{
    char *token_value;  // Stores the actual token string after any necessary parsing or manipulation.
    char *str_original; // Holds a backup of the original token string, potentially used for restoring the original state if needed or handling errors.
    bool variable_exists;
    int token_type;
    int execution_status;
    bool join;
    struct s_token *prev;
    struct s_token *next;
} t_token;

typedef struct s_io_redirect
{
    char *input_file;
    char *output_file;
    char *heredoc_tag;
    bool heredoc_is_quoted;
    int input_fd;
    int output_fd;
    int stdin_backup;
    int stdout_backup;
} t_io_redirect;

typedef struct s_command
{
    char *command;
    char *path;
    char **args;
    bool pipe_output;
    int *pipe_fd;
    t_io_redirect *io_fds;
    struct s_command *next;
    struct s_command *prev;
} t_command;

typedef struct s_shell_data
{
    bool is_active; //  Indicates whether the shell is running in interactive mode with a user typing commands) or non-interactive mode (e.g., executing a script).
    t_token *tokens;
    char *input_line; // Stores the raw input string from the user, which will be tokenized and parsed.
    char **env;
    char *current_directory;
    char *previous_directory; // Stores the previous working directory, useful for commands like cd - which switch back to the last directory.
    t_command *cmd;
    pid_t pid;

} t_shell_data;

enum e_quoting_status
{
    DEFAULT,
    SQUOTE,
    DQUOTE
};

enum e_token_types
{
    NOT_SEPARATOR = 0,
    SPACES,
    WORD,
    VAR,
    PIPE,
    INPUT,
    TRUNC,
    HEREDOC,
    APPEND,
    END
};

bool guide_message(bool value);
bool validate_env(t_shell_data *data_shell, char **env);
bool validate_working_directories(t_shell_data *data_shell);
int find_environment_var_index(char **env, char *var);
char *fetch_environment_var_value(char **env, char *var);
void configureInteractiveSignals();
void ignoreCommandSignals();
bool process_input_command(t_shell_data *data_shell);
bool is_all_whitespace(char *input);
int tokenize_input(t_shell_data *data_shell, char *input);
int ft_isspace(int c);
int is_separator(char *str, int i);
t_token *create_new_token(char *str, char *str_backup, int type, int status);
void add_token_to_list(t_token **list, t_token *new_token);
int validateAndProcessVar(t_token **tokens);
void error_msg(const char *error, const char *context, bool newline);
int expand_env_variables(t_shell_data *data_shell, t_token **tokens);
char *get_expanded_value(t_token *token, char *str, t_shell_data *data_shell);
void free_safe(void **ptr);
bool is_valid_var_char(char c);
bool is_special_char_after_dollar(char c);
bool is_dollar_enclosed_by_quotes(const char *str, int index);
void track_quote_state(t_token **node, char c);
int get_env_key_length(char *str);
void copy_var_value(char *dst, char *src, int *j);
int process_token_quotes(t_shell_data *data_shell);
t_command *create_new_command(bool should_pipe);
void append_command_node(t_command **cmd_list, t_command *new_cmd);
void build_command_list(t_shell_data *data_shell, t_token *tokens);
void parse_command_tokens(t_command **command_list, t_token **token_stream);
void split_var_token_as_command(t_command *last_command, char *cmd_string);
int build_command_arguments(t_token **token_node, t_command *last_cmd);
int count_argument_tokens(t_token *temp);
char *merge_adjacent_vars(t_token **token_node);
int extend_echo_arguments(t_token **token_node, t_command *last_cmd);
void prune_empty_vars(t_token **tokens);
int initialize_default_args(t_token **token_node, t_command *cmd);
int count_arguments(t_token *node);
char **extend_argument_array(
    int existing_count, char **new_array, t_command *cmd, t_token **tk_node);
int append_additional_arguments(t_token **token_list, t_command *cmd);
void delete_token_node(t_token *node, void (*del)(void *));
void handle_input_redirection(t_command **cmd_list, t_token **token_stream);
void display_redirect_error(const char *filename, const char *prefix, const char *message, bool is_fatal);
t_command *get_last_command_node(t_command *command_list);
void initialize_redirect_struct(t_command *cmd_node);
bool clear_previous_file_ref(t_io_redirect *redirect, bool is_input);
void handle_output_truncation(t_command **command_list, t_token **token_stream);
void set_signals_interactive(void);
void set_signals_noninteractive(void);
void handle_heredoc_redirection(t_shell_data *data, t_command **command_list, t_token **token_stream);
void free_string_array(char **arr);
bool evaluate_heredoc_input(t_shell_data *data, char **line, t_io_redirect *redirect, bool *stop_reading);
char *resolve_variable_value(t_token *token, char *input, t_shell_data *data);
char *expand_variable_in_heredoc(char *input, char *variable_value, int dollar_index);
char *join_string_array(char **array);
void handle_output_append(t_command **command_list, t_token **token_stream);
void handle_pipe_token(t_command **command_list, t_token **token_stream);
void free_safe_2(void *ptr);

// NEW

int dispatch_execution_flow(t_shell_data *data_shell);
void clear_token_list(t_token **lst, void (*del)(void *));
void free_command_list(t_command **head, void (*del)(void *));
bool restore_std_fds(t_io_redirect *redirect);
bool apply_io_redirection(t_io_redirect *redirects);
int dispatch_builtin(t_shell_data *shell, t_command *cmd);
int handle_cd_builtin(t_shell_data *shell, char **arguments);
int var_count(char **env);
char **resize_env_array(t_shell_data *shell, int new_size);
int handle_echo_builtin(t_shell_data *shell, char **arguments);
int handle_env_builtin(t_shell_data *shell, char **arguments);
bool update_env_variable(t_shell_data *shell, char *key, char *value);
int handle_export_builtin(t_shell_data *shell, char **args);
int handle_pwd_builtin(t_shell_data *shell, char **arguments);
bool is_valid_env_var_key(char *var);
int handle_unset_builtin(t_shell_data *shell, char **arguments);
void cleanup_shell_state(t_shell_data *shell, bool full_cleanup);
void terminate_shell(t_shell_data *shell_context, int exit_code);
int handle_exit_builtin(t_shell_data *shell, char **arguments);
int launch_command_processes(t_shell_data *shell);
bool validate_redirection_fds(t_io_redirect *redirects);
bool configure_pipe_redirection(t_command *cmd_list, t_command *current);
char *resolve_command_path(t_shell_data *shell, char *command);
bool is_directory_command(char *cmd_name);
int run_local_executable(t_shell_data *shell, t_command *cmd);
int exec_child_command(t_shell_data *shell, t_command *cmd);
void close_fds(t_command *cmds, bool close_backups);
void close_unused_pipes(t_command *cmd_list, t_command *exclude);
void simple_error_message(char *error_message);
// Zaid note that I have removed int execute_exit_command(t_shell_data *data_shell, char **args);
//  NEW
#endif