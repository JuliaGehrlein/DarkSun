CC = g++
CXXFLAGS = -Wall -pipe -std=c++11
LINKS = -lsquids -lnusquids
OBJ = sun_test.o

# ****************************************************
# Targets needed to bring the executable up to date

# The main.o target can be written more simply
%.o : %.cpp
	$(CC) $(CXXFLAGS) -c $<

main: $(OBJ)
	$(CC) $(CXXFLAGS) $(INCLUDES) $(OBJ) -o a $(LINKS)

clean:
	rm -rf *.o
