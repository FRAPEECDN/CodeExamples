#include <unordered_map>


void InsertIntoMap(const TCHAR* ItemID, const TCHAR* ItemNumber)
{
	if (!g_pMapStrToStr)
	{
		// Create unordered has map with 40000 entries for 
		g_pMapStrToStr = new std::unordered_map<tstring, TCHAR*, hash_tstring>();
		g_pMapStrToStr->reserve(40000);
	}

	TCHAR *p = new TCHAR[_tcslen(ItemNumber) + 1];
	_tcscpy((TCHAR*)p, ItemNumber);

	std::pair<std::unordered_map<tstring, TCHAR*>::iterator, bool> ret = g_pMapStrToStr->insert(std::pair<tstring, TCHAR*>(ItemID, p));
	if (!ret.second)
	{
		std::unordered_map<tstring, TCHAR*>::iterator it = ret.first;
		delete[] it->second;
		it->second = p;
	}
}

BOOL GetFromMap(const TCHAR* ItemID, TCHAR* ItemNumber)
{
	*ItemNumber = 0;

	if (!g_pMapStrToStr)
		return 0;

	std::unordered_map<tstring, TCHAR*>::iterator it = g_pMapStrToStr->find(ItemID);
	if (it == g_pMapStrToStr->end())
	{
		return 0;
	}

	_tcscpy((TCHAR*)ItemNumber, it->second);
	return -1;
}

extern "C" void DeleteMap(){
	std::unordered_map<tstring, TCHAR*>::iterator it;

	if (g_pMapStrToStr)
	{
		for (it = g_pMapStrToStr->begin(); it != g_pMapStrToStr->end(); it++)
		{
			delete[] it->second;
		}
		g_pMapStrToStr->clear();
		delete g_pMapStrToStr;
		g_pMapStrToStr = 0;
	}
#ifdef _DEBUG
	else{
		TRACE(_T("***Warning !! AcnoMap not in use\n"));
	}
#endif
}