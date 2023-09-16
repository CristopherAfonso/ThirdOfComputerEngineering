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
 * @file functions.cc
 * @brief Donde se aloja el desarrollo y la implementación de las funciones
 * de nuestro archivo cliente.
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

#include "../include/functions.h"

/**
 * @brief Analiza las entradas del programa, comprueba que son correctas, y
 * devuelve por referencia en la variable environment un objeto Matrix que es
 * el entorno de nuestra practica
 *
 * @param argc numero de argumentos del programa
 * @param argv conjunto de argumentos del programa
 * @param environment variable Matrix donde si las entradas del programa son
 * correctas, se pondrá el entorno en el que de debemos trabajar
 */
void Usage(int& argc, char** argv, Matrix& environment) {
  using std::cerr;
  using std::cout;
  try {
    if (argc != 2) throw 0;
    std::string second_parameter = argv[1];
    if (second_parameter == "--help") {
      cout << "Given a matrix, a start point and a final point, this "
              "program calculates the optimal way to get to the final "
              "point from the start one.\n";
      cout << "This is how the program should be executed:\n";
      cout << "  ./IA-P02 <input file>\n";
      cout << "The input file contains the size of the matrix, the start "
              "point and the final point.\n";
      cout << "As an example, here's the format that each of the input "
              "file lines should have: 10 10 (1, 1) (9, 9)\n";
      cout << "Which means that the matrix height and width are both 10, "
              "the start point is (1, 1), and the final point is (9, 9).\n";
    } else {
      std::vector<std::string> input_lines;  ///< Matrices to test
      std::string line;
      std::ifstream input;
      input.open(argv[1]);
      if (!input.is_open())
        throw 1;
      else {
        while (getline(input, line)) input_lines.push_back(line);
      }
      std::string line_format =
          "^([0-9]+)\\s([0-9]+)\\s\\(([0-9]+),\\s([0-9]+)\\)\\s\\(([0-9]+),\\s("
          "[0-9]+)\\)\\s([0-9]+(\\.[0-9]+)?)\\s(e|m)$";
      std::regex regex(line_format);
      std::smatch matches;
      for (int i = 0; i < int(input_lines.size()); ++i) {
        if (std::regex_search(input_lines[0], matches, regex)) {
          if (stoi(matches.str(3)) > stoi(matches.str(1)) ||
              stoi(matches.str(5)) > stoi(matches.str(1)))
            throw 3;
          if (stoi(matches.str(4)) > stoi(matches.str(2)) ||
              stoi(matches.str(6)) > stoi(matches.str(2)))
            throw 3;
          Point start_point(std::stoi(matches.str(3)),
                            std::stoi(matches.str(4)), 1, 0, "S");
          Point final_point(std::stoi(matches.str(5)),
                            std::stoi(matches.str(6)), 0, 1, "F");
          Matrix matrix(std::stoi(matches.str(1)), std::stoi(matches.str(2)),
                        start_point, final_point, matches.str(9)[0],
                        atof(matches.str(7).c_str()));
          environment = matrix;  ///< se devuelve la matriz
        } else {
          throw 2;
        }
      }
    }
  } catch (int exception) {
    switch (exception) {
      case 0:
        cerr << "Error - Incorrect parameter list. Try './IA-P02 --help'\n";
        break;
      case 1:
        cerr << "Error - Could not open the input file. Please, try again.\n";
        break;
      case 2:
        cerr << "Error - Incorrect format in the input file.\n";
        break;
      case 3:
        cerr << "Error - Incorrect definition of the points.\n";
        break;
    }
    exit(EXIT_FAILURE);
  }
}
