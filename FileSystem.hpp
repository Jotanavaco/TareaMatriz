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
  /* returns the index if the name asked exists in 
    the directory, otherwise returns BLOCK_NOT_FOUND*/
  int existingName(string fileName) {
    int directoryIndex = BLOCK_NOT_FOUND;
    bool exists = false;
    int index = 0;
    while (!exists && index < T_DIRECTORY) {
      if (directory[index].fileName == fileName) {
        directoryIndex = index;
        exists = true;
        index = T_DIRECTORY;
      }
      index++;
    }
    return directoryIndex;
  }
  /* returns the first block of memory from the directory,
    otherwise returns BLOCK_NOT_FOUND*/
  int searchFirstBlock(string fileName) {
    // if the block exists
    int directoryIndex = existingName(fileName);
    int block = BLOCK_NOT_FOUND;
    if (directoryIndex != BLOCK_NOT_FOUND) {
      block = directory[block].block;
    }
    return block;
  }
  /* returns the index of a free block of memory in the fatTable,
    otherwise returns FREE_MEMORY_NOT_FOUND*/
  int searchFreeBlock() {
    int freeBlockIndex = FREE_MEMORY_NOT_FOUND;
    int index = 0;
    bool blockFound = false;
    while (index < T_MEMORY_UNIT && (blockFound == false)) {
      if (fatTable[index] == FREE_BLOCK) {
        freeBlockIndex = index;
        blockFound = true;
        index = T_MEMORY_UNIT;
      }
      index++;
    }
    return freeBlockIndex;
  }
  /* prints the directory*/
  void printDirectory() {
  }
  /* prints the file*/
  void printFile(string name) {
  }
};