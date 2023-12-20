CC = i686-w64-mingw32-g++
CFLAGS = -static-libgcc -static-libstdc++
LDFLAGS = -lshlwapi

SRCS = src/main.cpp src/animation_ids.cpp src/movetypes.cpp src/sounds.cpp src/emotions.cpp
OBJS = $(SRCS:.cpp=.o)

TARGET = sofos.exe
RELEASE_FOLDER = sofos_release
VERSION_FILE = version.txt

.PHONY: all clean release

all: $(TARGET)
	strip --strip-all $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

release: $(TARGET)
	mkdir -p $(RELEASE_FOLDER)
	cp $(TARGET) $(RELEASE_FOLDER)/$(TARGET)
	@echo "$$(echo "$$(cat $(VERSION_FILE)) + 0.1" | bc -l)" > $(VERSION_FILE)
	zip -rq $(RELEASE_FOLDER)_v$$(cat $(VERSION_FILE)).zip $(RELEASE_FOLDER)