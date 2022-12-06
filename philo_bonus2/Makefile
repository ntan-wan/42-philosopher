NAME = philo_bonus
UNAME = $(shell uname)

ifeq ($(UNAME), Linux)
	PTHREAD_LIB = -lpthread -lrt
endif

CC = gcc
CFLAGS = -Wall -Wextra -Werror
MODE = none
ifeq ($(MODE), debug)
	CFLAGS += -fsanitize=thread
endif


HEADER_DIR =  includes/
SRCS_DIR = srcs/
SRCS = $(shell find srcs/*.c)
OBJS_DIR = objs/
OBJS = $(notdir $(SRCS:.c=.o))
OBJS_PREFIXED = $(addprefix $(OBJS_DIR), $(OBJS))

#text_color
COLOR_OFF =\033[0m
RED =\033[0;31m
GREEN =\033[0;32m
YELLOW =\033[0;33m
CYAN =\033[1;36m
MAGNETA =\033[95m 

all : $(NAME)

$(OBJS_DIR)%.o : $(SRCS_DIR)%.c
	@mkdir -p $(OBJS_DIR)
	@$(CC) -c $< -o $@ -I$(HEADER_DIR)
	@echo "$(GREEN)Compiling : $< $(COLOR_OFF)"

$(NAME) : $(OBJS_PREFIXED)
	@$(CC) $(OBJS_PREFIXED) $(PTHREAD_LIB) -o $(NAME)
	@echo "$(CYAN)$(NAME) done !$(COLOR_OFF)"

bonus : all

clean :
	@rm -rf $(OBJS_DIR)
	@echo "$(RED)Removed : obj files ($(NAME))$(COLOR_OFF)"

fclean : clean
	@rm -rf $(NAME)
	@echo "$(RED)Removed : $(NAME)$(COLOR_OFF)"

re : fclean all

.PHONY : clean fclean all re
