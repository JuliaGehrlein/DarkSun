CC = g++
CXXFLAGS = -Wall -pipe -std=c++11
INCLUDES = -I/usr/include/hdf5/serial #Change this if your hdf5.h is located somewhere else!
LINKS = -lSQuIDS -lnuSQuIDS -lgsl -lcblas -lhdf5 -lhdf5_hl
OBJ = sun_test.o

# ****************************************************
# Targets needed to bring the executable up to date

# The main.o target can be written more simply
%.o : %.cpp
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $<

main: $(OBJ)
	$(CC) $(CXXFLAGS) $(OBJ) -o a $(LINKS)

clean:
	rm -rf *.o
