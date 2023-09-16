# 3rd_computer_architecture

En esta carpeta trabajaremos con el simulador de máquinas superescalares SIMDE de la Universidad de La Laguna

SIMDE: https://etsiiull.github.io/SIMDE/ 

Tenemos un código fuente en C++ el cuál debemos pasar a ensamblador, y realizarle el máximo de optmizaciones posibles, el código es el siguiente:

// row , col - Coordenadas del píxel central para el cálculo
for (int row = 1; row < height - 1; row++) {

    for (int col = 1; col < width - 1; col++) {

        double magX = 0.0;

        for (int a = 0; a < 3; a++) {

            for (int b = 0; b < 3; b++) {

                magX += pImg[(row - 1 + a) * Wp + col - 1 + b] * kernelx[a][b];  

            }

        }

        resultImg[row * Wp + col] = magX;  

   }

}
