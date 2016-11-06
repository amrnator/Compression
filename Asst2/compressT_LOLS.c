//compressT_LOLS.c


#include "rle.c"
#include <math.h>
main(){
	
	char* z = "iiiiffeeeeeeeeeeeeeeeeeegggcczzz";
	char * x =  encode(z);
	printf("Final: %s\n",x);
}