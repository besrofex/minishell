#include "mini.h"

int is_whitespace(char c)
{
   return (c == ' ' || c == '\t' || c == '\n');
}

int is_delimiter(char c)
{
   return (c == ' ' || c == '\t' || c == '\n' || 
            c == '|' || c == '<' || c == '>' || c == '\0');
}

t_token *create_token(t_token_type type, char *value)
{
   t_token *token = malloc(sizeof(t_token));
   
   if (!token)
      return (NULL);
   token->type = type;
   token->value = ft_strdup(value);
   if (!token->value)
   {
      free(token);
      return (NULL);
   }
   token->quoted = 0;
   token->next = NULL;
   
   return (token);
}

void add_token_to_list(t_token **head, t_token *new_token)
{
    t_token *current;

   if (!new_token)
      return;
   
   if (!*head)
   {
      *head = new_token;
      return;
   }
   
   current = *head;
   while (current->next)
      current = current->next;
   
   current->next = new_token;
}

void free_tokens(t_token *tokens)
{
    t_token *tmp;

   while (tokens)
   {
      tmp = tokens;
      tokens = tokens->next;
      free(tmp->value);
      free(tmp);
   }
}

int check_syntax(t_token *tokens)
{
    t_token *current = tokens;
    t_token *prev = NULL;

   while (current)
   {
      // Vérification des pipes au début ou à la fin
      if (current->type == TOKEN_PIPE && (!prev || !current->next))
      {
         fprintf(stderr, "Syntax error near unexpected token `|'\n");
         return (0);
      }
      
      // Vérification des redirections sans fichier
      if ((current->type == TOKEN_REDIR_IN || 
         current->type == TOKEN_REDIR_OUT ||
         current->type == TOKEN_REDIR_APPEND ||
         current->type == TOKEN_HEREDOC) && 
         (!current->next || current->next->type != TOKEN_WORD))
      {
         fprintf(stderr, "Syntax error near redirection\n");
         return (0);
      }
      
      prev = current;
      current = current->next;
   }
   return (1);
}