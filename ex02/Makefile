NAME		=	PmergeMe
DIR_SRC		=	./
DIR_BUILD	=	./build
CXX			=	c++
CXXFLAGS	=	-std=c++98 -Wall -Werror -Wextra -g -pedantic 
LDINCLUDE	=	-I$(DIR_SRC)

SRCS		=	main.cpp PmergeMe.cpp Output.cpp
SRCS_NAME	=	$(basename $(SRCS))
OBJS		=	$(addsuffix .o, $(addprefix $(DIR_BUILD)/, $(SRCS_NAME)))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(OBJS) $(CXXFLAGS) -o $@

$(DIR_BUILD)/%.o: $(DIR_SRC)/%.cpp
	@mkdir -p $(dir $@) 
	@$(CXX) $(CXXFLAGS) $(LDINCLUDE) -c $< -o $@

clean:
	@rm -rf $(OBJS)
	@rm -rf $(DIR_BUILD)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re