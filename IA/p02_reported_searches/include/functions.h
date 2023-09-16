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
 * @file functions.h
 * @brief Donde se aloja la declaración de las funciones del archivo cliente
 * de nuestro programa.
 *
 * @bug No hay bugs conocidos
 *
 * Referencias: Enlaces de Interes
 * @see
 *
 * @version 1.0
 * @brief Historial de Revisiones
 * 06/11/22 - Creacion (primera version) del codigo:
 *            Solo he creado el archivo, le he puesto el comentario de
 *            cabecera y declarado los #include necesarios.
 * 06/11/22 - Archivo terminado.
 */

#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <vector>

#include "Matrix.h"

void Usage(int& argc, char** argv, Matrix& environment);
void Clear();