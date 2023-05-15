#pragma once
#include <list>

// Storeage for decision Info as needed
struct decideInfo
{
	CString textLine;
	CString Tag;
	long Count;
};

// Scanner for Catalyst Software, the idea being it checking a dictionary to determine if something needs to be added to it
// help with translation process
// If can use a dictionary with phrases as provided
// Output can be in output file as well as decicion file (user can then manually decide what to do with what has been detected)
// This was experiment to see what can be done for checking information (WAS NOT COMPLETED)
class CCatalystScanner
{
private:
	CCatalystScanner();
	~CCatalystScanner();

	static CCatalystScanner *m_pInstance;
	CString m_sDictionary, m_sOutputFile, m_sDecideFile;
	std::list<std::pair<CString, long>> m_lstFilesScanned;
	std::list<CString> m_lstDictionary;
	std::list<decideInfo> m_lstDecide;

protected:
	void LoadDictionary();
	void AddTextToDecide(CString text, CString looking, long counts);
	long ScanFileForText(TCHAR* FullFileName);
	void ScanFolderForSubFolders(TCHAR* rootPath);
	int ScanTextForSymbols(CString text, bool ProcessQueries = false, bool ProcessCommonSymbols = false, bool ProcessSpecialWords = false);
	bool SaveDictionary();

public:
	// Note that the following is the singleton pattern so we can call the class anywhere in the code and end up
	// with the correct instance of the class
	static CCatalystScanner* get_instance()
	{
      if (!m_pInstance)
         m_pInstance = new CCatalystScanner();
         
      return m_pInstance;
	}

	// Handles the deletion of the singleton
	static void delete_instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	// Functions to use scanner
	void SetResources(CString DictionaryName, CString OutputFile, CString DecideFile); // Indicate what files to use for the scanner
	bool StartScanning(CString Source, bool ProcessDecide); // Provide a source for scanning (directory), also indicate if decision will happen or not
	bool SaveDecide(); // Saving decisions (allows users to decide on action once)
	bool ReprocessScan(CString Source); // Idea is once a run is done with dicisions, it will scan again to check the dictionary and output (Not fully implemented yet)

	std::list<decideInfo>* GetDecideList();
	std::list<CString>* GetDictionary();
};