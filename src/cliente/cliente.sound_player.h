#ifndef __SOUND_PLAYER_H__
#define __SOUND_PLAYER_H__
#include <map>
#include <string>
#include <ctime>

/** Clase singleton usada para reproducir sonidos.
 * No es threadsafe, solo se puede ejecutar desde un thread.
 * Si se mandan a reproducir dos sonidos iguales en el mismo segundo, solo reproduce el primero.
 */
class SoundPlayer {
	protected:
		SoundPlayer();
		~SoundPlayer();
		static bool play_wave(const std::string& str);
		//static SoundPlayer* me;
		/** Guarda usando de clave el path, la hora de cunado fue reproducido. Se usa para ignorar archivos iguales si se reproducen en el mismo momento
		 */
		static std::map<std::string, std::time_t> archs;
		void thread_play(int fd, int sample_rate, int seconds);
		static void* wav_runner(void* arg);


	public:
		//static void init();
		//static void destroy();
		/** Funciones usadas para reproducir.
		 */
		static bool play(const std::string& str);
		static bool play(const char* str);
};


#endif
