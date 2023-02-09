#ifndef Assert_h__
#define Assert_h__

#include "stdafx.h"

#define GAE_XSTRINGIFY(x) #x
#define GAE_STRINGIFY(x) GAE_XSTRINGIFY(x)

#define GAE_CHAR_TO_WCHAR2(x) L ## x
#define GAE_CHAR_TO_WCHAR(x)	GAE_CHAR_TO_WCHAR2(x)
#define __WFILE__ GAE_CHAR_TO_WCHAR(__FILE__)

//#define _DEBUG_NO_ASSERT
#define _DEBUG_NO_WARNING

#ifdef _RELEASE
#define _RELEASE_TRACE
#endif

//#if defined(_DEBUG) && !defined(_DEBUG_NO_ASSERT)
#if defined(_DEBUG)

struct GAE_D3D_UTIL
{
	static inline const wchar_t* TranslateD3DError(HRESULT hErr)
	{
		switch(hErr)
		{
		case D3DERR_WRONGTEXTUREFORMAT:
			return L"D3DERR_WRONGTEXTUREFORMAT";
			break;
		case D3DERR_UNSUPPORTEDCOLOROPERATION:
			return L"D3DERR_UNSUPPORTEDCOLOROPERATION";
			break;
		case D3DERR_UNSUPPORTEDCOLORARG:
			return L"D3DERR_UNSUPPORTEDCOLORARG";
			break;
		case D3DERR_UNSUPPORTEDALPHAOPERATION:
			return L"D3DERR_UNSUPPORTEDALPHAOPERATION";
			break;
		case D3DERR_UNSUPPORTEDALPHAARG:
			return L"D3DERR_UNSUPPORTEDALPHAARG";
			break;
		case D3DERR_TOOMANYOPERATIONS:
			return L"D3DERR_TOOMANYOPERATIONS";
			break;
		case D3DERR_CONFLICTINGTEXTUREFILTER:
			return L"D3DERR_CONFLICTINGTEXTUREFILTER";
			break;
		case D3DERR_UNSUPPORTEDFACTORVALUE:
			return L"D3DERR_UNSUPPORTEDFACTORVALUE";
			break;
		case D3DERR_CONFLICTINGRENDERSTATE:
			return L"D3DERR_CONFLICTINGRENDERSTATE";
			break;
		case D3DERR_UNSUPPORTEDTEXTUREFILTER:
			return L"D3DERR_UNSUPPORTEDTEXTUREFILTER";
			break;
		case D3DERR_CONFLICTINGTEXTUREPALETTE:
			return L"D3DERR_CONFLICTINGTEXTUREPALETTE";
			break;
		case D3DERR_DRIVERINTERNALERROR:
			return L"D3DERR_DRIVERINTERNALERROR";
			break;
		case D3DERR_NOTFOUND:
			return L"D3DERR_NOTFOUND";
			break;
		case D3DERR_MOREDATA:
			return L"D3DERR_MOREDATA";
			break;
		case D3DERR_DEVICELOST:
			return L"D3DERR_DEVICELOST";
			break;
		case D3DERR_DEVICENOTRESET:
			return L"D3DERR_DEVICENOTRESET";
			break;
		case D3DERR_NOTAVAILABLE:
			return L"D3DERR_NOTAVAILABLE";
			break;
		case D3DERR_OUTOFVIDEOMEMORY:
			return L"D3DERR_OUTOFVIDEOMEMORY";
			break;
		case D3DERR_INVALIDDEVICE:
			return L"D3DERR_INVALIDDEVICE";
			break;
		case D3DERR_INVALIDCALL:
			return L"D3DERR_INVALIDCALL";
			break;
		case D3DERR_DRIVERINVALIDCALL:
			return L"D3DERR_DRIVERINVALIDCALL";
			break;
		case D3DERR_WASSTILLDRAWING:
			return L"D3DERR_WASSTILLDRAWING";
			break;
		case D3DERR_DEVICEREMOVED:
			return L"D3DERR_DEVICEREMOVED";
			break;
		case D3DERR_DEVICEHUNG:
			return L"D3DERR_DEVICEHUNG";
			break;
		default:
			return L"UNKOWN ERROR";
			break;
		}
	}
};

	#define GAE_TRACE											\
		Gae::Core::Util::Log(L"%s(%u) : ",__WFILE__, __LINE__);	\
		Gae::Core::Util::Log										\

	#define GAE_TRACE_ASSERT_HEADER()											\
		Gae::Core::Util::Log(L"=============== ASSERT ===============\n")		\

	#define GAE_TRACE_WARNING_HEADER()											\
		Gae::Core::Util::Log(L"=============== WARNING ===============\n")		\

	#define GAE_TRACE_EXP(exp)													\
		Gae::Core::Util::Log(L"\n");												\
		Gae::Core::Util::Log(L"EXPRESSION:\n\t");								\
		Gae::Core::Util::Log(L"%s\n", GAE_CHAR_TO_WCHAR(GAE_STRINGIFY(exp)));	\
		Gae::Core::Util::Log(L"\n");												\
		Gae::Core::Util::Log(L"======================================\n")		\

	#define GAE_TRACE_EXP_MSG(exp,...)											\
		Gae::Core::Util::Log(L"\n");												\
		Gae::Core::Util::Log(L"EXPRESSION:\n");									\
		Gae::Core::Util::Log(L"\t%s\n\n", GAE_CHAR_TO_WCHAR(GAE_STRINGIFY(exp)));\
		Gae::Core::Util::Log(L"MESSAGE:\n\t");									\
		Gae::Core::Util::Log(__VA_ARGS__);					\
		Gae::Core::Util::Log(L"\n\n");											\
		Gae::Core::Util::Log(L"======================================\n")		\

	#if !defined(_DEBUG_NO_WARNING)
		#define GAE_WARNING(exp)			\
			if(!(exp))						\
			{								\
				GAE_TRACE_WARNING_HEADER();	\
				GAE_TRACE_EXP(exp);			\
			}

		#define GAE_WARNINGM(exp,...)				\
			if(!(exp))								\
			{										\
				GAE_TRACE_WARNING_HEADER();			\
				GAE_TRACE_EXP_MSG(exp,__VA_ARGS__);	\
			}
	#else
		#define GAE_WARNING(exp)
		#define GAE_WARNINGM(exp,...)
	#endif

	#if !defined(_DEBUG_NO_ASSERT)
		#define GAE_ASSERT(exp)				\
			if(!(exp))						\
			{								\
				GAE_TRACE_ASSERT_HEADER();	\
				GAE_TRACE_EXP(exp);			\
				__asm {int 3};				\
			}

		#define GAE_ASSERTM(exp,...)				\
			if(!(exp))								\
			{										\
				GAE_TRACE_ASSERT_HEADER();			\
				GAE_TRACE_EXP_MSG(exp,__VA_ARGS__);	\
				__asm {int 3};						\
			}

		#define GAE_ASSERT_D3D_FN(retValue,exp)						\
		do															\
		{															\
			HRESULT d3dRes = exp;									\
			if(d3dRes != retValue)									\
			{														\
				GAE_TRACE_ASSERT_HEADER();							\
				GAE_TRACE_EXP_MSG(exp, L"Function call return value: %s\n\t%s", DXGetErrorString(d3dRes), DXGetErrorDescription(d3dRes));	\
				__asm {int 3};										\
			}														\
		} while(false)												

		#define GAE_ASSERT_FN(retValue,exp)									\
		do																	\
		{																	\
			HRESULT hr = exp;												\
			if(hr != retValue)												\
			{																\
				GAE_TRACE_ASSERT_HEADER();									\
				GAE_TRACE_EXP_MSG(exp, L"Function call return value: %s\n\t%s", DXGetErrorString(hr), DXGetErrorDescription(hr));	\
			}																\
		} while(false)
	#else
		#define GAE_ASSERT(exp)
		#define GAE_ASSERTM(exp,...)
		#define GAE_ASSERT_D3D_FN(retValue,exp)	exp;
		#define GAE_ASSERT_FN(retValue,exp)	exp;
	#endif

#else if defined(_RELEASE || _DEBUG_NO_ASSERT)

#if defined(_RELEASE_TRACE)

	#define GAE_TRACE											\
		Gae::Core::Util::Log(L"%s(%u) : ",__WFILE__, __LINE__);	\
		Gae::Core::Util::Log										\

#else

	#define GAE_TRACE

#endif

	#define GAE_TRACE_ASSERT_HEADER()
	#define GAE_TRACE_WARNING_HEADER()
	#define GAE_TRACE_EXP(exp)
	#define GAE_TRACE_EXP_MSG(exp,...)
	#define GAE_WARNING(exp)
	#define GAE_WARNINGM(exp,...)
	#define GAE_ASSERT(exp)
	#define GAE_ASSERTM(exp,...)
	#define GAE_ASSERT_D3D_FN(retValue,exp)	exp;
	#define GAE_ASSERT_FN(retValue,exp)	exp;

#endif

#endif // Assert_h__