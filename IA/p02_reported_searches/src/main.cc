/**
 * Universidad de La Laguna (ULL)
 * Escuela Superior de Ingenieria y Tecnologia (ESIT)
 * Grado en Ingenieria Informatica
 * Asignatura: Inteligencia Artificial (IA)
 * Curso: 3º
 * Practica 2: Búsquedas Informadas
 * @author Cristopher M. Afonso Mora (alu0101402031@ull.edu.es)
 * @author Rebeca Rodríguez Rodríguez (alu0101394763@ull.edu.es)
 * @date 06/11/2022 (dd/mm/yyyy)
 *
 * @file main.cc (programa cliente)
 * @brief Programa que dado solo un archivo de texto plano, es capaz de generar
 * una matriz de dos dimensiones y mostrar paso a paso el camino más óptimo
 * entre dos puntos de la matriz siguiendo el algoritmo de Primero el Mejor ó A*
 *
 * Si quiere saber mas detalles sobre como usar el programa ejecutable,
 * ejecute: main --help
 * para mas informacion.
 *
 * @bug No hay bugs conocidos
 *
 * Referencias:
 * @see
 *
 * @version 1.0
 * @copyright Copyright (c) 2022
 * @brief Historial de Revisiones
 * 06/11/22 - Creacion (primera version) del codigo:
 *            Solo he creado el archivo, le he puesto el comentario de
 *            cabecera y declarado su include.
 */

#include "../include/functions.h"

int main(int argc, char** argv) {
  Matrix environment;
  Usage(argc, argv, environment);
  environment.FindOptimalWay();
}