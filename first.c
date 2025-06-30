#include "mini.h"

t_token *tokenize_word(char *input, int *i)
{
   char    *word;
   int     start = *i;
   int     len = 0;
   int     in_quotes = 0;
   char    quote_char = 0;
   int     quoted = 0;
   
   if (!input || !i) // why i ??
      return (NULL);
   
   while (input[*i])
   {
      char c = input[*i];
      
      // Gestion des quotes
      if (!in_quotes && (c == '\'' || c == '"'))
      {
         in_quotes = 1;
         quoted = (c == '\'') ? 1 : 2;
         quote_char = c;
         (*i)++;
         start = *i;
         continue;
      }
      else if (in_quotes && c == quote_char)
      {
         in_quotes = 0;
         (*i)++;
         continue;  // On sort après la quote fermante
      }
      
        // Si on n'est pas dans des quotes et qu'on trouve un délimiteur
      if (!in_quotes && is_delimiter(c))
            break;
      
      (*i)++;
      len++;
   }
   
   // Cas spécial: quote non fermée
   if (in_quotes)
   {
      fprintf(stderr, "Error: unclosed quote `%c'\n", quote_char);
      return (NULL);
   }
   
    // Cas où on n'a rien à tokenizer (ne devrait pas arriver normalement)
   if (len == 0)
      return (NULL);
   
    // Créer le token avec le contenu
   word = ft_substr(input, start, len);
   if (!word)
      return (NULL);
      
   t_token *token = create_token(TOKEN_WORD, word);
   free(word); // create_token fait une copie avec strdup
   
   if (token)
      token->quoted = quoted;
   
   return (token);
}

t_token *tokenize(char *input)
{
   t_token *head = NULL;
   t_token *current = NULL;
   int     i = 0;
   
   if (!input)
      return (NULL);
   
   while (input[i])
   {
      while (input[i] && is_whitespace(input[i]))
         i++;
      
      if (!input[i])
         break;
      
      if (input[i] == '|')
      {
         current = create_token(TOKEN_PIPE, "|");
         i++;
      }
      else if (input[i] == '<')
      {
         if (input[i + 1] == '<')
         {
            current = create_token(TOKEN_HEREDOC, "<<");
            i += 2;
         }
         else
         {
            current = create_token(TOKEN_REDIR_IN, "<");
            i++;
         }
      }
      else if (input[i] == '>')
      {
         if (input[i + 1] == '>')
         {
            current = create_token(TOKEN_REDIR_APPEND, ">>");
            i += 2;
         }
         else
         {
            current = create_token(TOKEN_REDIR_OUT, ">");
            i++;
         }
      }
      else if ((input[i] == '<' && input[i+1] == '<' && input[i+2] == '<') || 
               (input[i] == '>' && input[i+1] == '>' && input[i+2] == '>'))
      {
         fprintf(stderr, "minishell: syntax error near unexpected token\n");
         free_tokens(head);
         return (NULL);
      }
      else
      {
         current = tokenize_word(input, &i);
         // tokenize_word gère déjà l'incrémentation de i
      }
      
      if (!current)
      {
         free_tokens(head);
         return (NULL);
      }
      
      add_token_to_list(&head, current);
   }
   
   // Ajouter un token EOF à la fin
   current = create_token(TOKEN_EOF, "");
   if (current)
      add_token_to_list(&head, current);
   
   return (head);
}

int main(int argc, char **argv, char **envp)
{
   char *line;
   t_token *tokens = NULL;
   t_cmd *commands = NULL;
   
   if (argc < 2)
   {
      printf("Usage: %s \"command\"\n", argv[0]);
      return (1);
   }
   
   line = ft_strdup(argv[1]);
   if (!line)
      return (1);
   
   tokens = tokenize(line);
   free(line);
   
   if (!tokens || !check_syntax(tokens))
   {
      free_tokens(tokens);
      return (1);
   }
   
   expand_tokens(tokens, envp, 0);
   commands = parse_tokens(tokens);
   
   free_tokens(tokens);
   free_commands(commands);
   
   return (0);
}