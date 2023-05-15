// This function checks a specific text string by detecting if any Wildcards (Single and Multiple) is present
bool TextCheckForWildcards(const TCHAR* input, const TCHAR singleWildcard, const TCHAR multipleWildcard)
{
	TCHAR SepToken[3] = { 0 };
	SepToken[0] = singleWildcard;
	SepToken[1] = multipleWildcard;
	TCHAR* inputCopy = _tcsdup(input);

	TCHAR* pToken = _tcstok(inputCopy, SepToken);
	free(inputCopy);
	return (pToken != nullptr);	// indicate there is Wildcards
}

// This function compares an input text string, with a pattern text string which can have wildcards (Single and Multiple), also can indicate if case matters
// Modified from article http://www.drdobbs.com/architecture-and-design/matching-wildcards-an-empirical-way-to-t/240169123 to handle TCHAR with (Multibyte Character Sets)
// as well as the required ability to define the single / multiple wild characters. Original code by Kirk J. Krauss
bool TextComparePatternMatch(const TCHAR* input, const TCHAR* pattern, const TCHAR singleWildcard, const TCHAR multipleWildcard, bool IgnoreCase)
{
	TCHAR* inputCopy = _tcsdup(input);
	TCHAR* patternCopy = _tcsdup(pattern);
	// i is the pointer for input and p is the pointer for the pattern
	unsigned int i = 0, p = 0;

	if (IgnoreCase) // Make sure case is same for input and pattern 
	{
		_tcslwr(inputCopy);
		_tcslwr(patternCopy);
	}

	TCHAR* inputStep = inputCopy;
	TCHAR* patternStep = patternCopy;

	// These two values are set when we observe a wildcard character.  They 
	// represent the locations, in the two strings, from which we start once 
	// we've observed it.
	//
	TCHAR *inputBookmark = nullptr;
	TCHAR *patternBookmark = nullptr;

	// Walk the text strings one character at a time.
	p = _tcsnextc(patternStep);
	i = _tcsnextc(inputStep);
	while (1)
	{
		// How do you match a unique text string?
		if (p == multipleWildcard)
		{
			// Easy: unique up on it!
			while (p == multipleWildcard) //  while (*(++pPatternText) == '*') "xy" matches "x**y"
			{
				patternStep = _tcsinc(patternStep);
				p = _tcsnextc(patternStep);
			}

			if (p == _T('\0')) //(!*pWildText)
			{
				free(inputCopy);
				free(patternCopy);
				return true;           // "x" matches "*"
			}

			if (p != singleWildcard) // *pPatternText != '?'
			{
				// Fast-forward to next possible match.
				while (i != p) // *pInputText != *pPatternText
				{
					inputStep = _tcsinc(inputStep);
					i = _tcsnextc(inputStep);

					if (i == _T('\0')) // (!(*(++pInputText)))
					{
						free(inputCopy);
						free(patternCopy);
						return false;  // "x" doesn't match "*y*"
					}
				}
			}

			patternBookmark = patternStep;
			inputBookmark = inputStep;
		}
		else if ((i != p) && (p != singleWildcard)) // *pInputText != *pPatternText && *pPatternText != '?'
		{
			// Got a non-match.  If we've set our bookmarks, back up to one 
			// or both of them and retry.
			//
			if (patternBookmark)
			{
				if (_tcsicmp(patternStep, patternBookmark) != 0)
				{
					patternStep = patternBookmark;
					p = _tcsnextc(patternStep);

					if (i != p)
					{
						// Don't go this far back again. ( pInputText = ++pInputBookmark;)
						inputBookmark = _tcsinc(inputBookmark);
						inputStep = inputBookmark;
						i = _tcsnextc(inputStep);
						continue;      // "xy" matches "*y"
					}
					else
					{
						patternStep = _tcsinc(patternStep);
						p = _tcsnextc(patternStep);
					}
				}

				if (i != _T('\0'))
				{
					inputStep = _tcsinc(inputStep);
					i = _tcsnextc(inputStep);
					continue;          // "mississippi" matches "*sip*"
				}
			}

			free(inputCopy);
			free(patternCopy);
			return false;              // "xy" doesn't match "x"
		}

		inputStep = _tcsinc(inputStep);
		i = _tcsnextc(inputStep);
		patternStep = _tcsinc(patternStep);
		p = _tcsnextc(patternStep);

		// How does the input text match up now?
		if (i == _T('\0'))
		{
			// How do you match a unique text string,  unique up on it!
			while (p == multipleWildcard)
			{
				patternStep = _tcsinc(patternStep);
				p = _tcsnextc(patternStep);
			}

			if (p == _T('\0'))
			{
				free(inputCopy);
				free(patternCopy);
				return true;           // "x" matches "x"
			}

			free(inputCopy);
			free(patternCopy);
			return false;              // "x" doesn't match "xy"
		}
	}
}