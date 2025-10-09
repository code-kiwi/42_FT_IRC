# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/20 12:41:57 by mhotting          #+#    #+#              #
#    Updated: 2025/10/08 17:27:12 by mhotting         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#################################################
# GENERAL

NAME					=	ircserv
CC						=	c++
CFLAGS					=	-Wall -Werror -Wextra -std=c++98
DEPFLAGS				=	-MP -MMD -MF
HFLAGS					=	$(addprefix -I, $(HEADERS_DIRS))

# Enables debig flag if DEBUG=1 is passed to make
ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0 -DDEBUG
	BUILD_MODE = DEBUG
else
	CFLAGS += -O2
	BUILD_MODE = RELEASE
endif

# HEADERS
HEADERS_DIRS			=	incs/ incs/commands/

# SERVER
SERVER_DIR				=	server/
SERVER_FILES			=	ServerChannel.cpp	\
							ServerClient.cpp	\
							ServerGeneral.cpp	\
							ServerMessages.cpp	\
							ServerNetwork.cpp
SERVER					=	$(addprefix $(SERVER_DIR), $(SERVER_FILES))

# COMMANDS
COMMANDS_DIR			=	commands/
COMMANDS_FILES			=	CommandFactory.cpp	\
							Command.cpp			\
							ErrorCommand.cpp	\
							PassCommand.cpp		\
							NickCommand.cpp		\
							UserCommand.cpp		\
							CapCommand.cpp		\
							JoinCommand.cpp		\
							PartCommand.cpp		\
							QuitCommand.cpp		\
							ModeCommand.cpp		\
							InviteCommand.cpp	\
							KickCommand.cpp		\
							PingCommand.cpp		\
							PrivMsgCommand.cpp	\
							TopicCommand.cpp	\
							WhoCommand.cpp		\
							NamesCommand.cpp	\
							ListCommand.cpp
COMMANDS				=	$(addprefix $(COMMANDS_DIR), $(COMMANDS_FILES))

# SOURCES GENERAL
SRCS_MAIN_DIR			=	srcs/
SRCS_FILES				=	$(SERVER)			\
							$(COMMANDS)			\
							main.cpp			\
							helpers.cpp			\
							Client.cpp			\
							Channel.cpp
SRCS					=	$(addprefix $(SRCS_MAIN_DIR), $(SRCS_FILES))

# OBJECTS GENERAL
OBJS_MAIN_DIR			=	.objs/
OBJS_FILES				=	$(SRCS_FILES:.cpp=.o)
OBJS					=	$(addprefix $(OBJS_MAIN_DIR), $(OBJS_FILES))

# DEPENDENCIES
DEPS_MAIN_DIR			=	.deps/
DEPS_FILES				=	$(OBJS_FILES:.o=.d)
DEPS					=	$(addprefix $(DEPS_MAIN_DIR), $(DEPS_FILES))

#################################################
# RULES
all: $(NAME)
	@echo "Build finished in $(BUILD_MODE) mode"

$(NAME): $(OBJS) 
	$(CC) $(OBJS) -o $@

$(OBJS_MAIN_DIR)%.o: $(SRCS_MAIN_DIR)%.cpp
	@mkdir -p $(@D)
	@mkdir -p $(DEPS_MAIN_DIR)$(dir $*)
	$(CC) $(CFLAGS) $(HFLAGS) $(DEPFLAGS) $(DEPS_MAIN_DIR)$*.d -c $< -o $@ 

-include $(DEPS)

debug:
	$(MAKE) DEBUG=1 re

clean:
	rm -rf $(OBJS_MAIN_DIR)
	rm -rf $(DEPS_MAIN_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re debug
