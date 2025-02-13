CC := c++

FLAGS := -std=c++98 -g -Wall -Wextra -Werror
NAME := ircserv
SRCS :=  src/core/ft_irc.cpp \
	src/core/Server.cpp \
	src/core/ServerConfig.cpp \
	src/core/ServerCommand/ServerCommand.cpp \
	src/core/ServerCommand/_pass.cpp \
	src/core/ServerCommand/_nick.cpp \
	src/core/ServerCommand/_user.cpp \
	src/core/ServerCommand/_invite.cpp \
	src/core/ServerCommand/_join.cpp \
	src/core/ServerCommand/_kick.cpp \
	src/core/ServerCommand/_topic.cpp \
	src/core/ServerCommand/_privmsg.cpp \
	src/core/ServerCommand/_mode.cpp \
	src/core/ServerCommand/_part.cpp \
	src/core/ServerCommand/_msg.cpp \
	src/core/ServerCommand/_quit.cpp \
	src/core/ServerCommand/_squit.cpp \
	src/core/ServerCommand/_ping.cpp \
	src/core/ServerCommand/_pong.cpp \
	src/core/ServerCommand/_info_me.cpp \
	src/core/ServerCommand/_info_mods.cpp \
	src/core/ServerCommand/_file_transfert.cpp \
	src/core/ServerCommand/_bot.cpp \
	src/core/Log.cpp \
	src/core/Client.cpp \
	src/core/Channel/Channel.cpp \
	src/core/Channel/_join.cpp \
	src/core/Channel/_invite.cpp \
	src/core/Channel/_kick.cpp \
	src/core/Channel/_topic.cpp \
	src/core/Channel/_privmsg.cpp \
	src/core/Channel/_mode.cpp \
	src/core/Channel/_part.cpp \
	src/core/Channel/_bot.cpp \
	src/core/Utils.cpp \
	src/core/Bot.cpp
OBJS := $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) -o $(NAME) $(OBJS)

%.o: %.cpp
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)
	@rm -rf logs/*.log

re: fclean all

.PHONY: all clean fclean re
