#include <string>
#include <iostream>
using namespace std ;
const int T_DIRECTORY = 3;
const int T_MEMORY_UNIT = 16;
const int FREE_BLOCK = -2;
const int FREE_MEMORY_NOT_FOUND = -3;
const int BLOCK_NOT_FOUND = -4;

// contains the internal metadata of the file
struct Directory {
  int memoryBlock;
  string fileName;
  time_t lastUpdateDate;
  bool isOpen;
  string userCode;
};

class FileSystem {
  // class atributes
  private:
  char *memoryUnit;
  Directory *directory;
  int *fatTable;
  int memoryUnitIndex;
  // class methods
  public:
  // class default Constructor, initialices all the values
  FileSystem() {
    memoryUnitIndex = 0;
    memoryUnit = new char[T_MEMORY_UNIT];
    directory = new Directory[T_DIRECTORY] ;
    fatTable = new int[T_MEMORY_UNIT] ;
    // fill the blank spaces with the deafault values
    for (int index = 0; index < T_MEMORY_UNIT; index++) {
      memoryUnit[index]= '_' ;
      fatTable[index]= FREE_BLOCK;
    }
  }
  // class default destructor
  ~FileSystem() {
    delete[] memoryUnit;
    delete[] directory;
    delete[] fatTable;
  }
  // basic class methods
  // comentar PAULA
  void create() {
  }
  // comentar JUAN
  void open() {
  }
  // comentar JUAN
  void write() {
  }
  // comentar BRYAN
  void append() {
  }
    // comentar BRYAN
  void remove() {
  }
  // comentar JOHN
  void read() {
  }
  // comentar JHON
  void close() {
  }
  // methods call by basic class methods
  /* returns true if the name asked exists in 
    the directory, otherwise returns false*/
  bool existingName(string fileName) {
    return true;
  }
  /* returns the first block of memory from the directory,
    otherwise returns BLOCK_NOT_FOUND*/
  int searchFirstBlock(string fileName) {
    return 0;
  }
  /* returns the index of a free block of memory,
    otherwise returns FREE_MEMORY_NOT_FOUND*/
  int searchFreeBlock() {
    return 0;
  }
  /* prints the directory*/
  void printDirectory() {
  }
  /* prints the file*/
  void printFile(string name) {
  }
};