#include "../class/image.cpp"

template <typename T>
class FreqImage : Image<T>{
	public:
		float origin_x, origin_y;
		T amp, freq_x, freq_y, phase;

		FreqImage(std::string name, std::string type, size_t width, size_t height);
		void sinImage();
		void cosImage();
		void setOrigin(float x, float y);
		void setInfo(T amp, T freq_x, T freq_y, T phase);
};

