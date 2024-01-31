NAME = philo
FLAGS = -Wall -Wextra -Werror
CC = gcc
SRCS = main.c utils.c time_utils.c state_utils.c philo_factory.c critical_section_utils.c abort.c thread/philosopher_thread.c thread/watchdog_thread.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
