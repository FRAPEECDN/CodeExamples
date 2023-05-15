#include "stdafx.h"  

#ifdef  _UNIT_TESTS 
#include "CppUnitTest.h"  

using namespace Microsoft::VisualStudio::CppUnitTestFramework;  

extern bool TextComparePatternMatch(const TCHAR* input, const TCHAR* pattern, const TCHAR singleWildcard, const TCHAR multipleWildcard, bool IgnoreCase);

namespace ExampleUnitTests
{  
	TEST_CLASS(CTest_MapDialogTextCompare)
	{
	public:

		TEST_CLASS_INITIALIZE(ClassInit)
		{
			// Blank - Note this test a single function
		}

		TEST_CLASS_CLEANUP(ClassCleanup)
		{
			// Blank - Note this test a single function
		}

		// Testing method for the Text Comparison pattern, testing function to match a pattern against a text and determining if the pattern is present in the text
		// if that is the case it would return true. The test function tests different cases of patterns and text as indicated in the comments above each section
		// Recommended test as implemented from the article using the same symbols as article 
		TEST_METHOD(TestTextComparePatternMatch)
		{
			// Cases with repeating character sequences.
			Assert::AreEqual(true, TextComparePatternMatch(_T("abcccd"), _T("*ccd"), _T('?'), _T('*'), false), L"repeating character sequences #1");
			Assert::AreEqual(true, TextComparePatternMatch(_T("mississipissippi"), _T("*issip*ss*"), _T('?'), _T('*'), false), L"repeating character sequences #2");
			Assert::AreEqual(false, TextComparePatternMatch(_T("xxxx*zzzzzzzzy*f"), _T("xxxx*zzy*fffff"), _T('?'), _T('*'), false), L"repeating character sequences #3");
			Assert::AreEqual(true, TextComparePatternMatch(_T("xxxx*zzzzzzzzy*f"), _T("xxx*zzy*f"), _T('?'), _T('*'), false), L"repeating character sequences #4");
			Assert::AreEqual(false, TextComparePatternMatch(_T("xxxxzzzzzzzzyf"), _T("xxxx*zzy*fffff"), _T('?'), _T('*'), false), L"repeating character sequences #5");
			Assert::AreEqual(true, TextComparePatternMatch(_T("xxxxzzzzzzzzyf"), _T("xxxx*zzy*f"), _T('?'), _T('*'), false), L"repeating character sequences #6");
			Assert::AreEqual(true, TextComparePatternMatch(_T("xyxyxyzyxyz"), _T("xy*z*xyz"), _T('?'), _T('*'), false), L"repeating character sequences #7");
			Assert::AreEqual(true, TextComparePatternMatch(_T("mississippi"), _T("*sip*"), _T('?'), _T('*'), false), L"repeating character sequences #8");
			Assert::AreEqual(true, TextComparePatternMatch(_T("xyxyxyxyz"), _T("xy*xyz"), _T('?'), _T('*'), false), L"repeating character sequences #9");
			Assert::AreEqual(true, TextComparePatternMatch(_T("mississippi"), _T("mi*sip*"), _T('?'), _T('*'), false), L"repeating character sequences #10");
			Assert::AreEqual(true, TextComparePatternMatch(_T("ababac"), _T("*abac*"), _T('?'), _T('*'), false), L"repeating character sequences #11");
			Assert::AreEqual(true, TextComparePatternMatch(_T("ababac"), _T("*abac*"), _T('?'), _T('*'), false), L"repeating character sequences #12");
			Assert::AreEqual(true, TextComparePatternMatch(_T("aaazz"), _T("a*zz*"), _T('?'), _T('*'), false), L"repeating character sequences #13");
			Assert::AreEqual(false, TextComparePatternMatch(_T("a12b12"), _T("*12*23"), _T('?'), _T('*'), false), L"repeating character sequences #14");
			Assert::AreEqual(false, TextComparePatternMatch(_T("a12b12"), _T("a12b"), _T('?'), _T('*'), false), L"repeating character sequences #15");
			Assert::AreEqual(true, TextComparePatternMatch(_T("a12b12"), _T("*12*12*"), _T('?'), _T('*'), false), L"repeating character sequences #16");
 
			// Additional cases where the '*' char appears in the tame string.
			Assert::AreEqual(true, TextComparePatternMatch(_T("*"), _T("*"), _T('?'), _T('*'), false), L"Wild card in string #1");
			Assert::AreEqual(true, TextComparePatternMatch(_T("a*abab"), _T("a*b"), _T('?'), _T('*'), false), L"Wild card in string #2");
			Assert::AreEqual(true, TextComparePatternMatch(_T("a*r"), _T("a*"), _T('?'), _T('*'), false), L"Wild card in string #3");
			Assert::AreEqual(false, TextComparePatternMatch(_T("a*ar"), _T("a*aar"), _T('?'), _T('*'), false), L"Wild card in string #4");
 
			// More double wildcard scenarios.
			Assert::AreEqual(true, TextComparePatternMatch(_T("XYXYXYZYXYz"), _T("XY*Z*XYz"), _T('?'), _T('*'), false), L"double wildcard scenarios #1");
			Assert::AreEqual(true, TextComparePatternMatch(_T("missisSIPpi"), _T("*SIP*"), _T('?'), _T('*'), false), L"double wildcard scenarios #2");
			Assert::AreEqual(true, TextComparePatternMatch(_T("mississipPI"), _T("*issip*PI"), _T('?'), _T('*'), false), L"double wildcard scenarios #3");
			Assert::AreEqual(true, TextComparePatternMatch(_T("xyxyxyxyz"), _T("xy*xyz"), _T('?'), _T('*'), false), L"double wildcard scenarios #4");
			Assert::AreEqual(true, TextComparePatternMatch(_T("miSsissippi"), _T("mi*sip*"),_T('?'), _T('*'), false), L"double wildcard scenarios #5");
			Assert::AreEqual(false, TextComparePatternMatch(_T("miSsissippi"), _T("mi*Sip*"), _T('?'), _T('*'), false), L"double wildcard scenarios #6");
			Assert::AreEqual(true, TextComparePatternMatch(_T("abAbac"), _T("*Abac*"), _T('?'), _T('*'), false), L"double wildcard scenarios #7");
			Assert::AreEqual(true, TextComparePatternMatch(_T("abAbac"), _T("*Abac*"), _T('?'), _T('*'), false), L"double wildcard scenarios #8");
			Assert::AreEqual(true, TextComparePatternMatch(_T("aAazz"), _T("a*zz*"), _T('?'), _T('*'), false), L"double wildcard scenarios #9");
			Assert::AreEqual(false, TextComparePatternMatch(_T("A12b12"), _T("*12*23"), _T('?'), _T('*'), false), L"double wildcard scenarios #10");
			Assert::AreEqual(true, TextComparePatternMatch(_T("a12B12"), _T("*12*12*"), _T('?'), _T('*'), false), L"double wildcard scenarios #11");
			Assert::AreEqual(true, TextComparePatternMatch(_T("oWn"), _T("*oWn*"), _T('?'), _T('*'), false), L"double wildcard scenarios #12");
 
			// Completely tame (no wildcards) cases.
			Assert::AreEqual(true, TextComparePatternMatch(_T("bLah"), _T("bLah"), _T('?'), _T('*'), false), L"no wildcards #1");
			Assert::AreEqual(false, TextComparePatternMatch(_T("bLah"), _T("bLaH"), _T('?'), _T('*'), false), L"no wildcards #2");
 
			// Simple mixed wildcard tests suggested by IBMer Marlin Deckert.
			Assert::AreEqual(true, TextComparePatternMatch(_T("a"), _T("*?"), _T('?'), _T('*'), false), L"mixed wildcard #1");
			Assert::AreEqual(true, TextComparePatternMatch(_T("ab"), _T("*?"), _T('?'), _T('*'), false), L"mixed wildcard #2");
			Assert::AreEqual(true, TextComparePatternMatch(_T("abc"), _T("*?"), _T('?'), _T('*'), false),L"mixed wildcard #3");
 
			// More mixed wildcard tests including coverage for false positives.
			Assert::AreEqual(false, TextComparePatternMatch(_T("a"), _T("??"), _T('?'), _T('*'), false), L"mixed wildcard false positives #1");
			Assert::AreEqual(true, TextComparePatternMatch(_T("ab"), _T("?*?"), _T('?'), _T('*'), false), L"mixed wildcard false positives #2");
			Assert::AreEqual(true, TextComparePatternMatch(_T("ab"), _T("*?*?*"), _T('?'), _T('*'), false), L"mixed wildcard false positives #3");
			Assert::AreEqual(true, TextComparePatternMatch(_T("abc"), _T("?**?*?"), _T('?'), _T('*'), false), L"mixed wildcard false positives #4");
			Assert::AreEqual(false, TextComparePatternMatch(_T("abc"), _T("?**?*&?"), _T('?'), _T('*'), false), L"mixed wildcard false positives #5");
			Assert::AreEqual(true, TextComparePatternMatch(_T("abcd"), _T("?b*??"), _T('?'), _T('*'), false), L"mixed wildcard false positives #6");
			Assert::AreEqual(false, TextComparePatternMatch(_T("abcd"), _T("?a*??"), _T('?'), _T('*'), false), L"mixed wildcard false positives #7");
			Assert::AreEqual(true, TextComparePatternMatch(_T("abcd"), _T("?**?c?"), _T('?'), _T('*'), false), L"mixed wildcard false positives #8");
			Assert::AreEqual(false, TextComparePatternMatch(_T("abcd"), _T("?**?d?"), _T('?'), _T('*'), false), L"mixed wildcard false positives #9");
			Assert::AreEqual(true, TextComparePatternMatch(_T("abcde"), _T("?*b*?*d*?"), _T('?'), _T('*'), false), L"mixed wildcard false positives #10");
 
			// Single-character-match cases.
			Assert::AreEqual(true, TextComparePatternMatch(_T("bLah"), _T("bL?h"), _T('?'), _T('*'), false), L"Single-character-match #1");
			Assert::AreEqual(false, TextComparePatternMatch(_T("bLaaa"), _T("bLa?"), _T('?'), _T('*'), false), L"Single-character-match #2");
			Assert::AreEqual(true, TextComparePatternMatch(_T("bLah"), _T("bLa?"), _T('?'), _T('*'), false), L"Single-character-match #3");
			Assert::AreEqual(false, TextComparePatternMatch(_T("bLaH"), _T("?Lah"), _T('?'), _T('*'), false), L"Single-character-match #4");
			Assert::AreEqual(true, TextComparePatternMatch(_T("bLaH"), _T("?LaH"), _T('?'), _T('*'), false), L"Single-character-match #5");
 
			// Many-wildcard scenarios.
			Assert::AreEqual(true, TextComparePatternMatch(_T("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab"), 
							_T("a*a*a*a*a*a*aa*aaa*a*a*b"), _T('?'), _T('*'), false), L"Many-wildcard scenarios #1");
			Assert::AreEqual(true, TextComparePatternMatch(_T("abababababababababababababababababababaacacacacacacacadaeafagahaiajakalaaaaaaaaaaaaaaaaaffafagaagggagaaaaaaaab"), 
							_T("*a*b*ba*ca*a*aa*aaa*fa*ga*b*"), _T('?'), _T('*'), false), L"Many-wildcard scenarios #2");
			Assert::AreEqual(false, TextComparePatternMatch(_T("abababababababababababababababababababaacacacacacacacadaeafagahaiajakalaaaaaaaaaaaaaaaaaffafagaagggagaaaaaaaab"), 
							_T("*a*b*ba*ca*a*x*aaa*fa*ga*b*"), _T('?'), _T('*'), false), L"Many-wildcard scenarios #3");
			Assert::AreEqual(true, TextComparePatternMatch(_T("abababababababababababababababababababaacacacacacacacadaeafagahaiajakalaaaaaaaaaaaaaaaaaffafagaagggagaaaaaaaab"), 
							_T("*a*b*ba*ca*aaaa*fa*ga*ggg*b*"), _T('?'), _T('*'), false), L"Many-wildcard scenarios #4");
			Assert::AreEqual(true, TextComparePatternMatch(_T("aaabbaabbaab"), _T("*aabbaa*a*"), _T('?'), _T('*'), false), L"Many-wildcard scenarios #5");
			Assert::AreEqual(true, TextComparePatternMatch(_T("a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*"), 
							_T("a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*"), _T('?'), _T('*'), false), L"Many-wildcard scenarios #6");
			Assert::AreEqual(true, TextComparePatternMatch(_T("aaaaaaaaaaaaaaaaa"), 
							_T("*a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*"), _T('?'), _T('*'), false), L"Many-wildcard scenarios #7");
			Assert::AreEqual(false, TextComparePatternMatch(_T("aaaaaaaaaaaaaaaa"), 
							_T("*a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*a*"), _T('?'), _T('*'), false), L"Many-wildcard scenarios #8");
			Assert::AreEqual(false, TextComparePatternMatch(_T("abc*abcd*abcde*abcdef*abcdefg*abcdefgh*abcdefghi*abcdefghij*abcdefghijk*abcdefghijkl*abcdefghijklm*abcdefghijklmn"), 
							_T("abc*abc*abc*abc*abc*abc*abc*abc*abc*abc*abc*abc*abc*abc*abc*abc*abc*"), _T('?'), _T('*'), false), L"Many-wildcard scenarios #9");
			Assert::AreEqual(true, TextComparePatternMatch(_T("abc*abcd*abcde*abcdef*abcdefg*abcdefgh*abcdefghi*abcdefghij*abcdefghijk*abcdefghijkl*abcdefghijklm*abcdefghijklmn"), 
							_T("abc*abc*abc*abc*abc*abc*abc*abc*abc*abc*abc*abc*"), _T('?'), _T('*'), false), L"Many-wildcard scenarios #10");
			Assert::AreEqual(false, TextComparePatternMatch(_T("abc*abcd*abcd*abc*abcd"), _T("abc*abc*abc*abc*abc"), _T('?'), _T('*'), false), L"Many-wildcard scenarios #11");
			Assert::AreEqual(true, TextComparePatternMatch(_T("abc*abcd*abcd*abc*abcd*abcd*abc*abcd*abc*abc*abcd"), 
							_T("abc*abc*abc*abc*abc*abc*abc*abc*abc*abc*abcd"), _T('?'), _T('*'), false), L"Many-wildcard scenarios #12");
			Assert::AreEqual(false, TextComparePatternMatch(_T("abababababababababababababababababababaacacacacacacacadaeafagahaiajakalaaaaaaaaaaaaaaaaaffafagaagggagaaaaaaaab"), 
							_T("*a*b*ba*ca*aaaa*fa*ga*gggg*b*"), _T('?'), _T('*'), false), L"Many-wildcard scenarios #13");

			Assert::AreEqual(true, TextComparePatternMatch(_T("abc"), _T("********a********b********c********"), _T('?'), _T('*'), false), L"Many-wildcard scenarios #14");
			Assert::AreEqual(false, TextComparePatternMatch(_T("********a********b********c********"), _T("abc"), _T('?'), _T('*'), false), L"Many-wildcard scenarios #15");
			Assert::AreEqual(false, TextComparePatternMatch(_T("abc"), _T("********a********b********b********"), _T('?'), _T('*'), false), L"Many-wildcard scenarios #16");
			Assert::AreEqual(true, TextComparePatternMatch(_T("*abc*"), _T("***a*b*c***"), _T('?'), _T('*'), false), L"Many-wildcard scenarios #17");
		}
	};
}

#endif //  _UNIT_TESTS 