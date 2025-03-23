NAME = ircserv

CXX = c++
CFLAGS = -std=c++98 -Wall -Wextra -Werror

SRC = 	$(wildcard srcs/*.cpp) \
		$(wildcard srcs/server/*.cpp) \
		$(wildcard srcs/client/*.cpp) \
		$(wildcard srcs/channel/*.cpp) \
		$(wildcard srcs/commands/*.cpp)

OBJ_DIR = .build
OBJ = $(SRC:srcs/%.cpp=$(OBJ_DIR)/%.o)

HEADERS = srcs/server/Server.hpp srcs/client/Client.hpp srcs/channel/Channel.hpp srcs/commands/Commands.hpp message.hpp;

all: $(NAME)

dev: CFLAGS = -std=c++98
dev: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CFLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR)/%.o: srcs/%.cpp | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re dev