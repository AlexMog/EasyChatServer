/*
** login.c for login in /home/alexmog/testwebsocket/c_version/src/src/commands
** 
** Made by Moghrabi Alexandre
** Login   <alexandre.moghrabi@epitech.eu>
** 
** Started on  Thu May  8 19:13:45 2014 Moghrabi Alexandre
** Last update Mon Sep  8 16:44:09 2014 Moghrabi Alexandre
*/

#include <pthread.h>
#include <curl/curl.h>
#include <websock/websock.h>
#include "server.h"
#include "datas.h"
#include "users.h"
#include "web_reader.h"
#include "commands.h"
#include "logs.h"

void		exec_login(t_users *user, t_datas *datas, char *args)
{
  char		*user_c;
  char		*pass;
  char		buffer[500];
  char		*web_response;
  int		rep;
  char		banreason[1024];

  if (user->username[0] != 0)
    {
      sendmessage(user->state, "ERROR 2 Allready logged in.");
      return ;
    }
  if (user->client[0] == 0)
    {
      sendmessage(user->state, "ERROR 3 CLIENT unset.");
      return ;
    }
  if (args == NULL)
    {
      sendmessage(user->state, "ERROR 1 No arguments found.");
      return ;
    }
  user_c = strtok(args, " ");
  pass = strtok(NULL, "\0");
  if (datas->auth_use == 1)
    {
      if (user_c == NULL || pass == NULL || strlen(pass) <= 2)
	{
	  sendmessage(user->state, "ERROR 1 Usage: LOGIN <user> <password>");
	  return ;
	}
      if (verify_banned(datas, user_c, banreason))
	{
	  snprintf(buffer, 500, "ERROR 10 %s",
		   (banreason[0] == 0 ? "No reason." : banreason));
	  sendmessage(user->state, buffer);
	  return ;
	}
      snprintf(buffer, 500, datas->auth_link, user_c, pass);
      if ((web_response = do_web_request(buffer)) == NULL)
	{
	  printf("Cannot connect to auth website.\n");
	  sendmessage(user->state, "ERROR 2 Cannot contact auth api.\n");
	  return ;
	}
      rep = atoi(web_response);
      printf("\rWeb rep: %s\n", web_response);
      if (web_response[0] > '9' || web_response[0] < '0')
	rep = -1;
      free(web_response);
      if (rep < 0)
	{
	  sendmessage(user->state, "ERROR 2 Login failed.\n");
	  return ;
	}
      user->id = rep;
    }
  else if (user_c == NULL || strlen(pass) <= 2)
    {
      sendmessage(user->state, "ERROR 1 Usage: LOGIN <user>");
      return ;
    }
  snprintf(user->username, 250, "%s", user_c);
  set_user_type(user, datas, 0);
  snprintf(buffer, 500, "JOIN %s:%d:%d", user->username, user->id,
	   user->type);
  send_to_all(datas, buffer, 1, NULL);
  sendmessage(user->state, "OK");
  exec_list(user, datas, args);
  /* Sending logs */
  t_list *elem = datas->logs;
  while (elem)
    {
      t_logs *log = ((t_logs*)elem->datas);
      snprintf(banreason, 1023, "LOG %s %s", log->login,
	       log->message);
      sendmessage(user->state, banreason);
      elem = elem->next;
    }
}
