/*
** client.c for client in /home/alexmog/testwebsocket/c_version/src/src/commands
** 
** Made by Moghrabi Alexandre
** Login   <alexandre.moghrabi@epitech.eu>
** 
** Started on  Wed May 14 19:11:32 2014 Moghrabi Alexandre
** Last update Wed May 14 19:53:00 2014 Moghrabi Alexandre
*/

#include <string.h>
#include <stdlib.h>
#include "server.h"
#include "datas.h"
#include "users.h"

void	exec_client(t_users *user, t_datas *datas, char *args)
{
  char	*protocol_version;
  char	*client;
  int	version;

  client = strtok(args, " ");
  protocol_version = strtok(NULL, "\0");
  if (client == NULL || protocol_version == NULL)
    {
      sendmessage(user->state, "ERROR 1 Need at last 2 arguments.");
      return ;
    }
  version = atoi(protocol_version);
  if (version != datas->version)
    {
      sendmessage(user->state, "ERROR 4 Protocol version is not the same.");
      return ;
    }
  snprintf(user->client, 250, "%s", client);
  user->protocol_version = version;
  sendmessage(user->state, "OK");
}
