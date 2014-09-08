/*
** configs_reader.c for melichat in /home/alexmog/testwebsocket/c_version/src/src
** 
** Made by Moghrabi Alexandre
** Login   <alexandre.moghrabi@epitech.eu>
** 
** Started on  Fri Jul 18 15:49:22 2014 Moghrabi Alexandre
** Last update Mon Sep  8 16:14:05 2014 Moghrabi Alexandre
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "server.h"

char	        *purge_line(char *line)
{
  int i = -1;

  while (line[++i])
    if (line[i] == '\t')
      line[i] = ' ';
  i = 0;
  while (line[i++] == ' ');
  return (strdup(&line[--i]));
}

int		interprete_line(t_datas *datas, char *line)
{
  char		*save = strdup(line);
  char		*option;
  char		*value;

  if (save != NULL)
    {
      if ((option = strtok(save, "=")) == NULL)
	{
	  fprintf(stderr, "ERROR: Option not found.");
	  return (1);
	}
      value = strtok(NULL, "\0");
      if (strcmp(option, "server_port") == 0)
	{
	  if (value == NULL || !is_numeric(value))
	    {
	      fprintf(stderr, "ERROR: value must be numeric.");
	      return (1);
	    }
	  strcpy(datas->port, value);
	  printf("CONFIGS: port set on %s\n", datas->port);
	}
      else if (strcmp(option, "auth_use") == 0)
	{
	  if (value == NULL)
	    {
	      fprintf(stderr, "ERROR: A value must be set.");
	      return (1);
	    }
	  if (value[0] == '0')
	    datas->auth_use = 0;
	  else if (value[0] == '1')
	    datas->auth_use = 1;
	  else
	    {
	      fprintf(stderr, "ERROR: value must be 1 or 0.");
	      return (1);
	    }
	  printf("CONFIGS: use authentification: %s\n", (datas->auth_use == 0 ? "false" : "true"));
	}
      else if (strcmp(option, "auth_link") == 0)
	{
	  if (value != NULL)
	    strcpy(datas->auth_link, value);
	  printf("CONFIGS: Using authlink: %s\n", (datas->auth_link != NULL ? value : "None."));
	}
      else if (strcmp(option, "admin_file") == 0)
	{
	  if (value == NULL)
	    {
	      fprintf(stderr, "ERROR: No file defined.");
	      return (1);
	    }
	  strcpy(datas->admin_file, value);
	  printf("CONFIGS: AdminFile: %s\n", datas->admin_file);
	}
      else if (strcmp(option, "op_file") == 0)
	{
	  if (value == NULL)
	    {
	      fprintf(stderr, "ERROR: No file defined.");
	      return (1);
	    }
	  strcpy(datas->op_file, value);
	  printf("CONFIGS: OperatorFile: %s\n", datas->op_file);
	}
      else if (strcmp(option, "ban_file") == 0)
	{
	  if (value == NULL)
	    {
	      fprintf(stderr, "ERROR: No file defined.");
	      return (1);
	    }
	  strcpy(datas->ban_file, value);
	  printf("CONFIGS: BanFile: %s\n", datas->ban_file);
	}
      else if (strcmp(option, "max_logs") == 0)
	{
	  if (value == NULL || !is_numeric(value))
	    {
	      fprintf(stderr, "ERROR: No number defined.");
	      return (1);
	    }
	  datas->max_logs = atoi(value);
	  printf("CONFIGS: MaxLogs: %d\n", datas->max_logs);
	}
      else
	fprintf(stderr, "WARNING: Option '%s' not defined.\n", option);
      free(save);
    }
  return (0);
}

int		read_configs(t_datas *datas)
{
  FILE		*fp;
  char		*line = NULL;
  size_t	len = 0;
  ssize_t	read;
  char		*purged_line;
  int		linenb = 1;

  if ((fp = fopen(CONFIGS_FILE, "r")) == NULL)
    return (1);
  while ((read = getline(&line, &len, fp)) != -1) {
    line[read - 1] = 0;
    purged_line = purge_line(line);
    if (strlen(purged_line) > 0 && purged_line[0] != '#'
	&& interprete_line(datas, purged_line))
	{
	  fprintf(stderr, " On line %d\n", linenb);
	  free(line);
	  fclose(fp);
	  free(purged_line);
	  return (1);
	}
    free(purged_line);
    ++linenb;
  }
  free(line);
  fclose(fp);
  return (0);
}
