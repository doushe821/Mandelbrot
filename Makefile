CC=g++

OUT_O_DIR?=build

OPTIMISATION_FLAGS=-O2 -ffastmath -mavx2

DED32_FLAGS=-ggdb3 -std=c++17 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts \
-Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self \
-Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 \
-Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override \
-Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast \
-Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast \
-Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -pie -fPIE -Werror=vla

SANITIZER_FLAGS=-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr 

LFLAGS=-lSDL2

ifeq ($(COMPILER), clang)
	CC=clang
else ifeq ($(COMPILER), gcc)
	CC=gcc
else
	CC=g++
endif

TARGET=releaseO2

ifeq ($(TARGET), releaseO2)
	CFLAGS=-O2 -mavx2 -flto -ffast-math
else ifeq($(TARGET), releaseO3)
	CFLAGS=-O3 -mavx2 -mavx -flto -ffast-math
else ifeq($(TARGET), debug)
	CFLAGS=-O2 -ffast-math -mavx2 \
	-ggdb3 -std=c++17 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
	-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts \
	-Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
	-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op \
	-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self \
	-Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 \
	-Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override \
	-Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast \
	-Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast \
	-Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer -pie -fPIE -Werror=vla \
	-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr 
endif


SOURCES=main.cpp MandelbrotCalculation.cpp Benchmark.cpp Visualizer.cpp ErrorParser.cpp CMDParser.cpp
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

include $(DEPS)

.PHONY clean:
	rm -rf *.o l