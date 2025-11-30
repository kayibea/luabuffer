CC        = cc
CFLAGS    = -std=c99 -O2 -Wall -Wextra -Werror -fPIC
LDFLAGS   = -shared -llua

OUT = buffer.so
SRC = $(wildcard src/*.c)

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR) $(OUT)

.PHONY: all clean
