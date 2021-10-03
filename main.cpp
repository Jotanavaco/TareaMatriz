#include <iostream>
#include "FileSystem.hpp"

int main() {
  FileSystem *fileSystem = new FileSystem();

  //Tests 
  fileSystem->create("prueba.txt", "Admin");
  fileSystem->open("prueba.txt");
  
  fileSystem->create("prueba2.txt", "Admin");
  fileSystem->create("prueba3.txt", "Admin");
  fileSystem->create("prueba4.txt", "Admin");

  fileSystem->open("prueba2.txt");
  fileSystem->write("prueba2.txt", 'h');
  fileSystem->write("prueba2.txt", 'o');
  fileSystem->write("prueba2.txt", 'l');
  fileSystem->write("prueba2.txt", 'a');

  //Call to impressions
  fileSystem->printDirectory();
  fileSystem->printUnitAndFatMemory();
}