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
 * @file Matrix.h
 * @brief Donde se aloja la declaración de la clase Matrix y la declaracion
 * de sus metodos. La clase Matrix es una clase que se encargara de simular el
 * entorno donde desarrollaremos la práctica, dentro de este entorno, tendremos
 * dos puntos, el inicio y el final del trayecto
 *
 * @bug No hay bugs conocidos
 *
 * Referencias: Enlaces de Interes
 * @see
 *
 * @version 1.0
 * @brief Historial de Revisiones
 * @copyright Copyright (c) 2022
 * 11/06/22 - Creacion (primera version) del codigo:
 *            Solo he creado el archivo, le he puesto el comentario de
 *            cabecera y declarado los #include necesarios.
 * 11/06/22 - Archivo terminado.
 */

#ifndef CLASS_MATRIX
#define CLASS_MATRIX

#include <cmath>
#include <iostream>
#include <vector>
#include <thread>
#include <ctime>

#include "Point.h"

class Matrix {
 public:
  Matrix();
  Matrix(const int&, const int&, const Point&, const Point&, const char&,
         const double&);
  std::vector<std::vector<Point>> getMatrix() const;
  char getHeuristicFunction() const;
  double getSecondsToWait() const;
  Point getInitialPos() const;
  Point getFinalPos() const;
  void setMatrix(const std::vector<std::vector<Point>>&);
  void setHeuristicFunction(const char& HeuristicFunction);
  void setSecondsToWait(const double& seconds);
  void setInitialPos(Point initial_pos);
  void setFinalPos(Point final_pos);
  double HeuristicFunction(const Point& current_pos,
                           const Point& final_pos) const;
  void FindOptimalWay();

  Matrix& operator=(const Matrix&);
  friend std::ostream& operator<<(std::ostream&, const Matrix&);

 private:
  std::vector<std::vector<Point>> matrix_;
  Point initial_pos_;
  Point final_pos_;
  char heuristic_function_;
  double seconds_to_wait_;

  void ShowActualPos(const Point& act_pos);
  void Clear();
};
#endif