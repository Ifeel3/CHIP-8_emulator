NAME	= CHIP8Emu
SRC		= src/Chip8.cpp \
		src/Emuwindow.cpp \
		src/main.cpp
INCLUDES= includes/
CXX		= clang++
CXXFLAGS= -std=c++98 -Wall -Werror -Wextra
OBJ		= $(SRC:%.cpp=%.o)

.PHONY: all re clean flclean

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDES) -c $? -o $@

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -lX11 -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all