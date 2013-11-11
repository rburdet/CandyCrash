#include "info.h"

Info::Info(){
	for ( int i = 0 ; i < NUMEROPIEZAS ; i++ ) {
		prob_piezas[i]=0;
	}
}

Info::~Info(){}

void Info::setProb_piezas(int prob,int x){
	prob_piezas[x]=prob;
}

int Info::getProb_piezas(int idx){
	return prob_piezas[idx];
}

int* Info::getArray(){
	return this->prob_piezas;
}
