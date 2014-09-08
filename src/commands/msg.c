/*
** msg.c for msg in /home/alexmog/testwebsocket/c_version/src/src/commands
** 
** Made by Moghrabi Alexandre
** Login   <alexandre.moghrabi@epitech.eu>
** 
** Started on  Thu May  8 19:14:04 2014 Moghrabi Alexandre
** Last update Mon Sep  8 16:19:15 2014 Moghrabi Alexandre
*/

#include <websock/websock.h>
#include "server.h"
#include "datas.h"
#include "users.h"

void		exec_msg(t_users *user, t_datas *datas, char *msg)
{
  char		buffer[1024];

  if (msg == NULL)
    {
      sendmessage(user->state, "ERROR 1 Message not found.");
      return ;
    }
  if (user->username[0] == 0)
    {
      sendmessage(user->state, "ERROR 2 Not logged in.");
      return ;
    }
  snprintf(buffer, 1024, "MSG %s %s", user->username, msg);
  send_to_all(datas, buffer, 0, user);
  sendmessage(user->state, "OK");
  /* Adding to logs */
  add_to_log(datas, msg, user->username);
}
