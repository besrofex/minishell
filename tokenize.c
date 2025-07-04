# include "mini.h"

t_token *tokenize_word(char *input, int *i)
{
   char    *word;
   int     start = *i;
   int     len = 0;
   int     in_quotes = 0;
   char    quote_char = 0;
   int     quoted = 0;
	char c;
   
   if (!input || !i) // why i ??
      return (NULL);
   
   while (input[*i])
   {
      c = input[*i];
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
         current = tokenize_word(input, &i); // tokenize_word gère déjà l'incrémentation de i
      if (!current)
      {
         free_tokens(head);
         return (NULL);
		}
      add_token_to_list(&head, current);
   }
   current = create_token(TOKEN_EOF, ""); // Ajouter un token EOF à la fin
   if (current)
      add_token_to_list(&head, current);
   return (head);
}