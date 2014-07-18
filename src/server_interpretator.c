/*
** server_interpretator.c for server_interpretator in /home/alexmog/testwebsocket/c_version/src
** 
** Made by Moghrabi Alexandre
** Login   <alexandre.moghrabi@epitech.eu>
** 
** Started on  Thu May  8 17:43:16 2014 Moghrabi Alexandre
** Last update Thu May 15 18:24:56 2014 Moghrabi Alexandre
*/

#include <websock/websock.h>
#include "users.h"
#include "server.h"
#include "datas.h"
#include "commands.h"

void	set_tabs(char **commands,
		 void (**fcnt)(t_users *, t_datas *, char *))
{
  commands[LOGIN] = "login";
  commands[LIST] = "list";
  commands[MSG] = "msg";
  commands[PRIVMSG] = "privmsg";
  commands[CLIENT] = "client";
  commands[WHOIS] = "whois";
  commands[ADDADMIN] = "addadmin";
  commands[REMADMIN] = "remadmin";
  commands[ADDOP] = "addop";
  commands[REMOP] = "remop";
  commands[KICK] = "kick";
  commands[BAN] = "ban";
  commands[UNBAN] = "unban";
  fcnt[LOGIN] = &exec_login;
  fcnt[LIST] = &exec_list;
  fcnt[MSG] = &exec_msg;
  fcnt[PRIVMSG] = &exec_privmsg;
  fcnt[CLIENT] = &exec_client;
  fcnt[WHOIS] = &exec_whois;
  fcnt[ADDADMIN] = &exec_addadmin;
  fcnt[REMADMIN] = &exec_remadmin;
  fcnt[ADDOP] = &exec_addop;
  fcnt[REMOP] = &exec_remop;
  fcnt[KICK] = &exec_kick;
  fcnt[BAN] = &exec_ban;
  fcnt[UNBAN] = &exec_unban;
}

void	interprete_command(t_users *user, char *command, char *args,
			   t_datas *datas)
{
  char	*commands[CMDS_END];
  void	(*fcnts[CMDS_END])(t_users *, t_datas *, char *);
  int	i = -1;

  set_tabs(commands, fcnts);
  to_lower(command);
  while (++i < CMDS_END)
    if (strcmp(commands[i], command) == 0)
      {
	pthread_mutex_lock(&datas->mutex);
	fcnts[i](user, datas, args);
	pthread_mutex_unlock(&datas->mutex);
	return ;
      }
  sendmessage(user->state, "ERROR 0 Command not found!");
}
