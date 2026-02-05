CC = gcc

all: oss user

oss: oss.c
	gcc -std=c99 -o oss oss.c

user: user.c
	gcc -std=c99 -o user user.c

clean:
	$(RM) user oss