#include "./frequency.h"

template <class T> FreqImage<T>::FreqImage(name, type, width, height) : Image(name, type, width, height){
	setInfo(1.0, 1.0, 1.0, 0.0);
	setOrigin(0.0, 0.0);
}
template <class T> FreqImage<T>::setOrigin(float x, float y){
	this->origin_x = x;
	this->origin_y = y;
}

template <class T> FreqImage<T>::setInfo(T amp, T freq_x, T freq_y, T phase){
	this->amp = amp;
	this->freq_x = freq_x;
	this->freq_y = freq_y;
	this->phase = phase;
}
