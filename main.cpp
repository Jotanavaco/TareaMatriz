#include <iostream>
#include "FileSystem.hpp"

int main() {
  FileSystem *fileSystem = new FileSystem();

  //Creacion de archivos
  fileSystem->create("prueba1.txt", 'D', "group");
  fileSystem->create("prueba2.txt", 'D', "group");
  fileSystem->create("prueba3.txt", 'D', "group");
  fileSystem->create("prueba4.txt", 'D', "group");

  //Abrir archivos
  fileSystem->open("prueba1.txt");
  fileSystem->open("prueba2.txt");
  fileSystem->open("prueba3.txt");
  fileSystem->open("prueba4.txt");

  //Se escribe y se agrega en el archivo
  fileSystem->append("prueba2.txt", 'h', 'D', "group");
  fileSystem->append("prueba2.txt", 'o', 'D', "group");
  fileSystem->append("prueba2.txt", 'l', 'D', "group");
  fileSystem->append("prueba2.txt", 'a', 'D', "group");
  fileSystem->write("prueba2.txt", 'H', -6, 'D', "group");

  fileSystem->append("prueba3.txt", 'M', 'D', "group");
  fileSystem->append("prueba3.txt", 'U', 'D', "group");
  fileSystem->append("prueba3.txt", 'N', 'D', "group");
  fileSystem->append("prueba3.txt", 'D', 'D', "group");
  fileSystem->append("prueba3.txt", 'O', 'D', "group");

  //Lectura de archivos
  string datos = fileSystem->read("prueba2.txt", 'D', "group");
  cout << "\nData: " << datos << "\n";

  //Eliminar archivos
  fileSystem->remove("prueba1.txt", 'A', "user");
  fileSystem->remove("prueba2.txt", 'F', "user");

  //Cerrar archivos
  fileSystem->close("prueba1.txt");
  fileSystem->close("prueba2.txt");
  fileSystem->close("prueba3.txt");

  //Llamar las impresiones
  fileSystem->printDirectory();
  fileSystem->printUnitAndFatMemory();
  fileSystem->printFile("prueba1.txt");
}