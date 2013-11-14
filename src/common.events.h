#ifndef __COMMON_EVENTS_H__
#define __COMMON_EVENTS_H__
/** Definicion de todos los eventos utilizados en los mensajes entre cliente y servidor.
 * TODOS: los mensjaes deben tener un campo de "event" que haga referencia a uno de estos eventos del enum.
 * Cuando el cliente envie mensajes al servidor con "event" "EVENT_GAME_MISC", se tendra que especificar un segundo campo, "ev-game", con alguno de estos eventos.
 */
typedef enum {
	EVENT_NONE=0, // -> Nada
	EVENT_LOGIN, // -> Login de usuario
	EVENT_NEW_USER, // -> Newo usuario
	EVENT_LIST_GAMES, // -> Listar partidas
	EVENT_NEW_GAME, // -> Crear partida
	EVENT_JOIN_GAME, // -> Unirse a partida
	EVENT_GAME_MISC, // -> Jsons que se pasan directamente al juego
	EVENT_GET_MAPS, // -> Lista de mapas para crear
	EVENT_GAME_CHAT, // -> Lista de mapas para crear
	EVENT_GAME_INFO,
	EVENT_GAME_USER_ADD,
	EVENT_GAME_USER_RM
} CommonEvents;

#endif
