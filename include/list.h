/*
** list.h for list in /home/alexmog/testwebsocket/c_version/src/src
** 
** Made by Moghrabi Alexandre
** Login   <alexmog@epitech.net>
** 
** Started on  Wed May 14 20:13:27 2014 Moghrabi Alexandre
** Last update Mon Sep  8 16:25:58 2014 Moghrabi Alexandre
*/

#ifndef LIST_H_
# define LIST_H_

typedef struct	s_list
{
  void		*datas;
  struct s_list	*next;
}		t_list;

int		list_add(t_list **list, void *datas, size_t data_size);
int		list_rem_front(t_list **list);
int		list_rem_cond(t_list **list, void *comp,
			      int (*fptr)(void *, void *));
void		*list_get_cond(t_list *list, void *comp,
			       int (*fptr)(void *, void *));
void		*list_get_from_index(t_list *list, void *comp, int *index,
				     int (*fptr)(void *, void *));
void		list_clean(t_list **list);
void		list_rem_back(t_list **list);

#endif /* !LIST_H_ */
