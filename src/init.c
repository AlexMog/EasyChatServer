/*
** init.c for init in /home/alexmog/testwebsocket/c_version/src/src
** 
** Made by Moghrabi Alexandre
** Login   <alexandre.moghrabi@epitech.eu>
** 
** Started on  Wed May 14 21:23:00 2014 Moghrabi Alexandre
** Last update Fri Jul 18 16:41:32 2014 Moghrabi Alexandre
*/

#include <time.h>
#include <stdlib.h>
#include "init.h"
#include "server.h"

int		set_list_from_file(t_list **list, char *file_name)
{
  FILE		*file;
  char		*line = NULL;
  size_t	len;
  ssize_t	read;

  if ((file = fopen(file_name, "r")) == NULL)
    {
      fprintf(stderr, "Cannot read file: %s\n", file_name);
      return (1);
    }
  while ((read = getline(&line, &len, file)) != -1)
    if (read > 2)
      {
	line[read - 1] = 0;
	printf("\tAdded: %s\n", line);
	list_add(list, line, read * sizeof(char));
      }
  free(line);
  fclose(file);
  return (0);
}

int		init_logfile(t_datas *datas)
{
  time_t	rawtime;
  struct tm	*timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(datas->logfile, 80, "./logs/%d-%m-%Y-%H-%M-%S.log", timeinfo);
  printf("Using logfile %s\n", datas->logfile);
  return (0);
}

int		init_datas(t_datas *datas)
{
  int		ret = 0;

  printf("Reading configs file...\n");
  if (read_configs(datas))
    return (1);
  printf("Loading banned list...\n");
  ret += set_list_from_file(&datas->banned, datas->ban_file);
  printf("Loading admins list...\n");
  ret += set_list_from_file(&datas->admins, datas->admin_file);
  printf("Loading op list...\n");
  ret += set_list_from_file(&datas->opers, datas->op_file);
  printf("Opening logfile...\n");
  ret += init_logfile(datas);
  return (ret);
}

/****
***** SAVE PART
****/

int		write_file_datas(t_list *datas, char *filename)
{
  FILE		*file;
  t_list	*elem;

  if ((file = fopen(filename, "w+")) == 0)
    {
      fprintf(stderr, "Cannot open file %s.\n", filename);
      return (1);
    }
  elem = datas;
  while (elem)
    {
      printf("\tAdding %s\n", (char*)elem->datas);
      fprintf(file, "%s\n", (char*)elem->datas);
      elem = elem->next;
    }
  fclose(file);
  return (0);
}

void		save_datas(t_datas *datas)
{
  printf("Writing ban file...\n");
  write_file_datas(datas->banned, datas->ban_file);
  printf("Writing admins file...\n");
  write_file_datas(datas->admins, datas->admin_file);
  printf("Writing op file...\n");
  write_file_datas(datas->opers, datas->op_file);
}
