CC = gcc
FLAGS = -mavx -mavx2

SOURCEDIR = /src/code
BUILDDIR = bin

EXECUTABLE = galois
SOURCEDIR = src
SOURCES = $(wildcard $(SOURCEDIR)/*.c)
OBJECTS = $(patsubst $(SOURCEDIR)/*.c,$(BUILDIR)/%.o,$(SOURCES))

all: dir $(BUILDDIR)/$(EXECUTABLE)

dir:
	mkdir -p $(BUILDDIR)

$(BUILDDIR)/$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ -o $@

# $(OBJECTS): $(BUILDDIR)/%.o : $(SOURCEDIR)/%.c
# 	$(CC) $(FLAGS) $< -o $@

clean:
	rm -rf $(BUILDDIR)
run:
	./$(BUILDDIR)/$(EXECUTABLE)