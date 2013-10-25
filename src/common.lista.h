#ifndef __LISTA_HPP__
#define __LISTA_HPP__

template<class T>
class Nodo {
	protected:
		T ele;
		Nodo<T> *sig;

	public:
		explicit Nodo(T& e) : ele(e), sig(NULL){}
		~Nodo(){
			if(this->sig)
				delete this->sig;
		}

		Nodo<T>& operator+=(Nodo<T> *n){
			this->sig = n;
			return *this;
		}

		Nodo<T>* operator++(int){
			return this->sig;
		}

		Nodo<T>& operator>>(T& ele){
			ele  = this->ele;
			return *this;
		}
};

template<class T>
class Lista {
	protected:
		Nodo<T> *cte, *pri;

	public:
		Lista() : cte(NULL), pri(NULL){
		}

		~Lista(){
			if(this->pri)
				delete this->pri;
		}

		/** Agrega elemento siguiente de cte
		 */
		Lista<T>& operator<<(T &ele){
			Nodo<T>* n = new Nodo<T>(ele);
			if (!this->pri){
				this->pri = n;
			}else if (this->cte){
				(*n) += (*this->cte)++;
				(*this->cte) += n;
			}

			this->cte = n;

			return *this;
		}

		/** Reemplaza Cte por elemento
		 */
		Lista<T>& operator<(T &ele){
			Nodo<T>* n = new Nodo<T>(ele);

			if (! this->pri){
				this->pri = n;
			}else if (this->cte){
				Nodo<T> * aux = this->pri;
				(*n) += (*this->cte)++;

				do {
					if((*aux)++ == this->cte){
						(*aux) += n;
						break;
					}
				}while((aux = (*aux)++));
				if(!aux)
					this->pri = n;

				(*this->cte) += NULL;
				delete this->cte;
			}

			this->cte = n;
			return *this;
		}
		
		/** Devuelve el elemento cte
		 */
		Lista<T>& operator>>(T& ele){
			if(this->cte)
				(*this->cte) >> ele;

			return *this;
		}

		/**Mueve Cte al primero
		 */
		Lista<T>& operator~(){
			this->cte = this->pri;
			return *this;
		}

		/** Mueve al siguiente
		 * @return bool: si no se pudo realizar el movimiento devuelve false
		 */
		bool operator++(int){
			if(this->cte){
				Nodo<T>* n = (* this->cte)++;
				if(n){
					this->cte = n;
					return true;
				}
			}

			return false;
		}

		/** Esta vacia?.
		 * @return true: si esta vacia, falso sino
		 */
		bool operator!(){
			//if(this->cte){
			//	if((*this->cte)++)
			//		return true;
			//}
			if(this->pri)
				return false;

			return true;
		}

		Lista<T>& operator=(Lista<T>& lst){
			//if(this->pri)
			//	delete this->pri;

			if (lst.pri){
				Nodo<T>* auxCte = lst.cte;
				T aux;
				~lst;
				do {
					lst >> aux;
					(*this) << aux;
				} while (lst++);

				lst.cte = auxCte;
				~(*this);
			}

			return *this;
		}
};

#endif
