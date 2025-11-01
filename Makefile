MAKE_CC = musl-gcc
MAKE_CFLAGS = -Os -s -ffunction-sections -fdata-sections -Iinclude -flto \
              -march=native -mtune=native \
              -fno-guess-branch-probability -fno-stack-protector

MAKE_LDFLAGS = -Wl,--gc-sections
MAKE_TARGET = dist/main
MAKE_SRC = src/main.c src/util.c

.PHONY: all clean

$(MAKE_TARGET): $(MAKE_SRC)
	$(MAKE_CC) $(MAKE_CFLAGS) $(MAKE_SRC) -o $@ $(MAKE_LDFLAGS)
	strip $@

clean:
	rm -f $(MAKE_TARGET)
