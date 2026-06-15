CC := gcc

TARGET := main
SRCS := main.c render.c update.c input.c canvas.c widgets.c app.c
OBJS := $(SRCS:.c=.o)

BASELIBS := -lmingw32 -lSDL3 -lSDL3_image -lSDL3_ttf -lm
STATLIBS := -static -lsetupapi -lwinmm -lgdi32 -limm32 -lole32 -loleaut32 -lversion -luuid

CFLAGS :=
LIBS := $(BASELIBS)

# Optional static linking
LIBS += $(if $(filter true,$(STATIC)),$(STATLIBS))

# Hide console unless debugging
LIBS += $(if $(filter true,$(DEBUG)),,-mwindows)

# Debug symbols
CFLAGS += $(if $(filter true,$(DEBUG)),-g)

# Default build
all: $(TARGET)

# Static build
static:
	@$(MAKE) --no-print-directory STATIC=true

# Debug build
debug:
	@$(MAKE) --no-print-directory DEBUG=true

# Static + Debug build
staticdebug:
	@$(MAKE) --no-print-directory STATIC=true DEBUG=true

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q $(OBJS) $(TARGET).exe 2>nul