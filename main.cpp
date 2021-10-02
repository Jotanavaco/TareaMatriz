#include <iostream>
#include "FileSystem.hpp"

int main() {
  FileSystem *fileSystem = new FileSystem();
  fileSystem->open("holaMundo");
}