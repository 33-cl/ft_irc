NAME = ircserv

CXX = c++
CFLAGS = -std=c++98 -Wall -Wextra -Werror

SRC = 	srcs/main.cpp \
		srcs/server/process.cpp srcs/server/setup.cpp srcs/server/utils.cpp srcs/server/signals.cpp\
		srcs/client/Client.cpp \
		srcs/channel/Channel.cpp \
		srcs/commands/Invite.cpp srcs/commands/Join.cpp srcs/commands/Kick.cpp \
		srcs/commands/List.cpp srcs/commands/Mode.cpp srcs/commands/Nick.cpp \
		srcs/commands/Part.cpp srcs/commands/Pass.cpp srcs/commands/Privmsg.cpp \
		srcs/commands/Quit.cpp srcs/commands/Topic.cpp srcs/commands/User.cpp \
		srcs/commands/Notice.cpp

OBJ_DIR = .build
OBJ = $(SRC:srcs/%.cpp=$(OBJ_DIR)/%.o)

HEADERS = srcs/messages.hpp srcs/server/Server.hpp srcs/commands/Command.hpp \
		  srcs/client/Client.hpp srcs/channel/Channel.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(CFLAGS) -o $(NAME) $(OBJ)

$(OBJ_DIR)/%.o: srcs/%.cpp $(HEADERS) Makefile | $(OBJ_DIR)
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) timebot

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
# === Bonus : compilation du bot IRC ===

# nom de l’exécutable bonus
BOT_NAME  = timebot

# source & objet du bot
BOT_SRC   = bot/timebot.cpp
BOT_OBJ   = $(OBJ_DIR)/bot/timebot.o

# règle de compilation des .cpp du dossier bots/
$(OBJ_DIR)/bot/%.o: bot/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c $< -o $@

# édition de lien du bot
$(BOT_NAME): $(BOT_OBJ)
	$(CXX) $(CFLAGS) -o $(BOT_NAME) $(BOT_OBJ)

# cible bonus pour faire juste le bot
.PHONY: bonus
bonus: $(BOT_NAME)

# # nettoyage du bot en même temps que fclean
# fclean: clean
# 	rm -f $(NAME) $(BOT_NAME)