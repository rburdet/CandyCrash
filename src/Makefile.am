# this is example-file: src/bin/Makefile.am

# additional include pathes necessary to compile the C++ programs
AM_CXXFLAGS = $(JSONCPP_CFLAGS) -std=c++0x @AM_CXXFLAGS@
AM_LDFLAGS = $(JSONCPP_LIBS) -std=c++0x @AM_LDFLAGS@

###############################################################################
# THE PROGRAMS TO BUILD
###############################################################################

# the program to build (the names of the final binaries)
bin_PROGRAMS = server cliente

# list of sources for the 'helloWorld' binary
server_SOURCES = \
	common.hmac.cpp \
	common.logger.cpp \
	common.md5.cpp \
	common.mutex.cpp \
	common.path_manager.cpp \
	common.socket.cpp \
	common.socket_io.cpp \
	common.thread.cpp \
	common.thread_socket.cpp \
	common.user_manager.cpp \
	server.partida.cpp \
	server.server.cpp \
	server.socket_listener.cpp \
	server.tablero.cpp \
	server.thread_usuario.cpp \
	server.cpp

cliente_SOURCES = \
	common.hmac.cpp \
	common.logger.cpp \
	common.md5.cpp \
	common.mutex.cpp \
	common.path_manager.cpp \
	common.socket.cpp \
	common.socket_io.cpp \
	common.thread.cpp \
	common.thread_socket.cpp \
	common.user_manager.cpp \
	cliente.caramelo.cpp \
	cliente.cliente.cpp \
	cliente.game.cpp \
	cliente.ipwindow.cpp \
	cliente.main_window.cpp \
	cliente.socket_connect.cpp \
	cliente.sound_player.cpp \
	cliente.tablerojuego.cpp \
	cliente.thread_listener.cpp \
	clientemain.cpp


# the additional libraries needed to link
server_CXXFLAGS = $(AM_CXXFLAGS) $(PTHREAD_CFLAGS)
server_LDADD = $(PTHREAD_LIBS)

cliente_CXXFLAGS = $(AM_CXXFLAGS) $(GTKMM3_CFLAGS) $(ASOUND_CFLAGS)
cliente_LDADD = $(GTKMM3_LIBS) $(ASOUND_LIBS)

###############################################################################
