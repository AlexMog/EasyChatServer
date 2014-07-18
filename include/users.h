/*
** users.h for users in /home/alexmog/testwebsocket/c_version
** 
** Made by Moghrabi Alexandre
** Login   <alexmog@epitech.net>
** 
** Started on  Thu May  8 16:54:34 2014 Moghrabi Alexandre
** Last update Tue Jun  3 18:32:02 2014 Moghrabi Alexandre
*/

#ifndef USERS_H_
# define USERS_H_

#include <websock/websock.h>
#include <time.h>

typedef struct _users
{
  libwebsock_client_state *state;
  char	username[250];
  int	id;
  int	type;
  char	client[250];
  int	protocol_version;
  struct timeval last_message;
}t_users;

#endif /* !USERS_H_ */
