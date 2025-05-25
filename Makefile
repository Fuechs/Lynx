_mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
I := $(patsubst %/,%,$(dir $(_mkfile_path)))

ifneq ($(words $(MAKECMDGOALS)),1)
.DEFAULT_GOAL = all
%:
	@$(MAKE) $@ --no-print-directory -rRf $(firstword $(MAKEFILE_LIST))
else
ifndef ECHO
T := $(shell $(MAKE) $(MAKECMDGOALS) --no-print-directory \
    -nrRf $(firstword $(MAKEFILE_LIST)) \
    ECHO="COUNTTHIS" | grep -c "COUNTTHIS")
N := x
C = $(words $N)$(eval N := x $N)
ECHO = python3.11 $(I)/src/util/compiler_progress.py --stepno=$C --nsteps=$T
endif

src = $(wildcard src/*.cpp src/*/*.cpp)
obj = $(src:.cpp=.o)
hdr = $(wildcard src/*.h src/*/*.h)
out = lynx
cflags = -g -O3 -std=c++20
ldflags = -g 
llvmflags = `llvm-config --cxxflags --ldflags --libs core`

all: $(src) $(obj) $(out)

$(out): $(obj)
	@$(ECHO) Linking $@
#	@clang++ $(llvmflags) $(ldflags) $^ -o $@
	@clang++ $(ldflags) $^ -o $@

%.o: %.cpp $(hdr)
	@$(ECHO) Compiling $<
#	@clang++ `llvm-config --cxxflags` $(cflags) -c $< -o $@
	@clang++ $(cflags) -c $< -o $@

clean: 
	@rm $(obj)
	@echo Done.

endif