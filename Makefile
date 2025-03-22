# NAME = ircserv

# CXX = c++
# CFLAGS = -std=c++98 -Wall -Wextra -Werror

# SRC =	srcs/main.cpp \
# 		srcs/server/Server.cpp srcs/server/setup.cpp srcs/server/utils.cpp \
# 		srcs/client/init.cpp srcs/client/accessors.cpp srcs/client/login.cpp srcs/client/join.cpp srcs/client/utils.cpp \
# 		srcs/channel/init.cpp srcs/channel/utils.cpp \
# 		srcs/commands/Join.cpp
# OBJ_DIR = .build
# OBJ = $(SRC:srcs/%.cpp=$(OBJ_DIR)/%.o)
# HEADERS = srcs/server/Server.hpp srcs/client/Client.hpp srcs/channel/Channel.hpp srcs/commands/Commands.hpp

# all: $(NAME)

# dev: CFLAGS = -std=c++98
# dev: $(NAME)

# $(NAME): $(OBJ)
# 	$(CXX) $(CFLAGS) -o $(NAME) $(OBJ)

# $(OBJ_DIR)/%.o: srcs/%.cpp $(HEADERS) | $(OBJ_DIR)
# 	@mkdir -p $(@D)  
# 	$(CXX) $(CFLAGS) -c $< -o $@

# $(OBJ_DIR):
# 	mkdir -p $(OBJ_DIR)

# clean:
# 	rm -rf $(OBJ_DIR)

# fclean: clean
# 	rm -f $(NAME)

# re: fclean all

# .PHONY: all clean fclean re dev

NAME = ircserv

CXX = c++
CFLAGS = -std=c++98 -Wall -Wextra -Werror

SRC =	srcs/main.cpp \
		srcs/server/Server.cpp srcs/server/setup.cpp srcs/server/utils.cpp \
		srcs/client/init.cpp srcs/client/accessors.cpp srcs/client/login.cpp srcs/client/join.cpp srcs/client/utils.cpp \
		srcs/channel/init.cpp srcs/channel/utils.cpp \
		srcs/commands/Join.cpp srcs/commands/Command.cpp;

OBJ_DIR = .build
OBJ = $(SRC:srcs/%.cpp=$(OBJ_DIR)/%.o)

HEADERS = srcs/server/Server.hpp srcs/client/Client.hpp srcs/channel/Channel.hpp srcs/commands/Commands.hpp

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