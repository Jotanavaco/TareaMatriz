#include <string>
#include <iostream>
using namespace std ;

class Archivo {
  // atributos de clase
  string nombreArchivo;
  bool estaAbierto;
  int tamanoArchivo;
  bool existeArchivo;
  string datos;


  // métodos de clase

  //Constructor por defecto
  Archivo::Archivo() {
    this->nombreArchivo = " ";
    this->estaAbierto = false;
    this->tamanoArchivo = 0;
    this->existeArchivo = false;
    this->datos = " ";
  }
}