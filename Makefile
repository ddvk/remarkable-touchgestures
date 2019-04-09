LDFLAGS=-lm
CFLAGS=-Wall -std=c99 
BUILDDIR=bin
TARGET=$(BUILDDIR)/touchinjector
SOURCEDIR=.
SOURCES := $(shell find $(SOURCEDIR) -name '*.c')
OBJECTS := $(addprefix $(BUILDDIR)/,$(SOURCES:%.c=%.o))

all: $(TARGET)

$(TARGET): $(OBJECTS) 
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(BUILDDIR)/*
