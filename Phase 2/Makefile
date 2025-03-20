CC = g++
CFLAGS = -Wall -Wextra -g
SRC = main.cpp myRISCVSim.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = myRISCVSim

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(EXEC)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) *~ *.bak $(EXEC)
