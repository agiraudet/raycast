NAME			:=	pewpew

DEPS			:=	-lraylib

BUILD_DIR	:=	./build

INC_DIR		:=	./inc

SRCS_DIR	:=	./src

SRCS			:=	Anim.cpp \
							Player.cpp \
							Map.cpp \
							Renderer.cpp \
							Sprite.cpp \
							Texture.cpp \
							XpmData.cpp \
							main.cpp

OBJS			:=	$(SRCS:%.cpp=$(BUILD_DIR)/%.o)

INC_FLAGS	:=	$(addprefix -I, $(INC_DIR))

CXXFLAGS	:=	-MD -Wall -Wextra -Werror $(INC_FLAGS) -g

CXX				:=	g++

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(DEPS) -o $@ $(DEPS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/$(EDIT_DIR)

$(BUILD_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

all: $(NAME) $(EDIT_NAME)

clean:
	rm -rf $(BUILD_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(OBJS:.o=.d)

.PHONY: all clean fclean re
