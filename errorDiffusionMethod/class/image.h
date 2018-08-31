#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <cfloat>
using namespace std;

template <typename T>
class Image{
	public:
		std::string file_name, name, type;
		std::vector<T> image;
		size_t width, height;

		Image(std::string name, std::string type, size_t width, size_t height);

		//method
		void setImgName();
		void readRawFile();
		void writeRawFile();
		void selectNextPixels();
};

