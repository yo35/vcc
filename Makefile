
# Commandes
CXX = g++
LD  = g++

# Options de compilation
CXXFLAGS  = -pedantic -Wall -Wextra -O2 -g `pkg-config --cflags gtkmm-2.4`
LDFLAGS   = `pkg-config --libs gtkmm-2.4`

# Fichiers a generer
EXE  = clock.exe
SRC  = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRC))
DEPS = $(patsubst %.cpp,%.dep,$(SRC))

all: $(EXE)

run: $(EXE)
	./$(EXE)

# Construction de l'executable a partir des .o
$(EXE): $(OBJS)
	@echo "\033[34;1mLinking du fichier $(notdir $@)...\033[0m"
	@$(LD) $^ -o $@ $(LDFLAGS)

# Construction des .o a partir des .cpp, et du fichier .dep correspondant
%.o: %.cpp %.dep
	@echo "\033[34;1mCompilation du fichier $(notdir $<)...\033[0m"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

# Construction des dependances de chaque .cpp et de l'arborescence
%.dep: %.cpp
	@echo "\033[34;1mGeneration des dependances du fichier $(notdir $<)...\033[0m"
	@$(CXX) $(CXXFLAGS) -MM -MF $@ $<

# Cibles PHONY
.PHONY: clean run

# Nettoyage
clean:
	rm -f *.o *.dep
	rm -f $(EXE)

# Dependances generees dynamiquement
ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)
endif
