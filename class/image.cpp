#include "./image.h"

template <class T> Image<T>::Image(std::string name, std::string type, size_t width, size_t heigth) : name(name), type(type), width(128), height(128){
	setImgName();
	image.resize(width*height, 0.0);
};
	
template <class T> void Image<T>::setImgName(){
	std::stringstream fname;
	fname << this->name << "_" << type << "_" << width << "-" << height << ".raw";
	this->file_name = fname.str();
}

template <class T> void Image<T>::readRawFile(){
	std::ifstream ifs(file_name.c_str());
	if (!ifs) std::cerr << "Failed to open " << file_name << std::endl;
	else{ ifs.read(reinterpret_cast<char*>(&image[0]), sizeof(T) * image.size()); }
}

template <typename T> void Image<T>::writeRawFile(){
	std::ofstream ofs(file_name.c_str());
	if (!ofs) std::cerr << "Failed to open " << file_name << std::endl;
	else { ofs.write(reinterpret_cast<char*>(&image[0]), sizeof(T) * image.size()); }
}

