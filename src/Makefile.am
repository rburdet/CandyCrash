AUTOMAKE_OPTIONS = subdir-objects foreign

# additional include pathes necessary to compile the C++ programs
AM_CXXFLAGS = $(JSONCPP_CFLAGS) -std=c++0x @AM_CXXFLAGS@
AM_LDFLAGS = $(JSONCPP_LIBS) -std=c++0x @AM_LDFLAGS@

###############################################################################
# THE PROGRAMS TO BUILD
###############################################################################

# the program to build (the names of the final binaries)
bin_PROGRAMS = candycrash-server candycrash-cliente candycrash-editor

# list of sources for the 'helloWorld' binary
candycrash_server_SOURCES = \
	common/common.caramelos.h \
	common/common.caramelos_movimientos.h \
	common/common.events.h \
	common/common.hmac.cpp \
	common/common.hmac.h \
	common/common.logger.cpp \
	common/common.logger.h \
	common/common.md5.cpp \
	common/common.md5.h \
	common/common.mutex.cpp \
	common/common.mutex.h \
	common/common.path_manager.cpp \
	common/common.path_manager.h \
	common/common.socket.cpp \
	common/common.socket.h \
	common/common.socket_io.cpp \
	common/common.socket_io.h \
	common/common.thread.cpp \
	common/common.thread.h \
	common/common.thread_socket.cpp \
	common/common.thread_socket.h \
	common/common.user_manager.cpp \
	common/common.user_manager.h \
	server/server.cpp \
	server/server.listador.h \
	server/server.partida.cpp \
	server/server.partida.h \
	server/server.partida_interface.h \
	server/server.server.cpp \
	server/server.server.h \
	server/server.server_interface.h \
	server/server.socket_listener.cpp \
	server/server.socket_listener.h \
	server/server.tablero.cpp \
	server/server.tablero.h \
	server/server.thread_usuario.cpp \
	server/server.thread_usuario.h


candycrash_cliente_SOURCES = \
	cliente/cliente.bar.h \
	cliente/cliente.button.h \
	cliente/cliente.caramelo.cpp \
	cliente/cliente.caramelo.h \
	cliente/cliente.cliente.cpp \
	cliente/cliente.cliente.h \
	cliente/cliente.cliente_interface.h \
	cliente/cliente.factory_caramelos.h \
	cliente/cliente.game.cpp \
	cliente/cliente.game.h \
	cliente/cliente.ipwindow.cpp \
	cliente/cliente.ipwindow.h \
	cliente/cliente.cpp \
	cliente/cliente.main_window.cpp \
	cliente/cliente.main_window.h \
	cliente/cliente.socket_connect.cpp \
	cliente/cliente.socket_connect.h \
	cliente/cliente.sound_player.cpp \
	cliente/cliente.sound_player.h \
	cliente/cliente.star.h \
	cliente/cliente.tablerojuego.cpp \
	cliente/cliente.tablerojuego.h \
	cliente/cliente.thread_listener.cpp \
	cliente/cliente.thread_listener.h \
	cliente/cliente.window.h \
	common/common.caramelos.h \
	common/common.caramelos_movimientos.h \
	common/common.events.h \
	common/common.hmac.cpp \
	common/common.hmac.h \
	common/common.logger.cpp \
	common/common.logger.h \
	common/common.md5.cpp \
	common/common.md5.h \
	common/common.mutex.cpp \
	common/common.mutex.h \
	common/common.path_manager.cpp \
	common/common.path_manager.h \
	common/common.socket.cpp \
	common/common.socket.h \
	common/common.socket_io.cpp \
	common/common.socket_io.h \
	common/common.thread.cpp \
	common/common.thread.h \
	common/common.thread_socket.cpp \
	common/common.thread_socket.h \
	common/common.user_manager.cpp \
	common/common.user_manager.h \
	cliente/widgets/cliente.label_entry.h \
	cliente/widgets/cliente.mapas_list.h \
	cliente/widgets/cliente.menu_bar.h \
	cliente/widgets/cliente.partidas_list.h \
	cliente/widgets/cliente.usuarios_list.h

candycrash_editor_SOURCES = \
	common/common.path_manager.cpp \
	common/common.path_manager.h \
	editor/barraestado.cpp \
	editor/barraestado.h \
	editor/boxopcionesbasicas.cpp \
	editor/boxopcionesbasicas.h \
	editor/celda.cpp \
	editor/celda.h \
	editor/columna.cpp \
	editor/columna.h \
	editor/generalprobs.cpp \
	editor/generalprobs.h \
	editor/info.cpp \
	editor/info.h \
	editor/main.cpp \
	editor/mainwindow.cpp \
	editor/mainwindow.h \
	editor/menu.cpp \
	editor/menu.h \
	editor/noteprobabilidades.cpp \
	editor/noteprobabilidades.h \
	editor/persistidor.h \
	editor/probceldas.cpp \
	editor/probceldas.h \
	editor/probcolumnas.cpp \
	editor/probcolumnas.h \
	editor/sonidos.cpp \
	editor/sonidos.h \
	cliente/cliente.sound_player.cpp \
	cliente/cliente.sound_player.h \
	common/common.logger.cpp \
	common/common.logger.h \
	common/common.mutex.cpp \
	common/common.mutex.h \
	editor/tablero.cpp \
	editor/tablero.h \
	editor/menupersistidor.cpp \
	editor/menupersistidor.h



# the additional libraries needed to link
candycrash_server_CXXFLAGS = $(AM_CXXFLAGS) $(PTHREAD_CFLAGS)
candycrash_server_LDADD = $(PTHREAD_LIBS)

candycrash_cliente_CXXFLAGS = $(AM_CXXFLAGS) $(GTKMM3_CFLAGS) $(ASOUND_CFLAGS)
candycrash_cliente_LDADD = $(GTKMM3_LIBS) $(ASOUND_LIBS)

candycrash_editor_CXXFLAGS = $(AM_CXXFLAGS) $(GTKMM2_CFLAGS) $(ASOUND_CFLAGS)
candycrash_editor_LDADD = $(GTKMM2_LIBS) $(ASOUND_LIBS)


#candycrash_serverdir = $(prefix)/share/$(PACKAGE)/server/
candycrash_serverdir = $(prefix)/share
#nobase_dist_candycrash_server_DATA = \
#	../share/candycrash/Mapas/posta.map \
#	../share/candycrash/Mapas/preentrega.map

#candycrash_clientedir = $(prefix)/share/$(PACKAGE)/cliente/
candycrash_clientedir = $(prefix)/share
nobase_dist_candycrash_cliente_DATA = \
	../share/candycrash/imagenes/bar_hor_blue.png \
	../share/candycrash/imagenes/button_purple.png \
	../share/candycrash/imagenes/bar_hor_yellow.png \
	../share/candycrash/imagenes/fondo.jpg \
	../share/candycrash/imagenes/button_blue.png \
	../share/candycrash/imagenes/hueco.png \
	../share/candycrash/imagenes/nohueco.png \
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

candycrash_editordir = $(prefix)/share
nobase_dist_candycrash_editor_DATA = \
	../share/candycrash/editor/Editor.glade

###############################################################################
