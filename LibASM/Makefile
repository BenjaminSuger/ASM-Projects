# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bsuger <bsuger@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/09/20 20:26:53 by bsuger            #+#    #+#              #
#    Updated: 2026/09/20 20:26:57 by bsuger           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= libasm.a
ASFLAGS		= -f elf64 -I $(SRCS_DIR)/
DEBUG_FLAGS	= -g -F dwarf
SRCS_DIR	= srcs
OBJS_DIR	= objs
INCS		= $(SRCS_DIR)/list.inc

SRCS		= $(SRCS_DIR)/ft_strlen.s \
		  $(SRCS_DIR)/ft_write.s \
		  $(SRCS_DIR)/ft_strcmp.s \
		  $(SRCS_DIR)/ft_strcpy.s \
		  $(SRCS_DIR)/ft_strdup.s \
		  $(SRCS_DIR)/ft_read.s 


BONUS_SRCS	= $(SRCS_DIR)/ft_list_size.s \
		  $(SRCS_DIR)/ft_list_push_front_.s \
		  $(SRCS_DIR)/ft_list_sort_.s \
		  $(SRCS_DIR)/ft_list_remove_if_.s \
		  $(SRCS_DIR)/ft_atoi_base.s

OBJS		= $(SRCS:$(SRCS_DIR)/%.s=$(OBJS_DIR)/%.o)
BONUS_OBJS	= $(BONUS_SRCS:$(SRCS_DIR)/%.s=$(OBJS_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.s $(INCS) | $(OBJS_DIR)
	nasm $(ASFLAGS) $< -o $@

$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

bonus: $(OBJS) $(BONUS_OBJS)
	ar rcs $(NAME) $(OBJS) $(BONUS_OBJS)

debug: ASFLAGS += $(DEBUG_FLAGS)
debug: fclean all

clean:
	/bin/rm -rf $(OBJS_DIR)

fclean: clean
	/bin/rm -f $(NAME)

re: fclean all

.PHONY: all bonus debug clean fclean re
