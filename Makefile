NAME			=	webserv

CC				=	c++
CFLAGS			=	-Wall -Werror -Wextra -g -std=c++98
DEPSFLAG		=	-MMD

INCLUDE_DIR		=	includes/
INCLUDE_FLAG	=	-I$(INCLUDE_DIR)

FILES_CONF_PARSING		=	$(addprefix conf_parsing/, \
								CgiLocation.class \
								DefineLocationVariableFields \
								DefineServerVariableFields \
								DefineTokenIdentifier \
								HtmlErrorCodes \
								Interpreter \
								Location.class \
								Parser \
								Server.class \
								Tokenizer \
								PrintServer \
							)

FILES_HTTP_EXCHANGES	=	$(addprefix http_exchanges/, \
								$(addprefix request_parsing/, \
									HttpRequest.class \
									HttpRequestHead.class \
									HttpRequestDelete.class \
									HttpRequestGet.class \
									HttpRequestLine.class \
									HttpRequestPost.class \
								) \
								$(addprefix response_generation/, \
									HttpResponse \
									Autoindex.class \
								) \
								HttpExchange.class \
								HttpField.class \
								HttpResponseStatus \
								HttpTools \
							)

FILES_SERVER			=	$(addprefix server/, \
								Cluster.class \
								Socket.class \
							)

FILES_HTTP_SHARED		=	$(addprefix shared/, \
								protected_write \
								utils \
							)

FILES_CGI				=	$(addprefix cgi/, \
								Cgi.class \
							)


FILES_NAMES				=	\
							$(FILES_CONF_PARSING) \
							$(FILES_HTTP_EXCHANGES) \
							$(FILES_SERVER) \
							$(FILES_HTTP_SHARED) \
							$(FILES_CGI) \
							main

SRC_DIR			=	srcs
SRC				=	$(addprefix $(SRC_DIR)/, $(addsuffix .cpp, $(FILES_NAMES)))


OBJ_DIR			=	obj
OBJ				=	$(addprefix $(OBJ_DIR)/, $(FILES_NAMES:=.o))
DEPS			=	$(addprefix $(OBJ_DIR)/, $(FILES_NAMES:=.d))

all : $(NAME)

clean :
	rm -rf $(OBJ_DIR)

fclean : clean
	rm -rf $(NAME)

re : fclean all

run : all
	./$(NAME) $(ARGS)

test : all
	valgrind --show-leak-kinds=all --leak-check=full --track-origins=yes --track-fds=yes -s ./$(NAME) $(ARGS)

-include $(DEPS)

$(NAME) : $(OBJ)
	$(CC) $(CFLAGS) $(DEPSFLAG) $(OBJ) -o $(NAME)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.cpp Makefile
	@mkdir -p $(OBJ_DIR)
	@if [ ! -d "$(dir $@)" ]; then mkdir -p $(dir $@); fi
	$(CC) $(CFLAGS) $(DEPSFLAG) $(INCLUDE_FLAG) -c $< -o $@

.PHONY : all clean fclean re run test