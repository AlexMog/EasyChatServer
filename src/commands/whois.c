/*
** whois.c for whois in /home/alexmog/testwebsocket/c_version/src/src/commands
** 
** Made by Moghrabi Alexandre
** Login   <alexandre.moghrabi@epitech.eu>
** 
** Started on  Thu May 15 12:13:56 2014 Moghrabi Alexandre
** Last update Thu May 15 18:29:11 2014 Moghrabi Alexandre
*/

#include "server.h"
#include "datas.h"
#include "users.h"

void		exec_whois(t_users *user, t_datas *datas, char *args)
{
  char		buffer[1024];
  t_users	*cible;
  int		index = 0;

  if (user->username[0] == 0)
    {
      sendmessage(user->state, "ERROR 2 Not logged in.");
      return ;
    }
  if (args == NULL)
    {
      sendmessage(user->state, "ERROR 1 Need 1 argument.");
      return ;
    }
  while ((cible = (t_users*)list_get_from_index(datas->users,
						args, &index,
						&verify_username_by_user_list)) != NULL)
    {
      snprintf(buffer, 1024, "WHOIS %d %s %d", cible->id, cible->client,
	       cible->type);
      sendmessage(user->state, buffer);
    }
  if (index == 0)
    sendmessage(user->state, "ERROR 9 User not connected.");
  else
    sendmessage(user->state, "OK");
}
