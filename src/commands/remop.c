/*
** whois.c for whois in /home/alexmog/testwebsocket/c_version/src/src/commands
** 
** Made by Moghrabi Alexandre
** Login   <alexandre.moghrabi@epitech.eu>
** 
** Started on  Thu May 15 12:13:56 2014 Moghrabi Alexandre
** Last update Thu May 15 14:43:04 2014 Moghrabi Alexandre
*/

#include "server.h"
#include "datas.h"
#include "users.h"

void	exec_remop(t_users *user, t_datas *datas, char *args)
{
  modify_user_state(2, user, datas, args, 0);
}
