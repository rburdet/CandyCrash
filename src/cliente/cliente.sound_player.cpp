#include "cliente.sound_player.h"
#include "../common/common.logger.h"

#include <string>
#include <map>
#include <alsa/asoundlib.h>
#include <stdlib.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <ctime>
#include <inttypes.h>

#define PCM_DEVICE "default"

using std::string;

std::map<std::string, std::time_t> SoundPlayer::archs;

bool SoundPlayer::isMuted = false;

/* Cabecera de archivo wave:
 * http://www.topherlee.com/software/pcm-tut-wavformat.html
 */
typedef struct {
	char riff[4];
	uint32_t size;
	char wave[4];
	uint16_t fmt;
	uint32_t length;
	uint16_t type;
	uint16_t channels;
	uint32_t sample_rate;
	uint32_t bit_rate;
	uint32_t bits_per_sample;
	char data[4];
	uint32_t file_size;
} TWavHeader;

typedef struct {
	int sample_rate;
	int channels;
	int seconds;
	int fd;
} TWavArgs;

SoundPlayer::SoundPlayer(){}
SoundPlayer::~SoundPlayer(){}

bool SoundPlayer::play(const char* str){
	string s(str);
	return SoundPlayer::play(s);
}

bool SoundPlayer::play(const std::string & str){
	if(SoundPlayer::isMuted)
		return true;

	std::time_t now = std::time(NULL);
	if((SoundPlayer::archs).find(str) != (SoundPlayer::archs).end()){
		if(now == (SoundPlayer::archs)[str])
			return false;
	}

	(SoundPlayer::archs)[str] = now;
	return SoundPlayer::play_wave(str);
}

bool SoundPlayer::play_wave(const std::string & str){
	TWavHeader head;
	int fd = open(str.c_str(), O_RDONLY); 
	if(!fd){
		Logger::log("Error abriendo archivo. "+str);
		return false;
	}

	if(read(fd, &head, sizeof(TWavHeader)) != sizeof(TWavHeader)){
		Logger::log("Error leyendo cabecera wav. "+str);
		return false;
	}

	if(head.riff[0] != 'R' || head.riff[1] != 'I'  || head.riff[2] 
			!= 'F' || head.riff[3] != 'F'){
		//printf("No es archivo wav. RIFF. '%c' '%c' '%c' '%c'\n", 
		//head.riff[0], head.riff[1], head.riff[2], head.riff[3]);
		Logger::log("No es archivo wave: RIFF. "+str);
		return false;
	}

	if(head.wave[0] != 'W' || head.wave[1] != 'A'  || head.wave[2] != 
			'V' || head.wave[3] != 'E'){
		//printf("No es archivo wav. WAVE. '%c' '%c' '%c' '%c'\n", head.wave[0]
		//, head.wave[1], head.wave[2], head.wave[3]);
		Logger::log("No es archivo wave: WAVE. "+str);
		return false;
	}

	TWavArgs *args = (TWavArgs*) malloc(sizeof(TWavArgs));
	args->sample_rate = head.sample_rate;
	args->channels = head.channels;
	args->fd = fd;
	
	int ms = ((head.size - 44) * 1000) / head.bit_rate;
	args->seconds = ms / 1000;
	if( ms % 1000)
		args->seconds++;

	pthread_t me;
	pthread_create(&me, NULL, SoundPlayer::wav_runner, args);
	return true;
}

// Inspirado de: https://gist.github.com/ghedo/963382 
void* SoundPlayer::wav_runner(void* arg) {
	TWavArgs* args = (TWavArgs*) arg;
	unsigned int pcm, tmp, dir;
	snd_pcm_t *pcm_handle;
	snd_pcm_hw_params_t *params;
	snd_pcm_uframes_t frames;
	char *buff;
	int buff_size, loops;

	/* Open the PCM device in playback mode */
	if (pcm = snd_pcm_open(&pcm_handle, PCM_DEVICE,
					SND_PCM_STREAM_PLAYBACK, 0) < 0) {
		string str("ERROR: Can't open PCM device.");
		str += string(PCM_DEVICE);
		str += string(snd_strerror(pcm));
		Logger::log(str);
	}

	/* Allocate parameters object and fill it with default values*/
	snd_pcm_hw_params_alloca(&params);

	snd_pcm_hw_params_any(pcm_handle, params);

	/* Set parameters */
	if (pcm = snd_pcm_hw_params_set_access(pcm_handle, params,
					SND_PCM_ACCESS_RW_INTERLEAVED) < 0){
		string str("ERROR: Can't set interleaved mode.");
		str += string(snd_strerror(pcm));
		Logger::log(str);
	}

	if (pcm = snd_pcm_hw_params_set_format(pcm_handle, params,
						SND_PCM_FORMAT_S16_LE) < 0) {
		string str("ERROR: Can't set format.");
		str += string(snd_strerror(pcm));
		Logger::log(str);
	}

	if (pcm = snd_pcm_hw_params_set_channels(pcm_handle, params, args->channels) 
			< 0) {
		string str("ERROR: Can't set channels number. ");
		str += string(snd_strerror(pcm));
		Logger::log(str);
	}

	if (pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params, 
				(unsigned int*) &(args->sample_rate), NULL) < 0) {
		string str("ERROR: Can't set rate. ");
		str += string(snd_strerror(pcm));
		Logger::log(str);
	}

	/* Write parameters */
	if (pcm = snd_pcm_hw_params(pcm_handle, params) < 0){
		string str("ERROR: Can't set harware parameters. ");
		str += string(snd_strerror(pcm));
		Logger::log(str);
	}

	/* Resume information */
	//printf("PCM name: '%s'\n", snd_pcm_name(pcm_handle));
	//printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(pcm_handle)));

	snd_pcm_hw_params_get_channels(params, &tmp);
	//printf("channels: %i ", tmp);

	//if (tmp == 1)
	//	printf("(mono)\n");
	//else if (tmp == 2)
	//	printf("(stereo)\n");

	snd_pcm_hw_params_get_rate(params, &tmp, 0);
	//printf("rate: %d bps\n", tmp);

	//printf("seconds: %d\n", args->seconds);	

	/* Allocate buffer to hold single period */
	snd_pcm_hw_params_get_period_size(params, &frames, 0);

	buff_size = frames * args->channels * 2 /* 2 -> sample size */;
	buff = (char *) malloc(buff_size);

	snd_pcm_hw_params_get_period_time(params, &tmp, NULL);

	for (loops = (args->seconds * 1000000) / tmp; loops > 0; loops--) {
		//if (pcm = read(0, buff, buff_size) == 0) {
		if (pcm = read(args->fd, buff, buff_size) == 0) {
			Logger::log("Archivo termino antes de lo que se esperaba");
			return NULL;
		}

		if (pcm = snd_pcm_writei(pcm_handle, buff, frames) == -EPIPE) {
			Logger::log("XRUN");
			snd_pcm_prepare(pcm_handle);
		} else if (pcm < 0) {
			string str("ERROR. Can't write to PCM device. ");
			str += string(snd_strerror(pcm));
			Logger::log(str);
		}
	}

	snd_pcm_drain(pcm_handle);
	snd_pcm_close(pcm_handle);
	free(buff);
	close(args->fd);
	free(args);
}

void SoundPlayer::mute(){
	SoundPlayer::isMuted = true;
}

void SoundPlayer::unmute(){
	SoundPlayer::isMuted = false;
}

void SoundPlayer::togglemute(){
	if(SoundPlayer::isMuted)
		SoundPlayer::isMuted = false;
	else
		SoundPlayer::isMuted = true;
}
