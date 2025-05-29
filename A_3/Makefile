# liadi.haim@gmail.com
CXX       = g++
CXXFLAGS  = -std=c++17 -Wall -Wextra
LDFLAGS   = -lsfml-graphics -lsfml-window -lsfml-system
VALGRIND  = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

SRC       = main.cpp Player.cpp Game.cpp GameGUI.cpp \
            Governor.cpp Spy.cpp Baron.cpp General.cpp Judge.cpp Merchant.cpp

OBJ       = $(SRC:.cpp=.o)

TEST_SRC  = test.cpp Player.cpp Game.cpp GameGUI.cpp \
            Governor.cpp Spy.cpp Baron.cpp General.cpp Judge.cpp Merchant.cpp

all: Main

Main: $(OBJ)
	$(CXX) $(CXXFLAGS) -o Main $^ $(LDFLAGS)

test: $(TEST_SRC)
	$(CXX) $(CXXFLAGS) -o test $^ $(LDFLAGS)

valgrind: test
	$(VALGRIND) ./test

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f Main test $(OBJ)
