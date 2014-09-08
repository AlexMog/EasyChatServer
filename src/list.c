/*
** clients.c for clients in /home/alexmog/projets/projets/PSU_2013_myftp/s_dir/src
** 
** Made by Moghrabi Alexandre
** Login   <alexandre.moghrabi@epitech.eu>
** 
** Started on  Tue Apr  8 17:08:40 2014 Moghrabi Alexandre
** Last update Mon Sep  8 16:28:22 2014 Moghrabi Alexandre
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <websock/websock.h>
#include "list.h"
#include "server.h"

int		list_add_end(t_list **list, void *datas, size_t data_size)
{
  t_list	*tmp;

  if ((tmp = malloc(sizeof(t_list))) == NULL)
    return (1);
  if ((tmp->datas = malloc(data_size)) == NULL)
    return (1);
  memcpy(tmp->datas, datas, data_size);
  tmp->next = NULL;
  if (*list == NULL)
    *list = tmp;
  else
    {
      t_list	*elem = *list;

      while (elem && elem->next)
	elem = elem->next;
      elem->next = tmp;
    }
  return (0);
}

int		list_add(t_list **list, void *datas, size_t data_size)
{
  t_list	*next;

  next = NULL;
  if (*list != NULL)
    next = *list;
  if ((*list = malloc(sizeof(t_list))) == NULL)
    return (1);
  if (((*list)->datas = malloc(data_size)) == NULL)
    return (1);
  memcpy((*list)->datas, datas, data_size);
  (*list)->next = next;
  return (0);
}

int		list_rem_front(t_list **list)
{
  t_list	*elem;

  if (*list == NULL)
    return (1);
  if ((*list)->next == NULL)
    {
      if ((*list)->datas != NULL)
	free((*list)->datas);
      free(*list);
      *list = NULL;
    }
  else
    {
      elem = (*list)->next;
      if ((*list)->datas != NULL)
	free((*list)->datas);
      free(*list);
      *list = elem;
    }
  return (0);
}

int		list_rem_cond(t_list **list, void *comp,
			      int  (*fptr)(void *, void *))
{
  t_list	*elem;
  t_list	*temp;
  t_list	*temp2;

  temp2 = NULL;
  if ((*list) == NULL || fptr((*list)->datas, comp))
    return (list_rem_front(list));
  elem = *list;
  while (elem)
    {
      temp = elem->next;
      if (elem->datas != NULL && fptr(elem->datas, comp))
	{
	  if (temp2 != NULL)
	    temp2->next = temp;
	  if (elem->datas != NULL)
	    free(elem->datas);
	  free(elem);
	  return (0);
	}
      temp2 = elem;
      elem = temp;
    }
  return (1);
}

void		*list_get_cond(t_list *list, void *comp,
			       int (*fptr)(void *, void *))
{
  t_list	*elem;

  elem = list;
  while (elem)
    {
      if (elem->datas != NULL && fptr(elem->datas, comp))
	return (elem->datas);
      elem = elem->next;
    }
  return (NULL);
}

void		*list_get_from_index(t_list *list, void *comp, int *index,
				     int (*fptr)(void *, void *))
{
  t_list	*elem;
  int		i;

  i = 0;
  elem = list;
  while (elem)
    {
      if (i >= *index)
	{
	  if (elem->datas != NULL && fptr(elem->datas, comp))
	    {
	      *index = i + 1;
	      return (elem->datas);
	    }
	}
      elem = elem->next;
      ++i;
    }
  return (NULL);
}

void		list_clean(t_list **list)
{
  t_list	*elem;
  t_list	*next;

  elem = *list;
  while (elem)
    {
      next = elem->next;
      if (elem->datas != NULL)
	free(elem->datas);
      free(elem);
      elem = next;
    }
  *list = NULL;
}

void		list_rem_back(t_list **list)
{
  t_list	*elem = *list;
  t_list	*tmp = NULL;

  while (elem)
    {
      if (elem->next == NULL)
	{
	  if (elem->datas != NULL)
	    free(elem->datas);
	  free(elem);
	  if (tmp != NULL)
	    tmp->next = NULL;
	  else
	    *list = NULL;
	  return ;
	}
      tmp = elem;
      elem = elem->next;
    }
}
