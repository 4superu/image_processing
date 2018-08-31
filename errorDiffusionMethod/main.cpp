#include "./class/image.cpp"

#define MAIN
int main(){
	const float MAX_VALUE = 255;
	const int IMAGE_WIDTH = 256;
	Image<unsigned char> orig_image("../lenna", "unsignedchar", IMAGE_WIDTH, IMAGE_WIDTH);
	Image<float> app_image("../newlenna", "float", IMAGE_WIDTH, IMAGE_WIDTH);
	Image<int> coord_image("coord", "int", IMAGE_WIDTH, IMAGE_WIDTH);

	orig_image.readRawFile();
	coord_image.selectNextPixels();
	coord_image.writeRawFile();
	int current_coord = 0;

	for(int i=0; i<IMAGE_WIDTH; i++){
		for(int j=0; j<IMAGE_WIDTH; j++){
			app_image.image[i*IMAGE_WIDTH+j] = (float)orig_image.image[i*IMAGE_WIDTH+j];
		}
	}
	app_image.writeRawFile();

	int next_coord = 1;
#ifdef MAIN
	while(next_coord != IMAGE_WIDTH*(IMAGE_WIDTH-1)){
		if(app_image.image[current_coord] != 255){
				next_coord = coord_image.image[current_coord];
				float value = app_image.image[next_coord] + app_image.image[current_coord];
				app_image.image[current_coord] = 0;
				
				while(value > MAX_VALUE){
					current_coord = next_coord;
					next_coord = coord_image.image[current_coord];
					app_image.image[current_coord] = 255;
					value = value - 255;

					if(!((current_coord%IMAGE_WIDTH == 0 && (current_coord/IMAGE_WIDTH)%2 == 1) || ((current_coord+1)%IMAGE_WIDTH == 0 && (current_coord/IMAGE_WIDTH)%2 == 0))){
						app_image.image[next_coord] += value * 7.0/16.0;
						if(current_coord/IMAGE_WIDTH != IMAGE_WIDTH-1){
							app_image.image[next_coord+IMAGE_WIDTH] += value * 1.0/16.0;
						}
					}
					if(current_coord/IMAGE_WIDTH != IMAGE_WIDTH-1){
						app_image.image[current_coord+IMAGE_WIDTH] += value * 5.0/16.0;
						if((current_coord/IMAGE_WIDTH)%2 == 0){
							app_image.image[current_coord+IMAGE_WIDTH+1] += value * 1.0/16.0;
							app_image.image[current_coord+IMAGE_WIDTH-1] += value * 3.0/16.0;
						}
						else{
							app_image.image[current_coord+IMAGE_WIDTH-1] += value * 1.0/16.0;
							app_image.image[current_coord+IMAGE_WIDTH+1] += value * 3.0/16.0;
						}
					}
					value = app_image.image[next_coord];
				}
				if(value == MAX_VALUE){
					app_image.image[next_coord]  = value;
					current_coord = coord_image.image[next_coord];
				}
				else{
					app_image.image[next_coord] = value;
					current_coord = next_coord;
				}
		}
	}

	app_image.writeRawFile();

	#endif
}
