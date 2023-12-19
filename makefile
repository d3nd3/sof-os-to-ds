CC = i686-w64-mingw32-g++
CFLAGS = -static-libgcc -static-libstdc++
LDFLAGS = -lshlwapi

SRCS = src/main.cpp src/animation_ids.cpp src/movetypes.cpp src/sounds.cpp src/emotions.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = sofos.exe

.PHONY: all clean

all: $(TARGET)
	strip --strip-all $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
