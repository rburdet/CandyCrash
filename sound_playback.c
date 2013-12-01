/*
 * Simple sound playback using ALSA API and libasound.
 *
 * Compile:
 * $ cc -o play sound_playback.c -lasound
 * 
 * Usage:
 * $ ./play <sample_rate> <channels> <seconds> < <file>
 * 
 * Examples:
 * $ ./play 44100 2 5 < /dev/urandom
 * $ ./play 22050 1 8 < /path/to/file.wav
 *
 * Copyright (C) 2009 Alessandro Ghedini <alessandro@ghedini.me>
 * --------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Alessandro Ghedini wrote this file. As long as you retain this
 * notice you can do whatever you want with this stuff. If we
 * meet some day, and you think this stuff is worth it, you can
 * buy me a beer in return.
 * --------------------------------------------------------------
 *  Requiere instalar libasound2 y libasound2-dev para que ande
 */

#include <alsa/asoundlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define PCM_DEVICE "default"
// http://www.topherlee.com/software/pcm-tut-wavformat.html
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

int main(int argc, char **argv) {
	unsigned int pcm, tmp, dir;
	int rate, channels, seconds;
	snd_pcm_t *pcm_handle;
	snd_pcm_hw_params_t *params;
	snd_pcm_uframes_t frames;
	char *buff;
	int buff_size, loops;
	TWavHeader head;

	if (argc < 2) {
		printf("Usage: %s <wave_file>\n",
								argv[0]);
		return -1;
	}

	//rate 	 = atoi(argv[1]);
	//channels = atoi(argv[2]);
	//seconds  = atoi(argv[3]);
	
	int fd = open(argv[1], O_RDONLY); 
	if(!fd){
		printf("Error abriendo archivo\n");
		return -1;
	}

	if(read(fd, &head, sizeof(TWavHeader)) != sizeof(TWavHeader)){
		printf("Error leyendo cabecera de wav\n");
		return -1;
	}

	if(head.riff[0] != 'R' || head.riff[1] != 'I'  || head.riff[2] != 'F' || head.riff[3] != 'F'){
		printf("No es archivo wav. RIFF. '%c' '%c' '%c' '%c'\n", head.riff[0], head.riff[1], head.riff[2], head.riff[3]);
		return -1;
	}

	if(head.wave[0] != 'W' || head.wave[1] != 'A'  || head.wave[2] != 'V' || head.wave[3] != 'E'){
		printf("No es archivo wav. WAVE. '%c' '%c' '%c' '%c'\n", head.wave[0], head.wave[1], head.wave[2], head.wave[3]);
		return -1;
	}

	//ms=((fileSize-44)*1000))/byteRate
	printf("Size %d\n", head.size);
	int ms = ((head.size - 44) * 1000 ) / head.bit_rate;
	printf("channels %d sample_rate %d duration(ms) %d\n", head.channels, head.sample_rate, ms);
	rate = head.sample_rate;
	channels = head.channels;
	seconds = ms / 1000;
	if( ms % 1000)
		seconds++;

	/* Open the PCM device in playback mode */
	if (pcm = snd_pcm_open(&pcm_handle, PCM_DEVICE,
					SND_PCM_STREAM_PLAYBACK, 0) < 0) 
		printf("ERROR: Can't open \"%s\" PCM device. %s\n",
					PCM_DEVICE, snd_strerror(pcm));

	/* Allocate parameters object and fill it with default values*/
	snd_pcm_hw_params_alloca(&params);

	snd_pcm_hw_params_any(pcm_handle, params);

	/* Set parameters */
	if (pcm = snd_pcm_hw_params_set_access(pcm_handle, params,
					SND_PCM_ACCESS_RW_INTERLEAVED) < 0) 
		printf("ERROR: Can't set interleaved mode. %s\n", snd_strerror(pcm));

	if (pcm = snd_pcm_hw_params_set_format(pcm_handle, params,
						SND_PCM_FORMAT_S16_LE) < 0) 
		printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));

	if (pcm = snd_pcm_hw_params_set_channels(pcm_handle, params, channels) < 0) 
		printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));

	if (pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, 0) < 0) 
		printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));

	/* Write parameters */
	if (pcm = snd_pcm_hw_params(pcm_handle, params) < 0)
		printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));

	/* Resume information */
	printf("PCM name: '%s'\n", snd_pcm_name(pcm_handle));

	printf("PCM state: %s\n", snd_pcm_state_name(snd_pcm_state(pcm_handle)));

	snd_pcm_hw_params_get_channels(params, &tmp);
	printf("channels: %i ", tmp);

	if (tmp == 1)
		printf("(mono)\n");
	else if (tmp == 2)
		printf("(stereo)\n");

	snd_pcm_hw_params_get_rate(params, &tmp, 0);
	printf("rate: %d bps\n", tmp);

	printf("seconds: %d\n", seconds);	

	/* Allocate buffer to hold single period */
	snd_pcm_hw_params_get_period_size(params, &frames, 0);

	buff_size = frames * channels * 2 /* 2 -> sample size */;
	buff = (char *) malloc(buff_size);

	snd_pcm_hw_params_get_period_time(params, &tmp, NULL);

	for (loops = (seconds * 1000000) / tmp; loops > 0; loops--) {

		//if (pcm = read(0, buff, buff_size) == 0) {
		if (pcm = read(fd, buff, buff_size) == 0) {
			printf("Early end of file.\n");
			return 0;
		}

		if (pcm = snd_pcm_writei(pcm_handle, buff, frames) == -EPIPE) {
			printf("XRUN.\n");
			snd_pcm_prepare(pcm_handle);
		} else if (pcm < 0) {
			printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(pcm));
		}

	}

	snd_pcm_drain(pcm_handle);
	snd_pcm_close(pcm_handle);
	free(buff);

	return 0;
}
