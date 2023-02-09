#ifndef Misc_h__
#define Misc_h__

#define GAE_RAD_DEGREE 0.017453f
//0.017453292519943295769f

#ifdef _DEBUG
	#define GAE_TRACE_MATRIX44(m)	\
		GAE_TRACE(L"\t[1]\t[2]\t[3]\t[4]\n");									\
		GAE_TRACE(L"Row[1]: [%f, %f, %f, %f]\n", m._11, m._12, m._13, m._14);	\
		GAE_TRACE(L"Row[2]: [%f, %f, %f, %f]\n", m._21, m._22, m._23, m._24);	\
		GAE_TRACE(L"Row[3]: [%f, %f, %f, %f]\n", m._31, m._32, m._33, m._34);	\
		GAE_TRACE(L"Row[4]: [%f, %f, %f, %f]\n", m._41, m._42, m._43, m._44)
#elif _RELEASE
	#define GAE_TRACE_MATRIX44(m)
#endif

#endif // Misc_h__