
CFLAGS := -Wall -Wextra -std=c11
SRCS := $(wildcard *.c)
HDRS := $(wildcard *.h)
OBJS := ${SRCS:.c=.o}


all : $(OBJS) $(HDRS)
	gcc $(CFLAGS) $(OBJS) -o test.exe

%.o : %.c $(HDRS)
	gcc $(CFLAGS) -c $< -o $@

clean :
	@- rm test.exe
	@- rm $(OBJS)
	
