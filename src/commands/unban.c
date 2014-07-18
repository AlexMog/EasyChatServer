/*
** whois.c for whois in /home/alexmog/testwebsocket/c_version/src/src/commands
** 
** Made by Moghrabi Alexandre
** Login   <alexandre.moghrabi@epitech.eu>
** 
** Started on  Thu May 15 12:13:56 2014 Moghrabi Alexandre
** Last update Thu May 15 18:21:20 2014 Moghrabi Alexandre
*/

#include "server.h"
#include "datas.h"
#include "users.h"

void	exec_unban(t_users *user, t_datas *datas, char *args)
{
  char	buffer[1024];

  if (!has_rights(user, 1))
    {
      sendmessage(user->state, "ERROR 5 You dont have "
		  "the rights to do that!");
      return ;
    }
  if (args == NULL)
    {
      sendmessage(user->state, "ERROR 1 Need 1 argument.");
      return ;
    }
  if (!verify_banned(datas, args, buffer))
    {
      sendmessage(user->state, "ERROR 7 User not banned.");
      return ;
    }
  if (rem_banned(datas, args))
    sendmessage(user->state, "ERROR 500 Internal server error.");
  else
    sendmessage(user->state, "OK");
}
