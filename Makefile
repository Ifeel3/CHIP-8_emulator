NAME	= CHIP8Emu
SRC		= src/Chip8.cpp \
		src/Emuwindow.cpp \
		src/main.cpp
INCLUDES= includes/
CXX		= clang++
CXXFLAGS= -std=c++98 -Wall -Werror -Wextra
OBJ		= $(SRC:src/%.cpp=obj/%.o)

.PHONY: all re clean flclean

obj/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDES) -c $? -o $@

all: $(NAME)

obj:
	@mkdir obj

$(NAME): obj $(OBJ)
	$(CXX) $(OBJ) -lX11 -o $@

clean:
	@rm -rf obj

fclean: clean
	@rm -f $(NAME)

re: fclean all