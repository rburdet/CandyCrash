#ifndef INFO_H
#define INFO_H

#define NUMEROPIEZAS 16

class Info{
	public:
		Info();
		~Info();
		void setProb_piezas(int prob,int x);
		int getProb_piezas(int idx);
		int* getArray();

	private:
		int prob_piezas[NUMEROPIEZAS];
};

#endif
