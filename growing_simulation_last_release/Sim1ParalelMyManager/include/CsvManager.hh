#ifndef CsvManager
#define CscManager 1

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>

class CsvManager{
public:
  CsvManager(void);
  ~CsvManager(void);
  
  void CreateCSV(const char* name);
  void Destroy();
  template<typename T>
  void FillCSV(T val  );
  static CsvManager* Instance();
  void Merge();
  
private:
  std::ofstream * archivo;
  static CsvManager* instance ;
};

template<typename T>
void CsvManager::FillCSV( T val  )
{
  * archivo <<val<<std::endl;
}


#endif
