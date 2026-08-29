NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = src/main.c \
       src/parsing.c \
       src/init.c \
       src/simulation.c \
       src/coder.c \
       src/dongle.c \
       src/scheduler.c \
       src/heap.c \
       src/monitor.c \
       src/time.c \
       src/logging.c \
       src/cleanup.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re