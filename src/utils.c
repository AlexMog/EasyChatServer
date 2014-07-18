/*
** utils.c for utils in /home/alexmog/testwebsocket/c_version/src/src/commands
** 
** Made by Moghrabi Alexandre
** Login   <alexandre.moghrabi@epitech.eu>
** 
** Started on  Wed May 14 19:45:52 2014 Moghrabi Alexandre
** Last update Fri Jul 18 16:34:54 2014 Moghrabi Alexandre
*/

#include <stdarg.h>
#include <ctype.h>
#include <stdio.h>
#include "server.h"
#include "users.h"

/***
 *** Utility functions
 ***/
char		*to_lower(char *str)
{
  int		i = -1;

  while (str != NULL && str[++i])
    str[i] = tolower(str[i]);
  return (str);
}

int		is_numeric(char *str)
{
  if (strlen(str) <= 0)
    return (1);
  while (*str)
    {
      if (*str > '9' || *str < '0')
	return (0);
      ++str;
    }
  return (1);
}

void		send_to_all(t_datas *datas, char *msg, int needLogin,
			    t_users *except)
{
  t_list	*elem;
  t_users	*user;

  elem = datas->users;
  while (elem)
    {
      user = (t_users*)elem->datas;
      if (user != except)
	{
	  if (needLogin && user->username[0] != 0)
	    sendmessage(user->state, msg);
	  else if (!needLogin)
	    sendmessage(user->state, msg);
	}
      elem = elem->next;
    }
}

void		addlog(t_datas *datas, char *format, ...)
{
  va_list	list;
  FILE*		logfile;
  time_t	rawtime;
  struct tm	*timeinfo;
  char		buffer[80];

  va_start(list, format);
  if ((logfile = fopen(datas->logfile, "a")) == NULL)
    {
      fprintf(stderr, "Cannot write in the logfile: ");
      vfprintf(stderr, format, list);
      fprintf(stderr, "\n");
      return ;
    }
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(buffer, 80, "[%d-%m-%Y-%H:%M:%S]", timeinfo);
  fprintf(logfile, "%s", buffer);
  vfprintf(logfile, format, list);
  va_end(list);
  fclose(logfile);
}

/***
 *** User functions
 ***/
int		verify_username(void *elem, void *comp)
{
  char		*user = (char*)comp;
  char		*elemd = (char*)elem;

  return (strcmp(to_lower(elemd), to_lower(user)) == 0);
}

int		verify_username_by_user_list(void *elem, void *comp)
{
  char		*user = (char*)comp;
  t_users      	*elemd = (t_users*)elem;

  return (strcmp(to_lower(elemd->username), to_lower(user)) == 0);
}

int		has_rights(t_users *user, int type)
{
  return (user->username[0] != 0 && user->type >= type);
}

void		set_user_type(t_users *user, t_datas *datas, int send_alert)
{
  char		buffer[1024];
  int		oldtype = user->type;

  if (user->username[0] == 0)
    return ;
  if (list_get_cond(datas->admins, user->username,
		    &verify_username) != NULL)
    {
      addlog(datas, "\t%s set as Admin.\n", user->username);
      user->type = 2;
    }
  else if (list_get_cond(datas->opers, user->username,
			 &verify_username) != NULL)
    {
      addlog(datas, "\t%s set as Operator.\n", user->username);
      user->type = 1;
    }
  else
    user->type = 0;
  buffer[0] = 0;
  if (send_alert)
    {
      if (user->type != oldtype && oldtype == 2)
	snprintf(buffer, 1024, "REMADMIN %s", user->username);
      else if (user->type != oldtype && oldtype == 1)
	snprintf(buffer, 1024, "REMOP %s", user->username);
      if (buffer[0] != 0)
	send_to_all(datas, buffer, 1, NULL);
      if (user->type == 2)
	snprintf(buffer, 1024, "ADDADMIN %s", user->username);
      else if (user->type == 1)
	snprintf(buffer, 1024, "ADDOP %s", user->username);
      if (user->type > 0)
	send_to_all(datas, buffer, 1, NULL);
    }
}

void		modify_user_state(int neededRight, t_users *user,
				  t_datas *datas, char *args, int newState)
{
  t_users	*cible = NULL;
  int		i = 1;
  t_list	**list = NULL;
  int		index = 0;

  if (!has_rights(user, neededRight))
    {
      sendmessage(user->state, "ERROR 5 You dont have"
		  " the rights to do that.");
      return ;
    }
  if (args == NULL)
    {
      sendmessage(user->state, "ERROR 2 User not found.");
      return ;
    }
  if (newState != 0)
    list = (newState == 1 ? &datas->opers : &datas->admins);
  if (list_get_cond(datas->admins, args,
		    &verify_username) != NULL)
    {
      if (newState != 2)
	{
	  list_rem_cond(&datas->admins, args, &verify_username);
	  addlog(datas, "\t%s deleted from admins.\n", args);
	}
      else
	{
	  sendmessage(user->state, "ERROR 6 User is already an admin.");
	  return ;
	}
    }
  if (list_get_cond(datas->opers, args,
		    &verify_username) != NULL)
    {
      if (newState != 1)
	{
	  list_rem_cond(&datas->opers, args, &verify_username);
	  addlog(datas, "\t%s deleted from operators.\n", args);
	}
      else
	{
	  sendmessage(user->state, "ERROR 6 User is already an operator.");
	  return ;
	}
    }
  if (newState != 0 && (list == NULL || list_add(list, args,
						 (strlen(args) + 1)
						 * sizeof(char))))
    {
      sendmessage(user->state, "ERROR 500 Internal server error.");
      return ;
    }
  if (newState != 0)
    addlog(datas, "\t%s added to %s.\n", args,
	   (newState == 1 ? "operators" : "admins"));
  while ((cible = (t_users*)list_get_from_index(datas->users, args, &index,
						&verify_username_by_user_list)) != NULL)
    {
      set_user_type(cible, datas, i);
      i = 0;
    }
  sendmessage(user->state, "OK");
}

/***
 *** Ban functions
 ***/

int		add_banned(t_datas *datas, char *username, char *reason)
{
  char		buffer[1024];

  snprintf(buffer, 1024, "%s:%s", username,
	   (reason == NULL ? "No reason" : reason));
  return (list_add(&datas->banned, buffer,
		   (strlen(buffer) + 1) * sizeof(char)));
}

int		verify_ban_username(void *elem, void *comp)
{
  char		*elemd = strdup((char*)elem);
  char		*user = (char*)comp;
  char		username[250];
  char		*tmp;

  username[0] = 0;
  tmp = strtok(elemd, ":");
  if (tmp != NULL)
    {
      strcpy(username, tmp);
      free(elemd);
    }
  return (username != NULL && strcmp(to_lower(username), to_lower(user)) == 0);
}

int		verify_banned(t_datas *datas, char *username, char *reason)
{
  char		*user;
  char		*temp;
  char		*tempr;

  if ((user = list_get_cond(datas->banned, username,
			    &verify_ban_username)) != NULL)
    {
      temp = strdup(user);
      strtok(temp, ":");
      tempr = strtok(NULL, "\0");
      if (tempr != NULL)
	strcpy(reason, tempr);
      free(temp);
      return (1);
    }
  return (0);
}

int		rem_banned(t_datas *datas, char *username)
{
  return (list_rem_cond(&datas->banned, username, &verify_ban_username));
}
