CC=g++

OUT_O_DIR?= build

CFLAGS= -O2 -std=c++14 -mavx2 \
-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr \
-ggdb3 -std=c++17 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -pie -fPIE -Werror=vla \


CFLAGS_RELEASE=-D _NDEBUG
LFLAGS= -lSDL2 

SOURCES=main.cpp MandelbrotCalculation.cpp UnitTest.cpp Visualizer.cpp ErrorParser.cpp CMDParser.cpp
OBJECTS:=$(addprefix $(OUT_O_DIR)/,$(SOURCES:.cpp=.o))
DEPS=$(OBJECTS:.o=.d)

EXECUTABLE=Mandelbrot.exe


.PHONY: clean
.PHONY: all

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	@$(CC) $(LDFLAGS) $(CFLAGS) $(LFLAGS) $(OBJECTS) -o $@

$(OBJECTS) : $(OUT_O_DIR)/%.o : %.cpp
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(LFLAGS) -c $< -o $@

$(DEPS) : $(OUT_O_DIR)/%.d : %.cpp
	@mkdir -p $(@D)
	@$(CC) -E $(CFLAGS) $< -MM -MT $(@:.d=.o) > $@

.PHONY clean:
	rm -rf *.o l