##
## Makefile for make in /home/alexmog/testwebsocket/c_version
## 
## Made by Moghrabi Alexandre
## Login   <alexmog@epitech.net>
## 
## Started on  Wed May  7 22:18:29 2014 Moghrabi Alexandre
## Last update Fri Jul 18 16:28:03 2014 Moghrabi Alexandre
##

BINDIR=	bin/

NAME=	$(BINDIR)server

SRCDIR=	src/

SRC=	$(SRCDIR)server.c		\
	$(SRCDIR)server_interpretator.c	\
	$(SRCDIR)commands/login.c	\
	$(SRCDIR)commands/list.c	\
	$(SRCDIR)commands/msg.c		\
	$(SRCDIR)commands/privmsg.c	\
	$(SRCDIR)commands/client.c	\
	$(SRCDIR)commands/whois.c	\
	$(SRCDIR)commands/addadmin.c	\
	$(SRCDIR)commands/remadmin.c	\
	$(SRCDIR)commands/addop.c	\
	$(SRCDIR)commands/remop.c	\
	$(SRCDIR)commands/kick.c	\
	$(SRCDIR)commands/ban.c		\
	$(SRCDIR)commands/unban.c	\
	$(SRCDIR)web_reader.c		\
	$(SRCDIR)utils.c		\
	$(SRCDIR)list.c			\
	$(SRCDIR)init.c			\
	$(SRCDIR)configs_reader.c

CFLAGS=	-W -Wall -Werror -Iinclude

LFLAGS=	-lwebsock -lcurl -lpthread

OBJ=	$(SRC:.c=.o)

CC=	gcc

RM=	rm -f

all:		$(NAME)

debug:		CFLAGS += -g3
debug:		all

$(NAME):	$(OBJ)
		$(CC) -o $(NAME) $(OBJ) $(LFLAGS)

clean:
		$(RM) $(OBJ)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all
