#include "CsvManager.hh"

CsvManager* CsvManager::instance = 0;

CsvManager::CsvManager(void)
{

}

CsvManager::~CsvManager(void)
{

}

void CsvManager::CreateCSV(const char* name)
{
  archivo = new std::ofstream;
  archivo->open(name);
}


void CsvManager::Destroy() 
{
  if (instance != 0) 
    {
      delete instance;
      instance = 0;
    }
}

CsvManager * CsvManager::Instance() 	
{
   if (instance == 0) 
     {
       instance = new CsvManager();
     }
   
   return instance;
 }

void CsvManager::Merge()
{

}








