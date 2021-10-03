#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;

const int T_DIRECTORY = 3;
const int T_MEMORY_UNIT = 16;
const int FREE_BLOCK = -2;
const int FREE_MEMORY_NOT_FOUND = -3;
const int BLOCK_NOT_FOUND = -4;
const int RESERVED_BLOCK = -5;

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
    for (int index = 0; index < T_DIRECTORY; index++) {
      directory[index].memoryBlock = FREE_BLOCK;
    }
  }

  // class default destructor
  ~FileSystem() {
    delete[] memoryUnit;
    delete[] directory;
    delete[] fatTable;
  }

  // basic class methods
  // Check if the file already exists, if not create it if you have the permission
  void create(string fileName, string userCode) {
	  int exists = existingName(fileName);

    if (exists == BLOCK_NOT_FOUND) {
      // Si tiene el permiso, falta definir
      if(userCode == "Admin") {
        int directoryIndex = searchFreeDirectoryBlock();
        int fatIndex = searchFreeFatBlock();

        if (directoryIndex != FREE_MEMORY_NOT_FOUND &&  fatIndex != FREE_MEMORY_NOT_FOUND) {
          directory[directoryIndex].memoryBlock = fatIndex; 
          directory[directoryIndex].fileName = fileName;
          directory[directoryIndex].lastUpdateDate = std::time(nullptr);
          directory[directoryIndex].isOpen = false;

          //The file is created empty, the FAT block is reserved
          fatTable[fatIndex] = RESERVED_BLOCK;

          directoryIndex++;
          fatIndex++;
          cout << "File " +fileName+ " created\n";
        } else {
          cout << "No free memory found\n";
        }
      } else {
        cout << "The user does not have permissions\n";
      }
    } else {
      cout << "File already exists\n";
    }
  }

  /*if the file exists and it is not open, the methods will open it*/
  void open(string fileName) {
    // cout << "Estoy en open\n";
    // look if the file exists
    int fileDirectoryIndex = existingName(fileName);
    if (fileDirectoryIndex != BLOCK_NOT_FOUND) {
      // if the file is already open
      if(directory[fileDirectoryIndex].isOpen == true) {
        cout << "File " +fileName+ " already open\n";
      } else {
        directory[fileDirectoryIndex].isOpen = true;
        cout << "The file " +fileName+ " was opened\n";
      }
    } else {
      cout << "File not found\n";
    }
  }
  /*if the file exists and it is open it search an empty 
  block in the fat memory to write the data*/
  void write(string fileName, char data) {
    // check if the file exists and if it is open
    int fileDirectoryIndex = existingName(fileName);
    if (fileDirectoryIndex != BLOCK_NOT_FOUND 
      && directory[fileDirectoryIndex].isOpen == true) {
        int counter = 0;
        int index = directory[fileDirectoryIndex].memoryBlock;
        // if it is the first data of the file
        if (fatTable[index] == FREE_BLOCK || fatTable[index] == RESERVED_BLOCK) {
          memoryUnit[index] = data;
          fatTable[index] = -1;
          cout << "Data saved on slot: " << index << endl;
        }  else {
          // if it is not the firs data of the file
          while (counter < T_MEMORY_UNIT) {
            if (fatTable[index] != -1) {
              index = fatTable[index];
            } else {
              int freeBlockIndex = searchFreeFatBlock();
              if (freeBlockIndex != FREE_MEMORY_NOT_FOUND) {
                fatTable[index] = freeBlockIndex;
                memoryUnit[freeBlockIndex] = data;
                fatTable[freeBlockIndex] = -1;
                counter = T_MEMORY_UNIT;
              } else {
                cout << "The data could not be stored, free memory not found\n";
              }
            }
            counter++;
          }
        }
    } else if(fileDirectoryIndex == BLOCK_NOT_FOUND) {
      cout << "File not found\n";
    } else {
      cout << "The file is not open\n";
    }
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
    // cout << "Estoy en existingName\n";
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

  /* returns the first block of the file from the directory,
    otherwise returns BLOCK_NOT_FOUND*/
  int searchFirstFileBlock(string fileName) {
    // if the block exists
    int directoryIndex = existingName(fileName);
    int block = BLOCK_NOT_FOUND;
    if (directoryIndex != BLOCK_NOT_FOUND) {
      block = directory[block].memoryBlock;
    }
    return block;
  }
  /* returns the index of a free block of memory in the fatTable,
    otherwise returns FREE_MEMORY_NOT_FOUND*/
  int searchFreeFatBlock() {
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
  /* returns the index of a free block of memory in the directory,
    otherwise returns FREE_MEMORY_NOT_FOUND*/
  int searchFreeDirectoryBlock() {
    int freeBlockIndex = FREE_MEMORY_NOT_FOUND;
    int index = 0;
    bool blockFound = false;
    
    while (index < T_DIRECTORY && (blockFound == false)) {
      if (directory[index].memoryBlock == FREE_BLOCK) {
        freeBlockIndex = index;
        blockFound = true;
        index = T_DIRECTORY;  //le da un fin al ciclo
      }
      index++;
    }
    return freeBlockIndex;
  }
  /* prints the directory*/
  void printDirectory() {
    int directoryIndex = 0;

    cout << "\nFull directory\n";
    while (directoryIndex < T_DIRECTORY) {
      cout << "Block: " << directory[directoryIndex].memoryBlock << "\n";
      cout << "File name: " << directory[directoryIndex].fileName << "\n";
      cout << "Date: " << std::put_time(std::gmtime(&directory[directoryIndex].lastUpdateDate), "%c") << '\n';
      cout << '\n';

      directoryIndex++;
    }
  }
  /* prints the memory Unit and fat Table*/
  void printUnitAndFatMemory() {
    cout << "\nFull unit and fat memory\n";
    for (int index = 0; index < T_MEMORY_UNIT; index++) {
      cout << "Index = " << index << "\t";
      cout << "UM = " << memoryUnit[index] << "\t";
      cout <<  "FM = " << fatTable[index] << endl;
    }
  }
  /* prints the file*/
  void printFile(string name) {
    
  }
};