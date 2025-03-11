NAME = ircserv

CXX = c++
CFLAGS = -std=c++98 -Wall -Wextra -Werror

SRC = srcs/main.cpp srcs/Server.cpp srcs/Client.cpp srcs/Channel.cpp
OBJ_DIR = .build
OBJ = $(SRC:srcs/%.cpp=$(OBJ_DIR)/%.o)
HEADERS = includes/Server.hpp includes/Client.hpp includes/Channel.hpp

all: $(NAME)

dev: CFLAGS = -std=c++98
dev: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CFLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR)/%.o: srcs/%.cpp $(HEADERS) | $(OBJ_DIR)
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re dev
