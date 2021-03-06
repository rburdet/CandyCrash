#ifndef __COMMON_EVENTS_H__
#define __COMMON_EVENTS_H__
typedef enum {
	EVENT_NONE=0, // -> Nada
	EVENT_LOGIN, // -> Login de usuario
	EVENT_NEW_USER, // -> Newo usuario
	EVENT_LIST_GAMES, // -> Listar partidas
	EVENT_NEW_GAME, // -> Crear partida
	EVENT_JOIN_GAME, // -> Unirse a partida
	EVENT_GAME_MISC, // -> Jsons que se pasan directamente al juego
	EVENT_GET_MAPS, // -> Lista de mapas para crear
	EVENT_GAME_CHAT // -> Lista de mapas para crear
} CommonEvents;

#endif
