CXX      = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -Iincludes
NAME     = PmergeMe

SRC_DIR  = src
OBJ_DIR  = obj
SRCS = main.cpp PmergeMe.cpp PmergeMeVector.cpp PmergeMeDeque.cpp PmergeMeUtils.cpp

OBJS     = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp includes/PmergeMe.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
