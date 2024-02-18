CXX = gcc
CXX_FLAGS = -lulfius -Wall -Wextra -O3 -g -DOMPI_SKIP_MPICXX -ljansson -lpthread  -lcurl

all: api

api: api.c 
	$(CXX)  -o api api.c $(CXX_FLAGS)




clean:
	rm -f denem denem.env *.o