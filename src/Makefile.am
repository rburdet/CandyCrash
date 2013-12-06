# this is example-file: src/bin/Makefile.am

# additional include pathes necessary to compile the C++ programs
AM_CXXFLAGS = $(JSONCPP_CFLAGS) -std=c++0x @AM_CXXFLAGS@
AM_LDFLAGS = $(JSONCPP_LIBS) -std=c++0x @AM_LDFLAGS@

###############################################################################
# THE PROGRAMS TO BUILD
###############################################################################

# the program to build (the names of the final binaries)
bin_PROGRAMS = candycrash_server candycrash_cliente

# list of sources for the 'helloWorld' binary
candycrash_server_SOURCES = \
	common.caramelos.h \
	common.caramelos_movimientos.h \
	common.events.h \
	common.hmac.cpp \
	common.hmac.h \
	common.logger.cpp \
	common.logger.h \
	common.md5.cpp \
	common.md5.h \
	common.mutex.cpp \
	common.mutex.h \
	common.path_manager.cpp \
	common.path_manager.h \
	common.socket.cpp \
	common.socket.h \
	common.socket_io.cpp \
	common.socket_io.h \
	common.thread.cpp \
	common.thread.h \
	common.thread_socket.cpp \
	common.thread_socket.h \
	common.user_manager.cpp \
	common.user_manager.h \
	server.cpp \
	server.listador.h \
	server.partida.cpp \
	server.partida.h \
	server.partida_interface.h \
	server.server.cpp \
	server.server.h \
	server.server_interface.h \
	server.socket_listener.cpp \
	server.socket_listener.h \
	server.tablero.cpp \
	server.tablero.h \
	server.thread_usuario.cpp \
	server.thread_usuario.h


candycrash_cliente_SOURCES = \
	cliente.bar.h \
	cliente.button.h \
	cliente.caramelo.cpp \
	cliente.caramelo.h \
	cliente.cliente.cpp \
	cliente.cliente.h \
	cliente.cliente_interface.h \
	cliente.factory_caramelos.h \
	cliente.game.cpp \
	cliente.game.h \
	cliente.ipwindow.cpp \
	cliente.ipwindow.h \
	clientemain.cpp \
	cliente.main_window.cpp \
	cliente.main_window.h \
	cliente.socket_connect.cpp \
	cliente.socket_connect.h \
	cliente.sound_player.cpp \
	cliente.sound_player.h \
	cliente.star.h \
	cliente.tablerojuego.cpp \
	cliente.tablerojuego.h \
	cliente.thread_listener.cpp \
	cliente.thread_listener.h \
	cliente.window.h \
	common.caramelos.h \
	common.caramelos_movimientos.h \
	common.events.h \
	common.hmac.cpp \
	common.hmac.h \
	common.logger.cpp \
	common.logger.h \
	common.md5.cpp \
	common.md5.h \
	common.mutex.cpp \
	common.mutex.h \
	common.path_manager.cpp \
	common.path_manager.h \
	common.socket.cpp \
	common.socket.h \
	common.socket_io.cpp \
	common.socket_io.h \
	common.thread.cpp \
	common.thread.h \
	common.thread_socket.cpp \
	common.thread_socket.h \
	common.user_manager.cpp \
	common.user_manager.h \
	widgets/cliente.label_entry.h \
	widgets/cliente.mapas_list.h \
	widgets/cliente.menu_bar.h \
	widgets/cliente.partidas_list.h \
	widgets/cliente.usuarios_list.h



# the additional libraries needed to link
candycrash_server_CXXFLAGS = $(AM_CXXFLAGS) $(PTHREAD_CFLAGS)
candycrash_server_LDADD = $(PTHREAD_LIBS)

candycrash_cliente_CXXFLAGS = $(AM_CXXFLAGS) $(GTKMM3_CFLAGS) $(ASOUND_CFLAGS)
candycrash_cliente_LDADD = $(GTKMM3_LIBS) $(ASOUND_LIBS)

#candycrash_serverdir = $(prefix)/share/$(PACKAGE)/server/
candycrash_serverdir = $(prefix)/share
nobase_dist_candycrash_server_DATA = \
	../share/candycrash/Mapas/posta.map \
	../share/candycrash/Mapas/preentrega.map

#candycrash_clientedir = $(prefix)/share/$(PACKAGE)/cliente/
candycrash_clientedir = $(prefix)/share
nobase_dist_candycrash_cliente_DATA = \
	../share/candycrash/imagenes/bar_hor_blue.png \
	../share/candycrash/imagenes/button_purple.png \
	../share/candycrash/imagenes/bar_hor_yellow.png \
	../share/candycrash/imagenes/fondo.jpg \
	../share/candycrash/imagenes/button_blue.png \
	../share/candycrash/imagenes/hueco.png \
	../share/candycrash/imagenes/button_green.png \
	../share/candycrash/imagenes/bar_vert_purple.png \
	../share/candycrash/imagenes/dot.png \
	../share/candycrash/imagenes/bar_vert_blue.png \
	../share/candycrash/imagenes/bar_vert_green.png \
	../share/candycrash/imagenes/fondos/stripes2.jpg \
	../share/candycrash/imagenes/fondos/paradise.jpg \
	../share/candycrash/imagenes/fondos/primera.jpg \
	../share/candycrash/imagenes/fondos/stripes.jpg \
	../share/candycrash/imagenes/bar_hor_red.png \
	../share/candycrash/imagenes/star.png \
	../share/candycrash/imagenes/bar_vert_yellow.png \
	../share/candycrash/imagenes/style.css \
	../share/candycrash/imagenes/bar_hor_green.png \
	../share/candycrash/imagenes/bar_hor_purple.png \
	../share/candycrash/imagenes/button_yellow.png \
	../share/candycrash/imagenes/star_big.png \
	../share/candycrash/imagenes/bar_vert_red.png \
	../share/candycrash/imagenes/button_red.png \
	../share/candycrash/sounds/message-new-instant.wav \
	../share/candycrash/sounds/audio-channel-rear-center.wav \
	../share/candycrash/sounds/alarm-clock-elapsed.wav \
	../share/candycrash/sounds/device-added.wav \
	../share/candycrash/sounds/audio-channel-rear-right.wav \
	../share/candycrash/sounds/CREDITS \
	../share/candycrash/sounds/audio-channel-side-left.wav \
	../share/candycrash/sounds/camera-shutter.wav \
	../share/candycrash/sounds/bell.wav \
	../share/candycrash/sounds/service-login.wav \
	../share/candycrash/sounds/phone-outgoing-busy.wav \
	../share/candycrash/sounds/trash-empty.wav \
	../share/candycrash/sounds/dialog-information.wav \
	../share/candycrash/sounds/audio-channel-side-right.wav \
	../share/candycrash/sounds/message.wav \
	../share/candycrash/sounds/audio-test-signal.wav \
	../share/candycrash/sounds/complete.wav \
	../share/candycrash/sounds/phone-outgoing-calling.wav \
	../share/candycrash/sounds/audio-channel-front-left.wav \
	../share/candycrash/sounds/suspend-error.wav \
	../share/candycrash/sounds/dialog-warning.wav \
	../share/candycrash/sounds/phone-incoming-call.wav \
	../share/candycrash/sounds/device-removed.wav \
	../share/candycrash/sounds/audio-channel-rear-left.wav \
	../share/candycrash/sounds/audio-channel-front-center.wav \
	../share/candycrash/sounds/service-logout.wav \
	../share/candycrash/sounds/audio-volume-change.wav \
	../share/candycrash/sounds/audio-channel-front-right.wav

###############################################################################
