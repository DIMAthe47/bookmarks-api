CC = c++
CFLAGS = -std=c++1y -fPIC
DEPS = $(shell pkg-config --cflags --libs libmongocxx)

all: ./BookmarksHandler.so ./Csimple.so

./Csimple.so:
	$(CC) $(CFLAGS) ./src/SimpleClass.cpp ./src/hello_mongo.cpp -shared -o libcsimple.so -lfastcgi-daemon2 $(DEPS)

./BookmarksHandler.so:
	$(CC) $(CFLAGS) ./src/BookmarksHandler.cpp ./src/DAO.cpp -shared -o libbookmarksHandler.so -lfastcgi-daemon2 $(DEPS)

clean:
	rm -rf *.so *.o
