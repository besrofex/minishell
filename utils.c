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
