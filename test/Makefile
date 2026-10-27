NAME	= test_ft_printf
LIB_DIR	= ..
LIB		= $(LIB_DIR)/libftprintf.a
SRCS	= test_ft_printf.c
CC		= cc
CFLAGS	= -Wall -Wextra -Werror -Wno-format
INCS	= -I $(LIB_DIR)/include

.PHONY: all run big lib clean fclean re
all: $(NAME)

$(NAME): $(SRCS) lib
	$(CC) $(CFLAGS) $(INCS) $(SRCS) $(LIB) -o $(NAME)

lib:
	$(MAKE) -C $(LIB_DIR)

run: all
	./$(NAME)

big: all
	./$(NAME) --big

clean:
	$(MAKE) -C $(LIB_DIR) clean

fclean:
	rm -f $(NAME)
	$(MAKE) -C $(LIB_DIR) fclean

re: fclean all
