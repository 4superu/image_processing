#include "./image.h"

template <class T> Image<T>::Image(std::string name, std::string type, size_t width, size_t height) : name(name), type(type), width(width), height(height){
	setImgName();
	image.resize(width*height, 0);
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

template <typename T> void Image<T>::selectNextPixels(){
	for(int i=0; i<(int)height; i++){
		if(i%2 == 0){
			for(int s=0; s<(int)width-1; s++){
				image[i*(int)width+s] = i*(int)width+s+1;
			}
			image[i*(int)width+(int)width-1] = (i+2)*(int)width-1;
		}
		else{
			for(int t=(int)width-1; t>0; t--){
				image[i*(int)width+t] = i*(int)width+t - 1;
			}
			image[i*(int)width] = (i+1)*(int)width;
		}
	}

}
