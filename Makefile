NAME	= libftprintf.a
SRCS	= 
OBJS	= $(SRCS:.c=.o)
CC		= cc
CFLAGS	= -Wall -Wextra -Werror
INCS	= -I.

.PHONY: all clean fclean re
all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c ft_printf.h
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all