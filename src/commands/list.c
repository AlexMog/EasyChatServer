/*
** list.c for list in /home/alexmog/testwebsocket/c_version/src/src/commands
** 
** Made by Moghrabi Alexandre
** Login   <alexandre.moghrabi@epitech.eu>
** 
** Started on  Thu May  8 19:13:53 2014 Moghrabi Alexandre
** Last update Thu May 15 12:30:44 2014 Moghrabi Alexandre
*/

#include <websock/websock.h>
#include "server.h"
#include "datas.h"
#include "users.h"

void		send_userlist(t_users *user, t_datas *datas)
{
  char		buffer[1020];
  t_list	*elem;
  char		to_send = 0;
  char		buffer_name[502];
  t_users	*userd;

  strcpy(buffer, "LIST");
  elem = datas->users;
  while (elem)
    {
      userd = (t_users*)elem->datas;
      if (userd->username[0] != 0
	  && strlen(buffer) + strlen(userd->username) < 600)
	{
	  snprintf(buffer_name, 502, " %s:%d:%d", userd->username,
		   userd->id, userd->type);
	  strcat(buffer, buffer_name);
	  to_send = 1;
	}
      else if (userd->username[0] != 0)
	{
	  sendmessage(user->state, buffer);
	  snprintf(buffer_name, 502, " %s:%d:%d", userd->username,
		   userd->id, userd->type);
	  strcpy(buffer, "LIST");
	  strcat(buffer, buffer_name);
	  to_send = 0;
	}
      elem = elem->next;
    }
  if (to_send)
    sendmessage(user->state, buffer);
}

void		exec_list(t_users *user, t_datas *datas, char *args)
{
  (void)args;
  if (user->username[0] == 0)
    {
      sendmessage(user->state, "ERROR 2 Not logged in.");
      return ;
    }
  send_userlist(user, datas);
  sendmessage(user->state, "OK");
}
