INCDIRS = -Iinclude
CXX = skm g++ $(INCDIRS)
src = $(wildcard src/*.cpp)
headers = $(wildcard include/*.h)
obj = $(src:.cpp=.o)

LDFLAGS = -std=c++14

ifeq ($(OS),Windows_NT)
    LDFLAGS += -lstdc++fs
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        LDFLAGS += -lstdc++fs
    endif
endif

OSFLAGS = -O3

ArcadeMachine: $(obj)
	$(CXX) -o $@ $^ $(LDFLAGS) $(OSFLAGS)

clean:
	rm $(obj)

format:
	clang-format -i $(src)
	clang-format -i $(headers)
