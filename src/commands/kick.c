/*
** whois.c for whois in /home/alexmog/testwebsocket/c_version/src/src/commands
** 
** Made by Moghrabi Alexandre
** Login   <alexandre.moghrabi@epitech.eu>
** 
** Started on  Thu May 15 12:13:56 2014 Moghrabi Alexandre
** Last update Thu May 15 18:25:03 2014 Moghrabi Alexandre
*/

#include <websock/websock.h>
#include <pthread.h>
#include "server.h"
#include "datas.h"
#include "users.h"

void		exec_kick(t_users *user, t_datas *datas, char *args)
{
  t_users	*cible;
  int		index = 0;
  char		buffer[800];
  char		*username;
  char		*reason;

  if (!has_rights(user, 1))
    {
      sendmessage(user->state, "ERROR 5 You dont have "
		  "the rights to do that!");
      return ;
    }
  if (args == NULL)
    {
      sendmessage(user->state, "ERROR 1 Need 2 arguments.");
      return ;
    }
  username = strtok(args, " ");
  reason = strtok(NULL, "\0");
  if (username == NULL)
    {
      sendmessage(user->state, "ERROR 1 User not found.");
      return ;
    }
  snprintf(buffer, 800, "KICK %s %s", username,
	   (reason == NULL ? "No reason" : reason));
  send_to_all(datas, buffer, 0, NULL);
  while ((cible = (t_users*)list_get_from_index(datas->users,
						username, &index,
						&verify_username_by_user_list)) != NULL)
    libwebsock_close(cible->state);
  if (index == 0)
    sendmessage(user->state, "ERROR 9 User not found.");
  else if (strcmp(user->username, username) != 0)
    sendmessage(user->state, "OK");
}
