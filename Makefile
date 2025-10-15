CC        = cc
INC   		= -Iinclude -Iextern/hexlib
CFLAGS    = -std=c99 -O2 -Wall -Werror -fPIC $(INC)
LDFLAGS   = -shared -llua

TARGET    = buffer
SRC_DIR   = src
BUILD_DIR = build
EXT_DIR   = extern

EXT_SRC   = $(wildcard $(EXT_DIR)/*/*.c)
EXT_OBJ   = $(patsubst $(EXT_DIR)/%.c, $(BUILD_DIR)/%.o, $(EXT_SRC))

SRC       = $(wildcard $(SRC_DIR)/*.c)
OBJ       = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))
OUT       = $(TARGET).so

all: $(OUT)

$(OUT): $(OBJ) $(EXT_OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(EXT_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(OUT)

.PHONY: all clean
