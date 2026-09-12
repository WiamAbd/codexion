NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread

SRCS = src/main.c \
       src/parsing.c \
       src/init.c \
       src/simulation.c \
       src/simulation_threads.c \
       src/coder.c \
       src/actions.c \
       src/dongle.c \
       src/heap.c \
       src/heap_life.c \
       src/monitor.c \
       src/time.c \
       src/logging.c \
       src/cleanup.c \
       src/heap_utils.c

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