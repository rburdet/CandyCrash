# Makefile de ejemplo para C++
#
# Creado: jue abr 15 15:34:19 ART 2004
#
# Copyleft 2004 - Leandro Lucarella, Bajo licencia GPL [http://www.gnu.org/]
#

# CONFIGURACION
################

# Nombre del ejecutable.
target = tp

# Extensión de los archivos a compilar (c para C, cpp o cc o cxx para C++).
extension = cpp

# Archivos con el código fuente que componen el ejecutable. Si no se especifica,
# toma todos los archivos con la extensión mencionada. Para especificar hay que
# descomentar la línea (quitarle el '#' del principio).
# NOTA: No poner cabeceras (.h).
#fuentes = entrada.cpp

# Si usa funciones de SDL2, descomenta '#'
#sdl2 = si

# Si usa funciones de SDL2_image, descomenta '#'
#sdl2_image = si

# Si usa funciones de math.h, descomentá (quitale el '#' a) la siguiente línea.
#math = si

# Si usa pthreads, descomentá (quitale el '#' a) la siguiente línea.
pthreads = si

# Si usa gthreads, descomentá (quitale el '#' a) la siguiente línea.
#gthreads = si

# Si es un programa GTK+, descomentá (quitale el '#' a) la siguiente línea.
#gtk = si

# Si es un programa gtkmm, descomentá (quitale el '#' a) la siguiente línea.
gtkmm = si

# Descomentar si se quiere ver como se invoca al compilador
#verbose = no


# CONFIGURACION "AVANZADA"
###########################

# Opciones para el compilador C/C++ en modo ansi.
CFLAGS = #-Wall -pedantic -pedantic-errors -fpermissive -Wlong-long

# Para que explote lo mas posible
CFLAGS += -O3 -DNDEBUG -fpermissive

# Para valgrind o debug
CFLAGS += -ggdb -DDEBUG -fno-inline -fpermissive

# Opciones del enlazador.
LDFLAGS = -L/usr/local/lib -ljsoncpp -lasound

LDFLAGS_SERVER = -L/usr/local/lib -ljsoncpp -lasound -lpthread -pthread

# Estandar de C a usar
CSTD = c99

# Estandar de C++ a usar
CXXSTD = c++0x

# Si se quiere compilar estaticamente, descomentar la siguiente linea
#static = si


# VARIABLES CALCULADAS A PARTIR DE LA CONFIGURACION
####################################################

ifdef sdl2
CFLAGS += $(shell pkg-config --cflags sdl2)
LDFLAGS += $(shell pkg-config --libs sdl2)
endif

ifdef sdl2_image
CFLAGS += $(shell pkg-config --cflags SDL2_image)
LDFLAGS += $(shell pkg-config --libs SDL2_image)
endif

# Agrego flags y libs de GTK+ de ser necesario.
ifdef gtk
CFLAGS += $(shell pkg-config --cflags gtk+-2.0)
LDFLAGS += $(shell pkg-config --libs gtk+-2.0)
endif

# Agrego flags y libs de GTK+ de ser necesario.
ifdef gtkmm
#CFLAGS += $(shell pkg-config --cflags gtkmm-2.4)
CFLAGS += $(shell pkg-config --cflags gtkmm-3.0)
#LDFLAGS += $(shell pkg-config --libs gtkmm-2.4)
LDFLAGS += $(shell pkg-config --libs gtkmm-3.0)
endif

# Agrego flags y libs de gthreads de ser necesario.
ifdef gthreads
CFLAGS += $(shell pkg-config --cflags gthread-2.0)
LDFLAGS += $(shell pkg-config --libs gthread-2.0)
endif

# Linkeo con libm de ser necesario.
ifdef math
LDFLAGS += -lm
endif

# Linkeo con pthreads de ser necesario.
ifdef pthreads
LDFLAGS += -lpthread -pthread
endif

ifdef static
LDFLAGS += -static
endif

# Pongo flags de C para C++ también
CXXFLAGS += $(CFLAGS)

# Uso enlazador de c++ si es código no C.
ifeq ($(extension), c)
CFLAGS += -std=$(CSTD)
LD = $(CC)
else
CXXFLAGS += -std=$(CXXSTD)
LD = $(CXX)
endif

# Si no especifica archivos, tomo todos.
#fuentes ?= $(wildcard *.$(extension))
fuentes_client ?= $(wildcard client*.$(extension))
fuentes_server ?= $(wildcard server*.$(extension))
fuentes_common ?= $(wildcard common*.$(extension))

occ := $(CC)
ocxx := $(CXX)
orm := $(RM)
old := $(LD)
ifdef verbose
RM := $(RM) -v
else
CC =  @echo "  CC  $@"; $(occ)
CXX = @echo "  CXX $@"; $(ocxx)
RM =  @echo "  CLEAN"; $(orm)
LD =  @echo "  LD  $@"; $(old)
endif


# REGLAS
#########

.PHONY: all clean

#all: $(target)
all: client server

o_common_files = $(patsubst %.$(extension),%.o,$(fuentes_common))
o_client_files = $(patsubst %.$(extension),%.o,$(fuentes_client)) $(o_common_files)
o_server_files = $(patsubst %.$(extension),%.o,$(fuentes_server)) $(o_common_files)

#o_files = $(patsubst %.$(extension),%.o,$(fuentes))

-include $(o_files:.o)

#$(target): $(o_files)
#	@if [ -z "$(o_files)" ]; \
#	then \
#		echo "No hay archivos de entrada, recuerde que la extensión debe ser '.$(extension)'."; \
#		false; \
#	fi >&2
#	$(LD) $(LDFLAGS) $(o_files) $(LOADLIBS) $(LDLIBS) -o $(target)

client: $(o_client_files)
	@if [ -z "$(o_client_files)" ]; \
	then \
		echo "No hay archivos de entrada para el cliente (archivos client*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(o_client_files) $(LDFLAGS) -o client

server: $(o_server_files)
	@if [ -z "$(o_server_files)" ]; \
	then \
		echo "No hay archivos de entrada para el servidor (archivos server*.$(extension))."; \
		false; \
	fi >&2
	$(LD) $(o_server_files) $(LDFLAGS_SERVER) -o server

clean:
	#$(RM) $(o_files) $(o_files:.o=.deps) $(target)
	@$(RM) -fv *.o client server

