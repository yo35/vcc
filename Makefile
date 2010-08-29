
# Commandes
CXX = g++
LD  = g++

# Options de compilation
CXXFLAGS  = -pedantic -Wall -Wextra -O2 -g `pkg-config --cflags gtkmm-2.4`
LDFLAGS   = `pkg-config --libs gtkmm-2.4`

# Fichiers à générer
EXE  = clock
SRC  = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp,%.o,$(SRC))
DEPS = $(patsubst %.cpp,%.dep,$(SRC))

all: $(EXE)

run: $(EXE)
	./$(EXE)

# Construction de l'exécutable à partir des .o
$(EXE): $(OBJS)
	@echo "\033[34;1mLinking du fichier $(notdir $@)...\033[0m"
	@$(LD) $^ -o $@ $(LDFLAGS)

# Construction des .o a partir des .cpp, et du fichier .dep correspondant
%.o: %.cpp %.dep
	@echo "\033[34;1mCompilation du fichier $(notdir $<)...\033[0m"
	@$(CXX) $(CXXFLAGS) -c -o $@ $<

# Construction des dépendances de chaque .cpp et de l'arborescence
%.dep: %.cpp
	@echo "\033[34;1mGénération des dépendances du fichier $(notdir $<)...\033[0m"
	@$(CXX) $(CXXFLAGS) -MM -MF $@ $<

# Cibles PHONY
.PHONY: clean run

# Nettoyage
clean:
	rm -f *.o *.dep
	rm -f $(EXE)

# Dépendances générées dynamiquement
ifneq ($(MAKECMDGOALS), clean)
-include $(DEPS)
endif
