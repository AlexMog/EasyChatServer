/*
** commands.h for commands in /home/alexmog/testwebsocket/c_version/src/src/commands
** 
** Made by Moghrabi Alexandre
** Login   <alexmog@epitech.net>
** 
** Started on  Thu May  8 19:14:23 2014 Moghrabi Alexandre
** Last update Thu May 15 16:41:35 2014 Moghrabi Alexandre
*/

#ifndef COMMANDS_H_
# define COMMANDS_H_

# include "users.h"
# include "datas.h"

typedef enum
  {
    LOGIN = 0,
    LIST,
    MSG,
    PRIVMSG,
    CLIENT,
    WHOIS,
    ADDADMIN,
    REMADMIN,
    ADDOP,
    REMOP,
    KICK,
    BAN,
    UNBAN,
    CMDS_END
  }	e_commands;

void	exec_login(t_users *user, t_datas *datas, char *args);
void	exec_list(t_users *user, t_datas *datas, char *args);
void	exec_msg(t_users *user, t_datas *datas, char *args);
void	exec_privmsg(t_users *user, t_datas *datas, char *args);
void	exec_client(t_users *user, t_datas *datas, char *args);
void	exec_whois(t_users *user, t_datas *datas, char *args);
void	exec_addadmin(t_users *user, t_datas *datas, char *args);
void	exec_remadmin(t_users *user, t_datas *datas, char *args);
void	exec_addop(t_users *user, t_datas *datas, char *args);
void	exec_remop(t_users *user, t_datas *datas, char *args);
void	exec_kick(t_users *user, t_datas *datas, char *args);
void	exec_ban(t_users *user, t_datas *datas, char *args);
void	exec_unban(t_users *user, t_datas *datas, char *args);

#endif /* !COMMANDS_H_ */
