/*
** whois.c for whois in /home/alexmog/testwebsocket/c_version/src/src/commands
** 
** Made by Moghrabi Alexandre
** Login   <alexandre.moghrabi@epitech.eu>
** 
** Started on  Thu May 15 12:13:56 2014 Moghrabi Alexandre
** Last update Thu May 15 18:26:02 2014 Moghrabi Alexandre
*/

#include <pthread.h>
#include "server.h"
#include "datas.h"
#include "users.h"

void		exec_ban(t_users *user, t_datas *datas, char *args)
{
  t_users	*cible;
  char		*username;
  char		*reason = NULL;
  char		buffer[1024];
  int		index = 0;

  if (!has_rights(user, 1))
    {
      sendmessage(user->state, "ERROR 5 You dont have "
		  "the rights to do that!");
      return ;
    }
  if (args == NULL)
    {
      sendmessage(user->state, "ERROR 1 Need 2 arguments!");
      return ;
    }
  username = strtok(args, " ");
  if (username == NULL)
    {
      sendmessage(user->state, "ERROR 1 No username found.");
      return ;
    }
  reason = strtok(NULL, "\0");
  if (verify_banned(datas, username, buffer))
    {
      sendmessage(user->state, "ERROR 8 User already banned.");
      return ;
    }
  if (add_banned(datas, username, reason))
    {
      sendmessage(user->state, "ERROR 500 Internal server error.");
      return ;
    }
  snprintf(buffer, 1024, "BAN %s %s", username,
	   (reason == NULL ? "No reason" : reason));
  send_to_all(datas, buffer, 0, NULL);
  while ((cible = (t_users*)list_get_from_index(datas->users,
						username, &index,
						&verify_username_by_user_list)) != NULL)
    libwebsock_close(cible->state);
  if (strcmp(user->username, username) != 0)
    sendmessage(user->state, "OK");
}
