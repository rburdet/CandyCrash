#include "probsetter.h"

ProbSetter::ProbSetter(Tablero* elTablero , NoteProbabilidades* boxProbs){
	this->tablero=elTablero;
	this->probs=boxProbs;
}

ProbSetter::~ProbSetter(){}


