#include "mini.h"

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
   
	if (commands)
    print_cmd_structure(commands);

   free_tokens(tokens);
   free_commands(commands);

   return (0);
}