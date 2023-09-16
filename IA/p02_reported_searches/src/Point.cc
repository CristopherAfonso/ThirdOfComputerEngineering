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
 * @file Point.cc
 * @brief Donde se desarrollan las funciones de la clase Point contenida
 * en el archivo de cabecera Point.h
 *
 * @bug No hay bugs conocidos
 *
 * Referencias: Enlaces de Interes
 * @see
 *
 * @version 1.0
 * @brief Historial de Revisiones
 * @copyright Copyright (c) 2022
 * 06/11/22 - Creacion (primera version) del codigo:
 *            Solo he creado el archivo, le he puesto el comentario de
 *            cabecera y declarado los #include necesarios.
 * 06/11/22 - Archivo terminado.
 */

#include "../include/Point.h"

Point::Point(void) : x_(0), y_(0), start_(0), final_(0), point_(" ") {}

Point::Point(const int& x, const int& y, const bool& start, const bool& final,
             const std::string& point)
    : x_(x), y_(y), start_(start), final_(final), point_(point) {}

Point& Point::operator=(const Point& point) {
  x_ = point.x_;
  y_ = point.y_;
  start_ = point.start_;
  final_ = point.final_;
  point_ = point.point_;
  return *this;
}