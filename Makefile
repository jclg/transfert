##
## Makefile :D
## 


CC_FreeBSD	= cc

CC_linux	= cc

CC_ia64-linux	= cc

CC_sun4		= /usr/sfw/bin/gcc

CC_		= cc



NAME		= transfert

SRC		= main.c\
		  network.c\
		  sender.c\
		  receiver.c\
		  md5.c

OBJ		= $(SRC:.c=.o)


CFLAGS		= -W -Wall -Wstrict-prototypes -pedantic

LIB		= -lcrypto

RM		= rm -f


all	: $(NAME)

re	: fclean $(NAME)

$(NAME)	:	$(OBJ)
	$(CC_$(HOSTTYPE)) -o $(NAME) $(OBJ)  $(LIB)

clean	:
	$(RM) $(OBJ)
	$(RM) *~ \#*

fclean	: clean
	$(RM) $(NAME)
love	:
	@echo ". . . not war"
