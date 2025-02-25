NAME = ircserv

CXX = c++
CFLAGS = -std=c++98 -Wall -Wextra -Werror

SRC = src/main.cpp src/Span.cpp
OBJ_DIR = .build
OBJ = $(SRC:src/%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CFLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR)/%.o: src/%.cpp inc/Span.hpp | $(OBJ_DIR)
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re