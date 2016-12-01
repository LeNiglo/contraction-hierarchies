NAME=		server

CXXFLAGS=	-std=c++11 -Wall -Wextra -W #-Werror

SRCS=		main.cc \
			parser.cc \
			graph.cc \
			calclatlong/haversine.cc

OBJS=		$(SRCS:.cc=.o)

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
			$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

.PHONY: all clean fclean re
