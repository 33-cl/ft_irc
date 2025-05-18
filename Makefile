NAME = ircserv

CXX = c++
CFLAGS = -std=c++98 -Wall -Wextra -Werror
BONUSFLAG = -DBONUS

SRC =  srcs/main.cpp srcs/server/process.cpp srcs/server/setup.cpp \
srcs/server/utils.cpp srcs/server/signals.cpp srcs/client/Client.cpp \
srcs/channel/Channel.cpp srcs/commands/Invite.cpp srcs/commands/Join.cpp \
srcs/commands/Kick.cpp srcs/commands/List.cpp srcs/commands/Mode.cpp \
srcs/commands/Nick.cpp srcs/commands/Part.cpp srcs/commands/Pass.cpp \
srcs/commands/Privmsg.cpp srcs/commands/Quit.cpp srcs/commands/Topic.cpp \
srcs/commands/User.cpp srcs/commands/Notice.cpp

OBJ_DIR = .build
OBJ = $(SRC:srcs/%.cpp=$(OBJ_DIR)/%.o)

HEADERS = srcs/messages.hpp srcs/server/Server.hpp srcs/commands/Command.hpp \
srcs/client/Client.hpp srcs/channel/Channel.hpp

.PHONY: all clean fclean re bonus

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CFLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR)/%.o: srcs/%.cpp $(HEADERS) | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -f .bonus-stamp

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: .bonus-stamp

.bonus-stamp: srcs/commands/Privmsg.cpp
	@rm -f $(OBJ_DIR)/commands/Privmsg.o
	@$(MAKE) CFLAGS="$(CFLAGS) $(BONUSFLAG)" all
	@touch $@