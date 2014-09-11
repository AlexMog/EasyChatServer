/*
** privmsg.c for privmsg in /home/alexmog/testwebsocket/c_version/src/src/commands
** 
** Made by Moghrabi Alexandre
** Login   <alexandre.moghrabi@epitech.eu>
** 
** Started on  Tue May 13 17:40:55 2014 Moghrabi Alexandre
** Last update Wed May 14 20:55:09 2014 Moghrabi Alexandre
*/

#include <string.h>
#include <stdio.h>
#include "datas.h"
#include "users.h"
#include "server.h"

int		test_username(void *elem, void *comp)
{
  char		*username = (char*)comp;
  t_users	*user = (t_users*)elem;

  return (strcmp(user->username, username) == 0);
}

void		exec_privmsg(t_users *user, t_datas *datas, char *args)
{
  char		*userc;
  char		*message;
  t_users	*userd;
  char		buffer[750];
  int		start = 0;

  if (user->username[0] == 0)
    {
      sendmessage(user->state, "ERROR 2 Not logged in.");
      return ;
    }
  userc = strtok(args, " ");
  message = strtok(NULL, "\0");
  if (userc == NULL)
    {
      sendmessage(user->state, "ERROR 1 User not found.");
      return ;
    }
  if (message == NULL)
    {
      sendmessage(user->state, "ERROR 1 Message not found.");
      return ;
    }
  while ((userd = (t_users*)list_get_from_index(datas->users,
						userc, &start,
						&test_username)) != NULL)
    {
      snprintf(buffer, 750, "PRIVMSG %s %s", user->username, message);
      sendmessage(userd->state, buffer);
    }
  if (start == 0)
    {
      sendmessage(user->state, "ERROR 9 User not connected.");
      return ;
    }
  sendmessage(user->state, "OK");
}
