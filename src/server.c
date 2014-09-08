/*
** server.c for Server-Melinyel-Tchat in /home/alexmog/testwebsocket/c_version
** 
** Made by Moghrabi Alexandre
** Login   <alexandre.moghrabi@epitech.eu>
** 
** Started on  Thu May  8 16:44:40 2014 Moghrabi Alexandre
** Last update Mon Sep  8 16:38:12 2014 Moghrabi Alexandre
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <websock/websock.h>
#include <string.h>
#include "users.h"
#include "datas.h"
#include "server.h"
#include "init.h"

long		timevaldiff(struct timeval *starttime, struct timeval *finishtime)
{
  long		msec;
  
  msec = (finishtime->tv_sec - starttime->tv_sec) * 1000;
  msec += (finishtime->tv_usec - starttime->tv_usec) / 1000;
  return (msec);
}

int		test_sock(void *elem, void *comp)
{
  int		*socket = (int*)comp;
  t_users	*user = (t_users*)elem;

  return (*socket == user->state->sockfd);
}

void	sendmessage(libwebsock_client_state *state, char *message)
{
  char	buffer[1024];

  if (state != NULL)
    {
      if (strlen(message) > 1020)
	message[1020] = 0;
      snprintf(buffer, 1024, "%s\r\n", message);
      addlog(((t_datas*)((libwebsock_context*)state->ctx)->user_data),
	     "To %d : %s\n", state->sockfd, message);
      libwebsock_send_text(state, message);
    }
}

int	onmessage(libwebsock_client_state *state, libwebsock_message *msg)
{
  char	*command = NULL;
  t_datas	*datas;
  char	*args = NULL;
  t_users	*client;

  datas = (t_datas*)((libwebsock_context*)state->ctx)->user_data;
  addlog(datas, "Client %d: %s", state->sockfd, msg->payload);
  if (strstr(msg->payload, "\r\n") == NULL)
    {
      addlog(datas, "\tMessage dont contain \\r\\n, dont accept it.\n");
      sendmessage(state, "ERROR 0 no \\r\\n found.");
      return (0);
    }
  if (strstr(msg->payload, " ") != NULL)
    {
      command = strtok(msg->payload, " ");
      args = strtok(NULL, "\r\n");
    }
  else
    command = strtok(msg->payload, "\r\n");
  if (command != NULL)
    {
      struct timeval current_time;
      addlog(datas, "\tCommand found: %s\n", command);
      client = (t_users*)list_get_cond(datas->users,
				       &state->sockfd, &test_sock);
      if (gettimeofday(&current_time, NULL) != -1)
	{
	  if (client->last_message.tv_sec != 0 && timevaldiff(&current_time, &client->last_message) <= 500)
	    {
	      // TODO FLOOD LOCK
	    }
	  memcpy(&client->last_message, &current_time, sizeof(current_time));
	}
      interprete_command(client, command, args, datas);
    }
  return (0);
}

int	onopen(libwebsock_client_state *state)
{
  t_users	user;
  t_datas	*datas;

  datas = (t_datas*)(((libwebsock_context*)state->ctx)->user_data);
  memset(&user, 0, sizeof(t_users));
  addlog(datas, "Client connected: %d\n", state->sockfd);
  user.state = state;
  pthread_mutex_lock(&datas->mutex);
  if (list_add(&datas->users, &user, sizeof(t_users)))
    {
      sendmessage(state, "ERROR 500 Internal server error, contact an admin!");
      libwebsock_close(state);
    }
  pthread_mutex_unlock(&datas->mutex);
  return (0);
}

int	onclose(libwebsock_client_state *state)
{
  t_list	*elem;
  t_users	*user;
  t_datas	*datas;
  char		buffer[500];

  datas = (t_datas*)(((libwebsock_context*)state->ctx)->user_data);
  addlog(datas, "Client closed: %d\n", state->sockfd);
  user = (t_users*)list_get_cond(datas->users, &state->sockfd, &test_sock);
  buffer[0] = 0;
  if (user != NULL && user->state != NULL)
    snprintf(buffer, 500, "QUIT %s:%d", user->username, user->id);
  if (user->username[0] != 0)
    {
      elem = datas->users;
      while (elem)
	{
	  if (elem->datas != user &&
	      ((t_users*)elem->datas)->state != NULL && buffer[0] != 0)
	    sendmessage(((t_users*)elem->datas)->state, buffer);
	  elem = elem->next;
	}
    }
  pthread_mutex_lock(&datas->mutex);
  list_rem_cond(&datas->users, &state->sockfd, &test_sock);
  pthread_mutex_unlock(&datas->mutex);
  return (0);
}

void	clean_datas(t_datas *datas)
{
  pthread_mutex_destroy(&datas->mutex);
  list_clean(&datas->users);
  list_clean(&datas->banned);
  list_clean(&datas->admins);
  list_clean(&datas->opers);
  list_clean(&datas->logs);
}

int	main(void)
{
  t_datas	datas;
  libwebsock_context *ctx = NULL;

  memset(&datas, 0, sizeof(t_datas));
  datas.version = SERVER_VERSION;
  ctx = libwebsock_init();
  if (ctx == NULL)
    {
      fprintf(stderr, "Error during libwebsock_init.\n");
      return (1);
    }
  ctx->user_data = &datas;
  printf("Starting datas initialisation...\n");
  if (init_datas(&datas))
    {
      clean_datas(&datas);
      return (1);
    }
  printf("Initialisation finished.\nListening on port %s\n", datas.port);
  addlog(&datas, "server listening on port %s\n", datas.port);
  libwebsock_bind(ctx, "0.0.0.0", datas.port);
  // Set listeners
  ctx->onmessage = onmessage;
  ctx->onopen = onopen;
  ctx->onclose = onclose;
  // Setting mutex
  pthread_mutex_init(&datas.mutex, NULL);
  // Loop server
  libwebsock_wait(ctx);
  // Saving datas
  save_datas(&datas);
  // Cleans here
  clean_datas(&datas);
  addlog(&datas, "Exiting.\n");
  return (0);
}
