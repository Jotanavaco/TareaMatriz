#include <string>
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;

const int T_DIRECTORY = 3;
const int T_MEMORY_UNIT = 16;
const int ARRAY_SIZE = 6;
const int END_OF_FILE = -1;
const int FREE_BLOCK = -2;
const int FREE_MEMORY_NOT_FOUND = -3;
const int BLOCK_NOT_FOUND = -4;
const int RESERVED_BLOCK = -5;
const int BEGINNING_CURSOR = -6;
const int END_CURSOR = -7;

// contains the internal metadata of the file
struct Directory {
  int memoryBlock;
  string fileName;
  time_t lastUpdateDate;
  bool isOpen;
  char user;
  string permission;
};

class FileSystem {
  // class atributes
  private:
  char *memoryUnit;
  Directory *directory;
  int *fatTable;
  int memoryUnitIndex;
  // permissions User arrays
  char userRead[ARRAY_SIZE] = {'B', 'C', 'D', 'E', 'F'};
  char userWrite[ARRAY_SIZE] = {'D', 'E', 'F'};
  char userRemove[ARRAY_SIZE] = {'E', 'F'};
  
  // permissions Group arrays
  char groupRead[ARRAY_SIZE] = {'B', 'C', 'D', 'E', 'F'};
  char groupWrite[ARRAY_SIZE] = {'C', 'D', 'E', 'F'};
  char groupRemove[ARRAY_SIZE] = {'F'};

  // permissions All arrays
  char allRead[ARRAY_SIZE] = {'B', 'C', 'D', 'E', 'F'};
  char allWrite[ARRAY_SIZE] = {'D', 'E', 'F'};
  char allRemove[ARRAY_SIZE] = {'F'};

  // class methods
  public:
  // class default Constructor, initialices all the values
  FileSystem() {
    memoryUnitIndex = 0;
    memoryUnit = new char[T_MEMORY_UNIT];
    directory = new Directory[T_DIRECTORY] ;
    fatTable = new int[T_MEMORY_UNIT];
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
  void create(string fileName, char user, string permission) {
	  int exists = existingName(fileName);
    bool havePermission = checkPermission(user, permission, 'R');

    if (exists == BLOCK_NOT_FOUND) {
      // Si tiene el permiso, falta definir
      if(havePermission) {
        int directoryIndex = searchFreeDirectoryBlock();
        int fatIndex = searchFreeFatBlock();

        if (directoryIndex != FREE_MEMORY_NOT_FOUND &&  fatIndex != FREE_MEMORY_NOT_FOUND) {
          directory[directoryIndex].memoryBlock = fatIndex; 
          directory[directoryIndex].fileName = fileName;
          directory[directoryIndex].lastUpdateDate = time(nullptr);
          directory[directoryIndex].isOpen = false;

          //The file is created empty, the FAT block is reserved
          fatTable[fatIndex] = RESERVED_BLOCK;

          directoryIndex++;
          fatIndex++;
          cout << "File " + fileName + " created\n";
        } else {
          cout << "\nNo free memory found for "  +fileName+ " in directory\n";
        }
      }
    } else {
      cout << "\nFile " + fileName + " already exists\n";
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
        cout << "\nFile " + fileName + " already open\n";
      } else {
        directory[fileDirectoryIndex].isOpen = true;
        cout << "The file " + fileName + " was opened\n";
      }
    } else {
      cout << "\nFile " + fileName + " not found \n";
    }
  }

  /*if the file exists and it is open it search an empty 
  block in the fat memory to write the data*/
  void append(string fileName, char data, char user, string permission) {
    bool havePermission = checkPermission(user, permission, 'W');
    int fileDirectoryIndex = existingName(fileName);

    if(havePermission) {
      // check if the file exists and if it is open
      if (fileDirectoryIndex != BLOCK_NOT_FOUND 
        && directory[fileDirectoryIndex].isOpen == true) {
          int counter = 0;
          int index = directory[fileDirectoryIndex].memoryBlock;
          // if it is the first data of the file
          if (fatTable[index] == FREE_BLOCK || fatTable[index] == RESERVED_BLOCK) {
            memoryUnit[index] = data;
            fatTable[index] = END_OF_FILE;
          } else {
            // if it is not the firs data of the file
            while (counter < T_MEMORY_UNIT) {
              if (fatTable[index] != END_OF_FILE) {
                index = fatTable[index];
              } else {
                int freeBlockIndex = searchFreeFatBlock();
                if (freeBlockIndex != FREE_MEMORY_NOT_FOUND) {
                  fatTable[index] = freeBlockIndex;
                  memoryUnit[freeBlockIndex] = data;
                  fatTable[freeBlockIndex] = END_OF_FILE;
                  counter = T_MEMORY_UNIT;
                } else {
                  cout << "\nNo free memory found for " <<  data << " in fat memory\n";
                }
              }
              counter++;
            }
          }
      } else if(fileDirectoryIndex == BLOCK_NOT_FOUND) {
        cout << "\nFile " + fileName + " not found\n";
      } else {
        cout << "\nThe file " + fileName + " is not open\n";
      }
    }
  }
  
  /*Writes to the file as indicated by the user (at the beginning or at the end)*/
  void write(string fileName, char data, int cursor, char user, string permission) {
    // check if the file exists and if it is open
    int fileDirectoryIndex = existingName(fileName);
    bool havePermission = checkPermission(user, permission, 'W');

    if(havePermission) {
      if (cursor == END_CURSOR) {
        append(fileName, data, user, permission);
      } else if(cursor ==  BEGINNING_CURSOR) {
        int oldFileIndex = directory[fileDirectoryIndex].memoryBlock;
        int newFileIndex = searchFreeFatBlock();
        if (newFileIndex != FREE_MEMORY_NOT_FOUND) {
          fatTable[newFileIndex] = oldFileIndex;
          memoryUnit[newFileIndex] = data;
          directory[fileDirectoryIndex].memoryBlock = newFileIndex;
        } else {
          cout << "\nNo free memory found for " << data << " in fat memory\n"; 
        }
      } else {
        cout << "\nInvalid cursor\n";
      }
    }
  }

  /*Delete the indicated file*/
  void remove(string fileName, char user, string permission) {
    // check if the file exists and if it is open
    int fileDirectoryIndex = existingName(fileName);
    bool havePermission = checkPermission(user, permission, 'X');

    if(havePermission) {
      if (fileDirectoryIndex != BLOCK_NOT_FOUND) {
        removeFileData(directory[fileDirectoryIndex].memoryBlock);
        directory[fileDirectoryIndex].memoryBlock = FREE_BLOCK;
        directory[fileDirectoryIndex].fileName = " ";
        directory[fileDirectoryIndex].lastUpdateDate = time(nullptr);
        directory[fileDirectoryIndex].isOpen = false;
        cout << "\nFile " + fileName + " was deleted\n";
      } else {
        cout << "\nFile " + fileName + " not found\n";
      }
    }
  }

  /* If file exists and its open then reads all the data*/
  string read(string fileName, char user, string permission) { 
    string allData;
    int index;
    bool next = true;

    bool havePermission = checkPermission(user, permission, 'R');
    int fileDirectoryIndex = existingName(fileName);
    
    if (havePermission) {
      // check if the file exists and if it is open
      if (fileDirectoryIndex != BLOCK_NOT_FOUND
      && directory[fileDirectoryIndex].isOpen == true) {
        index = directory[fileDirectoryIndex].memoryBlock;
        
        if (fatTable[index] != FREE_BLOCK && fatTable[index] != RESERVED_BLOCK) {      
          while(next){
            allData.push_back(memoryUnit[index]);
            allData.push_back(',');
            index = fatTable[index];

            if(index == END_OF_FILE){
              next = false;
            } 
          }
        } else {
          allData = "Empty file";
        } 
      } 
    }
    return allData;
  }
  
  //if file exists and its open then it close 
  void close(string fileName) {
    int fileDirectoryIndex = existingName(fileName);
    
    //file exists
    if (fileDirectoryIndex != BLOCK_NOT_FOUND) {
      // if the file is open
      if(directory[fileDirectoryIndex].isOpen == true) {
        directory[fileDirectoryIndex].isOpen = false;
        cout << "\nFile " +fileName+ " is closed now\n";
      } else {
        cout << "The file " +fileName+ " is already closed\n";
      }
    } else {
      cout << "\nFile " +fileName+ " not found\n";
    }
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
  void printFile(string fileName) {
    int directoryIndex = existingName(fileName);
    bool next = true;

    if (directoryIndex != BLOCK_NOT_FOUND 
    && directory[directoryIndex].isOpen == true) {
      int index = directory[directoryIndex].memoryBlock;
      
      if (fatTable[index] == FREE_BLOCK || fatTable[index] == RESERVED_BLOCK) {
        cout << "\nThe file " +fileName+ " has no data";
      } else {
        cout << "\n";
        cout << "File " +fileName+ " contains: \n";
        cout << "Data: ";
        
        while(next){
          cout << memoryUnit[index] << ", ";
          index = fatTable[index];
          if(index == END_OF_FILE){
            next = false;
          }
        }
        cout << "\n\n";
      }
    } else if(directoryIndex == BLOCK_NOT_FOUND) {
      cout << "\nFile " +fileName+ " not found\n";
    }
  }

  /*Check permission vectors*/
  bool checkPermission(char user, string permission
    , char permissionAction) {
    bool havePermission = false;
    
    // check if the permission is User
    if (permission == "user") {
      if (permissionAction == 'R') {
        havePermission = checkPermissionArray(user, userRead);
      } else if (permissionAction == 'W') {
        havePermission = checkPermissionArray(user, userWrite);
      } else if (permissionAction == 'X') {
        havePermission = checkPermissionArray(user, userRemove);
      } else {
        havePermission = false;
        cout <<  "\nPermissionAction not found\n";
      }
    } else if (permission == "group") {  // check if the permission is Group
      if (permissionAction == 'R') {
        havePermission = checkPermissionArray(user, groupRead);
      } else if (permissionAction == 'W') {
        havePermission = checkPermissionArray(user, groupWrite);
      } else if (permissionAction == 'X') {
        havePermission = checkPermissionArray(user, groupRemove);
      } else {
        havePermission = false;
        cout <<  "\nPermissionAction not found\n";
      }
    } else if (permission == "all") { // check if the permission is All
      if (permissionAction == 'R') {
        havePermission = checkPermissionArray(user, allRead);
      } else if (permissionAction == 'W') {
        havePermission = checkPermissionArray(user, allWrite);
      } else if (permissionAction == 'X') {
        havePermission = checkPermissionArray(user, allRemove);
      } else {
        havePermission = false;
        cout <<  "\nPermissionAction not found\n";
      }
    } else {
      havePermission = false;
      cout << "\nPermission not found\n";
    }
    return havePermission;
  }

  bool checkPermissionArray(char user, char arrayToCheck[]) {
    bool havePermission = false;
    int index = 0;
    while (havePermission == false && index < ARRAY_SIZE) {
      if (arrayToCheck[index] == user) {
        havePermission = true;
        index = ARRAY_SIZE;
      }
      index++;
    }
    if (!havePermission) {
      cout << "\nUser: " << user << " does not have the permission\n";
    }
    return havePermission;
  }
  /*Delete data from file*/
  void removeFileData(int index) {
    if (fatTable[index] != END_OF_FILE && 
      fatTable[index] != RESERVED_BLOCK) {
      removeFileData(fatTable[index]);
      memoryUnit[index] = '_';
      fatTable[index] = FREE_BLOCK;
    } else {
      memoryUnit[index] = '_';
      fatTable[index] = FREE_BLOCK;
    }
  }
};