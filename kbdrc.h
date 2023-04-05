//---------------------------------------------------------------------------
// Windows Keyboards Layouts (WKL)
// Header for keyboard DLL resource files (.rc)
//---------------------------------------------------------------------------

#include <windows.h>
#include <ntverp.h>

#ifdef VER_PRODUCTNAME_STR
#undef VER_PRODUCTNAME_STR
#endif

#ifdef VER_PRODUCTVERSION
#undef VER_PRODUCTVERSION
#endif

#ifdef VER_PRODUCTVERSION_STR
#undef VER_PRODUCTVERSION_STR
#endif

#ifdef VER_LEGALCOPYRIGHT_STR
#undef VER_LEGALCOPYRIGHT_STR
#endif

#define WKL_NAME_1(symbol)      #symbol
#define WKL_NAME(symbol,suffix) WKL_NAME_1(symbol) suffix

#define VER_FILETYPE              VFT_DLL
#define VER_FILESUBTYPE           VFT2_DRV_KEYBOARD
#define VER_PRODUCTNAME_STR       "Windows Keyboards Layouts (WKL)"
#define VER_LEGALCOPYRIGHT_STR    "(c) WKL Project"
#define VER_PRODUCTVERSION        1,0,0,0
#define VER_PRODUCTVERSION_STR    "1.0"
#define VER_INTERNALNAME_STR      WKL_NAME(WKL_PROJECT_NAME, "")
#define VER_ORIGINALFILENAME_STR  WKL_NAME(WKL_PROJECT_NAME, ".dll")
#define VER_LANGNEUTRAL
