CC 		= gcc
CFLAGS 	= -g -Wall

OBJ = main.o dlistnode.o dlist.o dlistiterator.o comparator.o edge.o vertex.o graph.o path.o pathelement.o
NAME = eulerian

all: eulerian

eulerian: $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< $(LDFLAGS)

clean:
	rm $(OBJ)	