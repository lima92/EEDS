#include <fonts.h>

//http://stackoverflow.com/questions/8617889/return-a-2d-array-from-a-function

typedef int (*ptr_double_size_char)[14][10];

ptr_double_size_char resize_double(int letter[7][5]){
	ptr_double_size_char result = malloc(sizeof(*result));
	for (int i = 0; i < 7; i++){
		for (int j = 0; j < 5; j++){
			(*result)[i*2][j*2] = letter[i][j];
			(*result)[i*2][j*2 + 1] = letter[i][j];
			(*result)[i*2 + 1][j*2] = letter[i][j];
			(*result)[i*2 + 1][j*2 + 1] = letter[i][j];
		}
	}
	return result;
}
