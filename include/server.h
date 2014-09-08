/*
** server.h for server.h in /home/alexmog/testwebsocket/c_version/src
** 
** Made by Moghrabi Alexandre
** Login   <alexmog@epitech.net>
** 
** Started on  Thu May  8 17:23:52 2014 Moghrabi Alexandre
** Last update Mon Sep  8 16:38:30 2014 Moghrabi Alexandre
*/

#ifndef SERVER_H_
# define SERVER_H_

# include "users.h"
# include "datas.h"

# define SERVER_VERSION	(93)

# define CONFIGS_FILE	("configs.cfg")

/* server_interpretator.c */
void	interprete_command(t_users *user, char *command, char *args,
			   t_datas *datas);
/* server.c */
void	sendmessage(libwebsock_client_state *state, char *message);
/* utils.c */
char	*to_lower(char *);
int	verify_banned(t_datas *datas, char *username, char *reason);
int	add_banned(t_datas *datas, char *username, char *reason);
int	rem_banned(t_datas *datas, char *username);
int	has_rights(t_users *user, int type);
int	verify_username(void *elem, void *comp);
void	set_user_type(t_users *user, t_datas *datas, int send_alert);
void	send_to_all(t_datas *datas, char *msg, int needLogin,
		    t_users *except);
void	modify_user_state(int neededRight, t_users *user, t_datas *datas,
			  char *args, int newState);
int	verify_username_by_user_list(void *elem, void *comp);
void	addlog(t_datas *datas, char *format, ...);
int	is_numeric(char *str);
int	read_configs(t_datas *datas);
void	add_to_log(t_datas *datas, char *message, char *user);

#endif /* !SERVER_H_ */
