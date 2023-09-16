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
 * @file Point.h
 * @brief Donde se aloja la declaración de la clase Point y la declaracion
 * de sus metodos. La clase Point es una clase que gestiona lo que es una
 * ubicacion dentro de nuestro entonrno de la aplicacion, los puntos inicio
 * y final del entorno son objetos de esta clase
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

#ifndef CLASS_POINT
#define CLASS_POINT

#include <string>

class Point {
 public:
  Point(void);
  Point(const int&, const int&, const bool&, const bool&, const std::string&);

  int getX() const { return x_; }
  int getY() const { return y_; }
  bool getStart() const { return start_; }
  bool getFinal() const { return final_; }
  std::string getPoint() const { return point_; }
  
  void setX(const int& x) { x_ = x; }
  void setY(const int& y) { y_ = y; }
  void setStart(const bool& start) { start_ = start; }
  void setFinal(const bool& end) { final_ = end; }
  void setPoint(const std::string& point) { point_ = point; }
  
  Point& operator=(const Point& point);

 private:
  int x_;
  int y_;
  bool start_;
  bool final_;
  std::string point_;
};
#endif