/*
 * main.c
 *
 *  Created on: May 3, 2023
 *      Author: Cristopher Manuel Afonso Mora / Ángel Javier Díaz Balboa
 */

#include <stdio.h>
#include "io.h"
#include "pgm4.h"
#include "system.h"

int main() __attribute__ ((section(".main.text")));
int main() {

	char* path_in = "/mnt/host/boat.512.pgm";
	char* path_out = "/mnt/host/boat_conv_tr.512.pgm";
	int width_photo = 0;
	int height_photo = 0;
	unsigned char** orig_uc_matrix = pgmread2(path_in, &width_photo, &height_photo);
	unsigned char** dest_uc_matrix = (unsigned char**)malloc(512*sizeof(unsigned char*));
	//unsigned char** dest_uc_matrix = (unsigned char**)(TIGHTLY_COUPLED_DATA_MEM_BASE);
	if (orig_uc_matrix == NULL || dest_uc_matrix == NULL) printf("NULL ptr found\n");
	int i = 0;
    for (; i < 512; ++i) {
      dest_uc_matrix[i] = (unsigned char*)malloc(512*sizeof(unsigned char));
      if (dest_uc_matrix[i] == NULL) printf("NULL ptr found\n");
    }
	printf("Read OK\n");

	alt_timestamp_start(); // inicializamos los timers
	alt_u32 timer1 = alt_timestamp();
	int kernel[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
	int accum = 0;
	i = 1;
	int j = 1;
	for(; i < 511; ++i) {
		j = 1;
		for(; j < 511; ++j) {
			accum = 0;
			accum = accum + (kernel[0] * orig_uc_matrix[i - 1][j - 1]);
			accum = accum + (kernel[1] * orig_uc_matrix[i - 1][j]);
			accum = accum + (kernel[2] * orig_uc_matrix[i - 1][j + 1]);
			accum = accum + (kernel[3] * orig_uc_matrix[i][j - 1]);
			accum = accum + (kernel[4] * orig_uc_matrix[i][j]);
			accum = accum + (kernel[5] * orig_uc_matrix[i][j + 1]);
			accum = accum + (kernel[6] * orig_uc_matrix[i + 1][j - 1]);
			accum = accum + (kernel[7] * orig_uc_matrix[i + 1][j]);
			accum = accum + (kernel[8] * orig_uc_matrix[i + 1][j + 1]);
			if (accum > 255) accum = 255;
			else if (accum < 0) accum = 0;
			dest_uc_matrix[i][j] = (unsigned char)accum;
		}
	}
	alt_u32 timer_result = alt_timestamp() - timer1;
	printf("Ciclos de aplicación del algoritmo: %u \n", timer_result);
	printf("Nanosegundos de aplicación del algoritmo: %u \n", (timer_result) * 20);
	printf("Segundos de aplicación del algoritmo: %f \n", (double)((timer_result) * 20) / 100000000 );


	if (pgmwrite2(path_out, height_photo, width_photo, dest_uc_matrix, NULL, 1) == 0) printf("Written OK\n");
	else { printf("ERROR\n"); }


//	for (i = 0; i < 512; ++i) {
//		(void*)orig_uc_matrix[i];
//		free(orig_uc_matrix[i]);
//		free((void*)dest_uc_matrix[i]);
//	}
//	free(orig_uc_matrix);
//	free(dest_uc_matrix);
	return 0;
}
