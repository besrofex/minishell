
#ifndef MINI_H
# define MINI_H

# include <stdlib.h>
# include <stdio.h>
# include "./libft42/libft.h"


typedef enum e_token_type {
    TOKEN_WORD,         // mots, arguments, noms de fichiers
    TOKEN_PIPE,         // |
    TOKEN_REDIR_IN,     // <
    TOKEN_REDIR_OUT,    // >
    TOKEN_REDIR_APPEND, // >>
    TOKEN_HEREDOC,      // <<
    TOKEN_EOF           // fin de ligne
} t_token_type;


typedef struct s_token {
    t_token_type    type;
    char            *value;     // contenu du token
    int             quoted;     // type de quote (0, ', ")
    struct s_token  *next;
} t_token;


typedef struct s_redirection {
    t_token_type type;
    char *file;
    struct s_redirection *next;
} t_redirection;

typedef struct s_cmd {
    char **args;
    t_redirection *redirections;
    struct s_cmd *next;
} t_cmd;

// utils :
int is_whitespace(char c);
int is_delimiter(char c);
t_token *create_token(t_token_type type, char *value);
void add_token_to_list(t_token **head, t_token *new_token);
void free_tokens(t_token *tokens);
int check_syntax(t_token *tokens);

// main 
t_token *tokenize_word(char *input, int *i);
t_token *tokenize(char *input);

// utils_token_2
void	expand_tokens(t_token *tokens, char **envp, int exit_status);
char	*expand_str(char *str, char **envp, int exit_status);
char	*expand_var(char *str, int *i, char **envp, int exit_status);
char	*handle_quotes_expansion(char *str, int *i, char **envp, int exit_status);
char	*process_dollar_in_dquote(char *str, int *i, char **envp, int exit_status);


t_cmd   *parse_tokens(t_token *tokens);
char	*ft_strjoin_free(char *s1, char *s2, int to_free);
char	*get_env_value(char *var_name, char **envp);
char	*handle_globbing(char *pattern);


void add_argument(t_cmd *cmd, t_token *token);
void add_redirection(t_cmd *cmd, t_token *token);
t_cmd *init_cmd(void);

#endif