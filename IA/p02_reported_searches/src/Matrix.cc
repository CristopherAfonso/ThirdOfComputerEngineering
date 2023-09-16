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
 * @file Matrix.cc
 * @brief Donde se desarrollan las funciones de la clase Matrix contenida
 * en el archivo de cabecera Matrix.h
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

#include "../include/Matrix.h"

Matrix::Matrix() { matrix_.resize(0); }

Matrix::Matrix(const int& height, const int& width, const Point& start,
               const Point& final, const char& heuristic_function = 'e',
               const double& seconds_to_wait = 1.00) {
  matrix_.resize(height + 2);
  for (int i = 0; i < int(matrix_.size()); ++i) {
    matrix_[i].resize(width + 2);
  }
  for (int i = 0; i < int(matrix_.size()); ++i) {
    for (int j = 0; j < int(matrix_[i].size()); ++j) {
      matrix_[i][j].setX(i);
      matrix_[i][j].setY(j);
      matrix_[i][j].setPoint("   ");
    }
  }
  matrix_[start.getX()][start.getY()].setStart(1);
  matrix_[start.getX()][start.getY()].setPoint(" S ");
  matrix_[final.getX()][final.getY()].setFinal(1);
  matrix_[final.getX()][final.getY()].setPoint(" F ");
  initial_pos_ = start;
  initial_pos_.setStart(true);
  initial_pos_.setPoint(" S ");
  final_pos_ = final;
  final_pos_.setFinal(true);
  final_pos_.setPoint(" F ");
  if (heuristic_function == 'e' || heuristic_function == 'm') {
    heuristic_function_ = heuristic_function;
  }
  seconds_to_wait_ = seconds_to_wait;
}

std::vector<std::vector<Point>> Matrix::getMatrix() const { return matrix_; };

char Matrix::getHeuristicFunction() const { return heuristic_function_; };

double Matrix::getSecondsToWait() const { return seconds_to_wait_; }

Point Matrix::getInitialPos() const { return initial_pos_; }

Point Matrix::getFinalPos() const { return final_pos_; }

void Matrix::setMatrix(const std::vector<std::vector<Point>>& matrix) {
  matrix_ = matrix;
}

void Matrix::setHeuristicFunction(const char& HeuristicFunction) {
  if (HeuristicFunction == 'e' || HeuristicFunction == 'm')
    heuristic_function_ = HeuristicFunction;
}

void Matrix::setSecondsToWait(const double& seconds) {
  seconds_to_wait_ = seconds;
}

void Matrix::setInitialPos(Point initial_pos) {
  initial_pos.setStart(true);
  initial_pos.setX(initial_pos_.getX() % matrix_.size());
  initial_pos.setY(initial_pos.getY() % (matrix_[initial_pos_.getX()].size()));
  matrix_[initial_pos_.getX()][initial_pos_.getY()].setStart(false);
  matrix_[initial_pos_.getX()][initial_pos_.getY()].setFinal(
      initial_pos.getFinal());
  initial_pos_ = initial_pos;
}

void Matrix::setFinalPos(Point final_pos) {
  final_pos.setStart(true);
  final_pos.setX(final_pos_.getX() % matrix_.size());
  final_pos.setY(final_pos.getY() % (matrix_[final_pos_.getX()].size()));
  matrix_[final_pos_.getX()][final_pos_.getY()].setStart(false);
  matrix_[final_pos_.getX()][final_pos_.getY()].setFinal(final_pos.getFinal());
  final_pos_ = final_pos;
}

/**
 * @brief Metodo que aplica la funcion heuristica y devuelve el resultado de
 * una de las dos funciones heuristicas que se ofrecen, entre ellas tenemos la
 * euclidiana y la manhathan
 *
 * @param current_pos posición actual donde se halla el coche
 * @param final_pos posicin donde el coche debe llegar
 * @return double resultado a devolver que contiene el resultado de la funcion
 * heurisitica elegida
 */
double Matrix::HeuristicFunction(const Point& current_pos,
                                 const Point& final_pos) const {
  /// Euclidean
  if (heuristic_function_ == 'e') {
    return sqrt(pow(current_pos.getX() - final_pos.getX(), 2) +
                pow(current_pos.getY() - final_pos.getY(), 2));
    /// Manhattan
  } else if (heuristic_function_ == 'm')
    return (abs(current_pos.getX() - final_pos.getX()) +
            abs(current_pos.getY() - final_pos.getY()));
  return 0;
}

void Matrix::FindOptimalWay() {
  using std::cout;
  const std::string kCar{"🚑 "};
  const std::string kRight{" → "};
  const std::string kLeft{" ← "};
  const std::string kUp{" ↑ "};
  const std::string kDown{" ↓ "};
  Point current_pos = initial_pos_;
  current_pos.setStart(false);
  current_pos.setPoint(kCar);
  cout << *this;
  double lower_cost{-1.00};
  char next_dir{' '};
  int count_steps{0};
  while (!current_pos.getFinal()) {
    if (current_pos.getX() > 0) {
      Point aux_point = current_pos;
      aux_point.setX(aux_point.getX() - 1);
      if ((lower_cost < 0.00) ||
          ((lower_cost > 0.00) &&
           (lower_cost > this->HeuristicFunction(aux_point, final_pos_)))) {
        lower_cost = this->HeuristicFunction(aux_point, final_pos_);
        next_dir = 'u';  ///< up direction
      }
    }

    if (current_pos.getX() < int(matrix_.size() - 1)) {
      Point aux_point = current_pos;
      aux_point.setX(aux_point.getX() + 1);
      if ((lower_cost < 0.00) ||
          ((lower_cost > 0.00) &&
           (lower_cost > this->HeuristicFunction(aux_point, final_pos_)))) {
        lower_cost = this->HeuristicFunction(aux_point, final_pos_);
        next_dir = 'd';  ///< down direction
      }
    }

    if (current_pos.getY() > 0) {
      Point aux_point = current_pos;
      aux_point.setY(aux_point.getY() - 1);
      if ((lower_cost < 0.00) ||
          ((lower_cost > 0.00) &&
           (lower_cost > this->HeuristicFunction(aux_point, final_pos_)))) {
        lower_cost = this->HeuristicFunction(aux_point, final_pos_);
        next_dir = 'l';  ///< left direction
      }
    }

    if (current_pos.getY() < int(matrix_.size() - 1)) {
      Point aux_point = current_pos;
      aux_point.setY(aux_point.getY() + 1);
      if ((lower_cost < 0.00) ||
          ((lower_cost > 0.00) &&
           (lower_cost > this->HeuristicFunction(aux_point, final_pos_)))) {
        lower_cost = this->HeuristicFunction(aux_point, final_pos_);
        next_dir = 'r';  ///< right direction
      }
    }

    if (next_dir == 'r') {
      matrix_[current_pos.getX()][current_pos.getY()].setPoint(kRight);
      current_pos.setY(current_pos.getY() + 1);
      current_pos.setFinal(
          matrix_[current_pos.getX()][current_pos.getY()].getFinal());
      matrix_[current_pos.getX()][current_pos.getY()].setPoint(kCar);
    }
    if (next_dir == 'l') {
      matrix_[current_pos.getX()][current_pos.getY()].setPoint(kLeft);
      current_pos.setY(current_pos.getY() - 1);
      current_pos.setFinal(
          matrix_[current_pos.getX()][current_pos.getY()].getFinal());
      matrix_[current_pos.getX()][current_pos.getY()].setPoint(kCar);
    }
    if (next_dir == 'u') {
      matrix_[current_pos.getX()][current_pos.getY()].setPoint(kUp);
      current_pos.setX(current_pos.getX() - 1);
      current_pos.setFinal(
          matrix_[current_pos.getX()][current_pos.getY()].getFinal());
      matrix_[current_pos.getX()][current_pos.getY()].setPoint(kCar);
    }
    if (next_dir == 'd') {
      matrix_[current_pos.getX()][current_pos.getY()].setPoint(kDown);
      current_pos.setX(current_pos.getX() + 1);
      current_pos.setFinal(
          matrix_[current_pos.getX()][current_pos.getY()].getFinal());
      matrix_[current_pos.getX()][current_pos.getY()].setPoint(kCar);
    }

    std::this_thread::sleep_for(std::chrono::seconds(int(seconds_to_wait_)));
    this->Clear();
    this->ShowActualPos(current_pos);

    lower_cost = -1.00;
    next_dir = ' ';
    ++count_steps;
  }
  cout << "\n\n";
  cout << "Pasos realizados: " << count_steps;
  cout << "\n\n";
}

Matrix& Matrix::operator=(const Matrix& matrix) {
  matrix_ = matrix.matrix_;
  initial_pos_ = matrix.initial_pos_;
  final_pos_ = matrix.final_pos_;
  heuristic_function_ = matrix.heuristic_function_;
  seconds_to_wait_ = matrix.seconds_to_wait_;
  return *this;
}

std::ostream& operator<<(std::ostream& out, const Matrix& matrix) {
  for (int i = 0; i < int(matrix.matrix_.size()); ++i) {
    if (i == 0 || i == int(matrix.matrix_.size() - 1)) {
      out << "+";
      for (int j = 0; j < int(matrix.matrix_[i].size()); ++j) {
        if ((j == 0) || (j == int(matrix.matrix_[i].size() - 1))) continue;
        out << "———";
      }
      out << "+\n";
    } else {
      for (int j = 0; j < int(matrix.matrix_[i].size()); ++j) {
        if (j == 0 || j == int(matrix.matrix_[i].size() - 1))
          out << '|';
        else
          out << matrix.matrix_[i][j].getPoint();
      }
      out << std::endl;
    }
  }

  return out;
}

void Matrix::ShowActualPos(const Point& act_pos) {
  std::cout << *this;
}

/**
 * @brief función que nos permite limpiar el buffer de la terminal sin importar
 * el sistema operativo en el que ejecutemos el programa
 *
 */
void Matrix::Clear() {
#if defined _WIN64 || defined _WIN32
  system("cls");
  // clrscr(); // including header file : conio.h
#elif defined(__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
  // system("clear");
  std::cout << u8"\033[2J\033[1;1H";  // Using ANSI Escape Sequences
#elif defined(__APPLE__)
  system("clear");
#endif
}