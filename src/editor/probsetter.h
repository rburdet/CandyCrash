#ifndef PROB_SETTER_H
#define PROB_SETTER_H

#include "noteprobabilidades.h"
#include "tablero.h"

//Clase intermedia para interactuar desde el notebook a el tablero
class ProbSetter{
	public:
		ProbSetter(Tablero* tablero, NoteProbabilidades* boxProbs);
		~ProbSetter();
	private:
		Tablero* tablero;
		NoteProbabilidades* probs;

};

#endif
