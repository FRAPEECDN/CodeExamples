#include "stdafx.h"
#include "resource.h"
#include "Scanner.h"
#include "Utility.h"
#include <algorithm>
#include <fstream>

// Init Static reference
CCatalystScanner* CCatalystScanner::m_pInstance = NULL;

CCatalystScanner::CCatalystScanner()
{
	m_lstDictionary.clear();
	m_lstDecide.clear();
	m_sDictionary = _T("");
	m_sOutputFile = _T("");
	m_sDecideFile = _T("");
}

CCatalystScanner::~CCatalystScanner()
{
	m_lstFilesScanned.clear();
	m_lstDictionary.clear();
	m_lstDecide.clear();
}

void CCatalystScanner::SetResources(CString DictionaryName, CString OutputFile, CString DecideFile)
{
	m_sDictionary = DictionaryName;
	m_sOutputFile = OutputFile;
	m_sDecideFile = DecideFile;

	LoadDictionary();
}

bool CCatalystScanner::ReprocessScan(CString Source)
{
	bool RetVal = false;

	RetVal = SaveDictionary();
	return RetVal;
}

bool CCatalystScanner::StartScanning(CString Source, bool ProcessDecide)
{
	bool RetVal = false;
	m_lstFilesScanned.clear();
	ScanFolderForSubFolders(Source.GetBuffer());

	if (!ProcessDecide)
		RetVal = SaveDictionary();
	else
		RetVal = true;

	return RetVal;
}

// Scan a specific file for resource occurrences 
long CCatalystScanner::ScanFileForText(TCHAR* FullFileName)
{
	CString portionCheck;
	BYTE* buf = new BYTE[4048];	// Assign 4 K memory
	ScanningMode Mode = ScanningMode::NONE;
	long bytesRead = 0;
	long LinesChecked = 0;
	bool ProcessLines = false;

	HANDLE hFile = CreateFile(FullFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		while ((bytesRead = Read_Line_asByte(hFile, buf, 4048, true, false)) > 0)
		{
			CString textLine = CString(buf);
			textLine.TrimLeft(); textLine.TrimRight();

			if (ProcessLines)
			{
				if (textLine.Compare(PortionStart) != 0)
				{
					if (textLine.Compare(PortionEnd) == 0)
					{
						ProcessLines = false;
						continue;
					}
					else
					{
						switch(Mode)
						{
						case ScanningMode::PROCESS_DIALOG:
							{
								if ((textLine.Left(_tcslen(PortionCap)).Compare(PortionCap) == 0) || (textLine.Left(_tcslen(DLGDefButton)).Compare(DLGDefButton) == 0) || (textLine.Left(_tcslen(DLGButton)).Compare(DLGButton) == 0) ||
										(textLine.Left(_tcslen(DLGText)).Compare(DLGText) == 0))
								{
									portionCheck = CleanEntry(textLine);
								}
								else if (textLine.Left(_tcslen(DLGControl)).Compare(DLGControl) == 0)
								{
									portionCheck = CleanFirstEntry(textLine);
								}
							}
							break;

						case ScanningMode::PROCESS_MENU:
							{
								if (textLine.Find(PortionSep) < 0)
								{
									portionCheck = CleanEntry(textLine);
								}
							}
							break;

						case ScanningMode::PROCESS_TABLE:
							{
								portionCheck = CleanEntry(textLine);
							}
							break;
						};
					}
				}
				ScanTextForSymbols(portionCheck, false, false, true);
				LinesChecked++;
			}
			else
			{
				if (textLine.Find(StringTable) >= 0)  
				{
					Mode = ScanningMode::PROCESS_TABLE;
					ProcessLines = true;
				}
				else if ((textLine.Find(DialogID) >= 0) || (textLine.Find(Dialog_OTHER) >= 0))
				{
					Mode = ScanningMode::PROCESS_DIALOG;
					ProcessLines = true;
				}
				else if (textLine.Find(PopupID) >= 0)
				{
					Mode = ScanningMode::PROCESS_MENU;
					ProcessLines = true;
				}
			}
		}
		CloseHandle(hFile);
	}

	delete [] buf;
	return LinesChecked;
}

// Scan a folder structure for files to scan, so that dictionary can be build
void CCatalystScanner::ScanFolderForSubFolders(TCHAR* rootPath)
{
	TCHAR fullPath[MAX_PATH] = { 0 };
	TCHAR sPath[_MAX_PATH] = { 0 };
	WIN32_FIND_DATA FindFileData = { 0 };
	HANDLE hFind = nullptr;
	size_t nLen = 0;

	_tcsncpy_s(sPath, MAX_PATH, rootPath, _countof(sPath));
	sPath[_countof(sPath) - 1] = 0;
	nLen = _tcslen(sPath);

	if (sPath[nLen - 1] == '\\')
	{
		_stprintf_s(fullPath, _countof(fullPath), _T("%s%s"), sPath, _T("*"));
	}
	else
	{
		_stprintf_s(fullPath, _countof(fullPath), _T("%s%s"), sPath, _T("\\*"));
	}

	hFind = FindFirstFile(fullPath, &FindFileData);
	do
	{
		// Process File name into a full path to be used as subfolder or filename for scanning
		_tcsncpy_s(fullPath, _countof(fullPath), sPath, _countof(sPath));
		fullPath[_countof(fullPath) - 1] = 0;
		nLen = _tcslen(fullPath);

		if (fullPath[nLen - 1] != '\\')
			_tcscat_s(fullPath, _countof(fullPath), _T("\\"));
		_tcscat_s(fullPath, _countof(fullPath), FindFileData.cFileName);
		// Check 1 -> Is a file a subfolder, if it is then process it as well
		if ((FindFileData.dwFileAttributes | FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY && (FindFileData.cFileName[0] != '.'))	
		{
			ScanFolderForSubFolders(fullPath);
		}
		else if (FindFileData.cFileName[0] != '.')
		{
			CString sFileName = FindFileData.cFileName;
			CString sExt, sFullPath;

			int dotPos = sFileName.ReverseFind(_T('.'));
			sExt = sFileName.Mid(dotPos + 1); // Extact extension
			if (sExt.CompareNoCase(_T("rc")) == 0 )
			{
				sFullPath = fullPath;
				std::pair<CString, long> entry;
				entry.first = sFullPath;
				entry.second = ScanFileForText(fullPath);
				m_lstFilesScanned.push_back(entry);
			}
		}

	} while (FindNextFile(hFind, &FindFileData));

	FindClose(hFind);
}

void CCatalystScanner::LoadDictionary()
{
	BYTE* buf = new BYTE[4048];	// Assign 4 K memory
	long bytesRead = 0;
	bool found = false;

	// Open Dictionary file and load preset values to be included in scan
	HANDLE hFile = CreateFile(m_sDictionary, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		while ((bytesRead = Read_Line_asByte(hFile, buf, 4048, true, false)) > 0)
		{
			found = (std::find(m_lstDictionary.begin(), m_lstDictionary.end(), CString(buf)) != m_lstDictionary.end());
			if (!found) 
				m_lstDictionary.push_back(CString(buf));
		}
		CloseHandle(hFile);
	}
	delete [] buf;

	// Check some basics to make sure they are there
	found = (std::find(m_lstDictionary.begin(), m_lstDictionary.end(), CString(BasicString)) != m_lstDictionary.end());
	if (!found) 
		m_lstDictionary.push_back(CString(BasicString));
	found = (std::find(m_lstDictionary.begin(), m_lstDictionary.end(), CString(BasicDecimal)) != m_lstDictionary.end());
	if (!found) 
		m_lstDictionary.push_back(CString(BasicDecimal));
	found = (std::find(m_lstDictionary.begin(), m_lstDictionary.end(), CString(BasicInteger)) != m_lstDictionary.end());
	if (!found) 
		m_lstDictionary.push_back(CString(BasicInteger));
	found = (std::find(m_lstDictionary.begin(), m_lstDictionary.end(), CString(BasicFloat)) != m_lstDictionary.end());
	if (!found) 
		m_lstDictionary.push_back(CString(BasicFloat));
	found = (std::find(m_lstDictionary.begin(), m_lstDictionary.end(), CString(BasicHex)) != m_lstDictionary.end());
	if (!found) 
		m_lstDictionary.push_back(CString(BasicHex));
	found = (std::find(m_lstDictionary.begin(), m_lstDictionary.end(), CString(basicNewLineR)) != m_lstDictionary.end());
	if (!found) 
		m_lstDictionary.push_back(CString(basicNewLineR));
	found = (std::find(m_lstDictionary.begin(), m_lstDictionary.end(), CString(basicNewLineN)) != m_lstDictionary.end());
	if (!found) 
		m_lstDictionary.push_back(CString(basicNewLineN));
	found = (std::find(m_lstDictionary.begin(), m_lstDictionary.end(), CString(basicTab)) != m_lstDictionary.end());
	if (!found) 
		m_lstDictionary.push_back(CString(basicTab));
}

bool CCatalystScanner::SaveDictionary()
{
	std::fstream fs;
	std::list<CString>::iterator it;

	fs.open(m_sOutputFile, std::fstream::out);

	if (!fs.is_open())
		return false;	// error file not created

	for (it = m_lstDictionary.begin(); it != m_lstDictionary.end(); it++)
	{
		fs << CT2A(*it) << std::endl;
	}

	fs.flush();
	fs.close();
	return true;
}

bool CCatalystScanner::SaveDecide()
{
	std::fstream fs;
	std::list<decideInfo>::iterator it;

	fs.open(m_sDecideFile, std::fstream::out);

	if (!fs.is_open())
		return false;	// error file not created

	for (it = m_lstDecide.begin(); it != m_lstDecide.end(); it++)
	{
		fs << CT2A(_T("Text : ")) << CT2A(it->textLine) << CT2A(_T(" ; Tag : ")) << CT2A(it->Tag) << CT2A(_T("; Number encountered - ")) << it->Count << std::endl;
	}

	fs.flush();
	fs.close();
	return true;
}

// Return values
// 0 - Do Nothing, fully translated text
// -1 - Found something, have to get user to decide to include/exclude
// > 0 - Found a dictionary item(s) and added it to dictionary
int CCatalystScanner::ScanTextForSymbols(CString text, bool ProcessQueries, bool ProcessCommonSymbols, bool ProcessSpecialWords)
{
	bool found = false;
	bool CompleteQuery = false;
	bool CompleteSymbol = false;
	bool CompleteSelection = false;
	int idx = 0, textLen = 0, PosSkip = 0, PosStart = 0, PosEnd = 0;
	CString portion;
	int RetVal = 0;

	textLen = text.GetLength();

	if (textLen <= 0)
		return RetVal;

	// Start with Web Links if found will need to add to dictionary, then can skip query part
	if (ProcessSpecialWords)
	{
		for (idx = 0; idx < _countof(StringLink); idx++ )
		{
			PosStart = FindNoCase(text, StringLink[idx]);
			if (PosStart >= 0)
			{
				PosEnd = FindNoCase(text, _T(" "), PosStart + 1);
				if (PosEnd < 0)
				{
					portion = text.Right(textLen - PosStart);
				}
				else
				{
					portion = text.Mid(PosStart, PosEnd - PosStart);
				}

				found = (std::find(m_lstDictionary.begin(), m_lstDictionary.end(), portion) != m_lstDictionary.end());
				if (!found)
					m_lstDictionary.push_back(portion);

			}
		}
	}

	//Next is queries, when found determine how likely the text is a query and decide if it is query which will be added to dictionary, or if highly likely will ask user to decide
	if (ProcessQueries)
	{
		int *QueryWord = new int[_countof(StringQueries)];
		memset(QueryWord, 0, _countof(StringQueries) * sizeof(int));

		// Idea here is to scan for the words and then to count how many of the specific words have been found to determine if it is a query or not
		for (idx = 0; idx < _countof(StringQueries); idx++ )
		{
			PosSkip = _tcslen(StringQueries[idx]);
			PosStart = FindNoCase(text, StringQueries[idx]);
			while (PosStart >= 0)
			{
				// Check to make sure the word does contain a space before it or is start of text, otherwise it is not correct
				if ((PosStart > 0) && (text[PosStart - 1] != ' '))
				{
					PosStart += PosSkip + 1;
					if (PosStart >= textLen)
						break;
					else
						continue;
				}

				// Find the end of the entire word portion, so for instance if the word contains text after it will be longer
				// than the word being looked for
				PosEnd = FindNoCase(text, _T(" "), PosStart + PosSkip);	
				if (PosEnd == -1)
				{
					// Save rest of string for the portion to compare it (with no case) against word being looked for
					portion = text.Right(textLen - PosStart);
				}
				else
				{
					// Save the entire word portion to compare it (with no case) against word being looked for
					portion = text.Mid(PosStart, PosEnd - PosStart);
				}

				if (portion.CompareNoCase(StringQueries[idx]) == 0)
				{
					QueryWord[idx]++;	// increase count aka correct word
				}

				// Move on to next portion when there is portion left
				// Don't waste processing with checking after end of text reached
				if (PosEnd == -1)
					break;	

				PosStart = FindNoCase(text, StringQueries[idx], PosStart + PosEnd + 1);
			}
		}

		// found a select from where
		if ((QueryWord[0] == 1) && (QueryWord[1] == 1) && (QueryWord[2] == 1))
		{
			found = (std::find(m_lstDictionary.begin(), m_lstDictionary.end(), text) != m_lstDictionary.end());
			if (!found)
				m_lstDictionary.push_back(text);
			
			CompleteQuery = true;
		}

		if (!CompleteQuery && QueryWord[8] == 1)
		{
			found = (std::find(m_lstDictionary.begin(), m_lstDictionary.end(), text) != m_lstDictionary.end());
			if (!found)
				m_lstDictionary.push_back(text);

			CompleteQuery = true;
		}

		if (!CompleteQuery && ((QueryWord[0] > 0) || (QueryWord[1] > 0) || (QueryWord[2] > 0) || (QueryWord[3] > 0) || (QueryWord[4] > 0) || (QueryWord[5] > 0) || (QueryWord[6] > 0) || (QueryWord[7] > 0) || (QueryWord[8] > 0)))
		{
			long cnts = QueryWord[0] + QueryWord[1] + QueryWord[2] + QueryWord[3] + QueryWord[4] + QueryWord[5] + QueryWord[6] + QueryWord[7] + QueryWord[8];
			if (cnts > 1)
			{
				AddTextToDecide(text, _T("Query"), cnts);
				CompleteQuery = true;
			}
		}

		ProcessQueries = !CompleteQuery;	// Stop the other query section
		delete [] QueryWord;
	}

	// Now check text for column selection queries, when selection aka "," is higher than 3 then it is a column selection, is found to be 1 or 2 user will be asked
	if (ProcessQueries)
	{
		long cntColumns = 0;
		PosStart = FindNoCase(text, _T("\""));
		while (PosStart >= 0)
		{
			if ((text[PosStart + 1] == ',') && (text[PosStart + 2] == '\"'))
			{
				cntColumns++;
				PosEnd = PosStart + 3;
				PosStart = FindNoCase(text, _T("\""), PosEnd);
			}
			else
			{
				PosStart = -1;
			}
		}

		if (PosStart > 0)
		{
			if (cntColumns >=  3)
			{
				found = (std::find(m_lstDictionary.begin(), m_lstDictionary.end(), text) != m_lstDictionary.end());
				if (!found)
					m_lstDictionary.push_back(text);		
			}
			else
			{
				AddTextToDecide(text, _T("Columns ,"), cntColumns);
			}
		}
	}


	//Next is specific meaning Symbols like ~ for smart strings, *. .* indicating posible files and so on, in some cases the user will be ask to confirm
	if (ProcessSpecialWords)
	{
		bool blnDoTrim = false;
		// Idea here is to scan for the multiple symbols and then check if it repeats, or have for instance extensions etc. associated
		// with it more
		for (idx = 0; idx < _countof(StringSymbolsStart); idx++ )
		{
			PosStart = FindNoCase(text, StringSymbolsStart[idx]);
			while (PosStart >= 0)
			{
				// Find the end of the entire word portion, so for instance if the word contains text after it will be longer
				// than the word being looked for
				if (StringSymbolsStart[idx] == _T("~"))
				{
					PosEnd = FindNoCase(text, _T("~"), PosStart + 1);
				}
				else if (StringSymbolsStart[idx] == _T("^"))
				{
					int e1 = 0, e2 = 0;

					e1 = FindNoCase(text, _T("@"), PosStart + 1);
					e2 = FindNoCase(text, _T("^"), PosStart + 1);
					
					if ((e1 != -1) && (e1 < e2))
						PosEnd = e1 - 1;
					else
						PosEnd = e2;

				}
				else if (StringSymbolsStart[idx] == _T("@"))
				{
					CString tmp;
					PosEnd = FindEndingSymbol(text, PosStart + 1);
					if (PosEnd == -1)
					{
						tmp = text.Right(textLen - (PosStart - 1));
					}
					else
					{
						tmp = text.Mid(PosStart, PosEnd - (PosStart - 1));
					}
					int postmp = FindNoCase(tmp, _T("."));
					if (postmp > 0)
					{
						int newStart = FindStartingSymbol(text, PosStart);
						blnDoTrim = true;
						if (newStart > 1)
							PosStart = newStart;
						else if (newStart == 1)
							PosStart = 0;
					}
				}
				else
				{
					PosEnd = FindEndingSymbol(text, PosStart + 1);
				}
				if (PosEnd == -1)
				{
					if (StringSymbolsStart[idx] == _T("~"))
					{
						break;	// Need two pieces of ~, so no more smart strings
					}
					// Save rest of string for the portion to compare it (with no case) against word being looked for
					portion = text.Right(textLen - (PosStart - 1));
					if (blnDoTrim)
					{
						portion.TrimLeft();
						blnDoTrim = false;
					}
				}
				else
				{
					if (StringSymbolsStart[idx] == _T("~"))
					{
						if (text[PosEnd + 1] == _T('='))
						{
							PosEnd = FindNoCase(text, _T(":"), PosEnd + 1);
							PosEnd--;
						}
					}
					// Save the entire word portion to compare it (with no case) against word being looked for
					portion = text.Mid(PosStart, PosEnd - (PosStart - 1));
					if (blnDoTrim)
					{
						portion.TrimLeft();
						blnDoTrim = false;
					}
				}

				// Move on to next portion when there is portion left
				// Don't waste processing with checking after end of text reached
				if (PosEnd == -1)
					break;	
				else
				{
					if (portion.GetLength() > 1)
					{
						found = (std::find(m_lstDictionary.begin(), m_lstDictionary.end(), portion) != m_lstDictionary.end());
						if (!found)
							m_lstDictionary.push_back(portion);		
					}
				}

				PosStart = FindNoCase(text, StringSymbolsStart[idx], PosEnd + 1);
			}
		}

	}

	// Check for the more common symnbols, if a high count is found then user will be asked to verify
	if (ProcessCommonSymbols)
	{
		// Idea here is to scan for the multiple common symbols and then check if it repeats, or have for instance extensions etc. associated
		// with it more

		long finalCount = countAllSymbols(text);
		if (finalCount >= 10)
		{
			AddTextToDecide(text, _T("Common Symbols"), finalCount);
		}
	}

	return RetVal;	// Do Nothing
}

void CCatalystScanner::AddTextToDecide(CString text, CString looking, long counts)
{
	decideInfo myInfo;
	std::list<decideInfo>::iterator it;
	bool found = false;

	// See if text is in dictionary first, if it is no need to add to decide
	bool foundDic = (std::find(m_lstDictionary.begin(), m_lstDictionary.end(), text) != m_lstDictionary.end());
	if (foundDic)
		return;

	myInfo.textLine = text;
	myInfo.Tag = looking;
	myInfo.Count = counts;

	for (it = m_lstDecide.begin(); it != m_lstDecide.end(); it++)
	{
		if ((myInfo.textLine == it->textLine) && (myInfo.Tag == it->Tag))
		{
			found = true;
			break;
		}
	}

	if (!found)
		m_lstDecide.push_back(myInfo);
}

std::list<decideInfo>* CCatalystScanner::GetDecideList()
{
	return &m_lstDecide;
}

std::list<CString>* CCatalystScanner::GetDictionary()
{
	return &m_lstDictionary;
}