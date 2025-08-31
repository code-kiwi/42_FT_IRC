# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mhotting <mhotting@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/20 12:41:57 by mhotting          #+#    #+#              #
#    Updated: 2025/08/31 16:18:44 by mhotting         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#################################################
# GENERAL

NAME					=	ircserv

CC						=	c++
CFLAGS					=	-Wall -Werror -Wextra -std=c++98
DEPFLAGS				=	-MP -MMD -MF
HFLAGS					=	-I$(HEADERS_DIR)

# Enables debig flag if DEBUG=1 is passed to make
ifeq ($(DEBUG), 1)
	CFLAGS += -g -O0 -DDEBUG
	BUILD_MODE = DEBUG
else
	CFLAGS += -O2
	BUILD_MODE = RELEASE
endif

# HEADERS
HEADERS_DIR				=	incs/

# SOURCES GENERAL
SRCS_MAIN_DIR			=	srcs/
SRCS_FILES				=	main.cpp
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

clean:
	rm -rf $(OBJS_MAIN_DIR)
	rm -rf $(DEPS_MAIN_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
