NAME := ircserv

# tools
CC := c++ -g
CFLAGS := -Wall -Wextra -Werror -std=c++98  -I./inc/
COBJFLAGS := $(CFLAGS) -c

#paths
OBJ_PATH := obj
SRC_PATH := src

# src files and obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# clean files list
CLEAN_LIST := $(OBJ)
FULL_CLEAN_LIST := $(CLEAN_LIST) $(OBJ_PATH) $(NAME)

# default rule
default: makedir $(NAME)

# non-phony targets
$(NAME): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(COBJFLAGS) -o $@ $<

#phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(OBJ_PATH)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -rf $(CLEAN_LIST)

.PHONY: fclean
fclean:
	@echo CLEAN $(FULL_CLEAN_LIST)
	@rm -rf $(FULL_CLEAN_LIST)

.PHONY: re
re: fclean default
