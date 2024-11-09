BINARY = exec
BUILDIR = build
SRCDIR = src

CXX = g++
SDLFLAGS = -lSDL2
DEPFLAGS = -MP -MD
CFLAGS = -Wall -Wextra $(SDLFLAGS) $(DEPFLAGS)
CPPFILES = $(wildcard $(SRCDIR)/*.cpp)
OBJFILES	= $(patsubst %.cpp, %.o, $(patsubst $(SRCDIR)/%, $(BUILDIR)/%, $(CPPFILES)))
DEPFILES = $(patsubst %.cpp, %.d, $(patsubst $(SRCDIR)/%, $(BUILDIR)/%, $(CPPFILES)))

all: build $(BINARY) run
$(BINARY): $(build) $(OBJFILES)
	$(CXX) -o $@ $^ $(SDLFLAGS)

build:
	mkdir -p build

$(BUILDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf $(BINARY) $(OBJFILES) $(DEPFILES)

run:
	./$(BINARY)

-include $(DEPFILES)
