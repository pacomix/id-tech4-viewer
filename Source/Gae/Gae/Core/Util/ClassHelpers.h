#ifndef __CLASSHELPERS_H
#define __CLASSHELPERS_H

#define GAE_DELETESAFE(object)												\
	/*GAE_ASSERTM(NULL != object, L"Attempt to delete a NULL pointer!");*/	\
	if(NULL != object)														\
	{																		\
		delete object;														\
		object = NULL;														\
	}

#define GAE_DELETESAFEARRAY(object)													\
	/*GAE_ASSERTM(NULL != object, L"Attempt to delete a NULL pointer array!");	*/	\
	if(NULL != object)																\
	{																				\
		delete[] object;															\
		object = NULL;																\
	}

#ifdef _DEBUG_DRAW
	#define GAE_DEBUG_FN_DECL(...)	__VA_ARGS__
	#define GAE_DEBUG_FN_IMPL(...)	__VA_ARGS__
#else
	#define GAE_DEBUG_FN_DECL(...) __VA_ARGS__
	#define GAE_DEBUG_FN_IMPL(...) __VA_ARGS__
#endif

#define SINGLETON_DECL(ClassName)				\
		public:									\
		static inline void CreateSingleton()	\
		{										\
			if(m_spInstance) return;			\
			m_spInstance = new(ClassName);		\
			if(!m_spInstance->Init())			\
			{									\
				GAE_ASSERTM(false, L"Init failed!")\
			}									\
		}										\
												\
		static inline void DestroySingleton()	\
		{										\
			if(!m_spInstance->Uninit())			\
			{									\
				GAE_ASSERTM(false, L"Uninit failed!")\
			}									\
			GAE_DELETESAFE(m_spInstance);			\
		}										\
												\
		static inline ClassName& Singleton()	\
		{										\
			return *m_spInstance;				\
		}										\
												\
		private:								\
		ClassName();							\
		~##ClassName();							\
		bool Init();							\
		bool Uninit();							\
		static ClassName* m_spInstance			\

#define SINGLETON_HIJACK_DECL(ClassName,Hijack)	\
	public:										\
	static inline void CreateSingleton()		\
	{											\
		if(m_spInstance && m_spHijack) return;	\
		m_spInstance = new(ClassName);			\
		m_spHijack = new(Hijack);				\
		if(m_spInstance->Init())				\
		{										\
			GAE_ASSERTM(false, L"Init failed!");	\
		}										\
	}											\
												\
	static inline void DestroySingleton()		\
	{											\
		if(!m_spInstance->Uninit())				\
		{										\
			GAE_ASSERTM(false, L"Uninit failed!")\
		}										\
		GAE_DELETESAFE(m_spInstance);				\
		GAE_DELETESAFE(m_spHijack);					\
	}											\
												\
	static inline Hijack& Singleton()			\
	{											\
		return *m_spHijack;						\
	}											\
												\
	private:									\
		ClassName() {}							\
		~##ClassName(){}						\
		bool Init();							\
		bool Uninit();							\
		static ClassName* m_spInstance;			\
		static Hijack* m_spHijack				\

#define SINGLETON_IMPL(ClassName)				\
	ClassName* ClassName::m_spInstance = NULL;	\
	ClassName::##ClassName(){}					\
	ClassName::~##ClassName(){}					\

#define SINGLETON_HIJACK_IMPL(ClassName,Hijack)	\
	ClassName* ClassName::m_spInstance = NULL;	\
	Hijack* ClassName::m_spHijack = NULL		\

#define GAE_CONTAINER_DECL( Obj, ContainerName )				\
	public:										\
	typedef std::vector< Obj* > Vec##ContainerName;	\
	const Vec##ContainerName& GetVec##ContainerName() const; \
	UINT32 Create##ContainerName();				\
	UINT32 Add##ContainerName(Obj* p##ContainerName);		\
	void Remove##ContainerName(Obj* p##ContainerName);		\
	Obj* Get##ContainerName(UINT32 iIdx) const;	\
	bool Contains##ContainerName(const Obj* p##ContainerName) const;	\
	bool Is##ContainerName##Empty() const;\
	void Clear##ContainerName();\
	\
	private:									\
	Vec##ContainerName m_Vec##ContainerName				



	#define GAE_CONTAINER_IMPL( Class, Obj, ContainerName )	\
	const Class::Vec##ContainerName& Class::GetVec##ContainerName() const\
	{\
	return m_Vec##ContainerName;\
	}\
	UINT32 Class::Create##ContainerName()	\
	{													\
	Obj* p##ContainerName = new Obj;				\
	return (UINT32)Add##ContainerName( p##ContainerName );	\
	}													\
	\
	UINT32 Class::Add##ContainerName(Obj* p##ContainerName)	\
	{												\
	Vec##ContainerName::iterator itFound = std::find(m_Vec##ContainerName.begin(), m_Vec##ContainerName.end(), p##ContainerName); \
	GAE_ASSERTM(itFound == m_Vec##ContainerName.end(), L"Object is already in the list!");	\
	m_Vec##ContainerName.push_back(p##ContainerName);	\
	return (UINT32)m_Vec##ContainerName.size() - 1;	\
	}									\
	void Class::Remove##ContainerName(Obj* p##ContainerName)	\
	{\
	Vec##ContainerName::iterator itFound = std::find(m_Vec##ContainerName.begin(), m_Vec##ContainerName.end(), p##ContainerName); \
	GAE_ASSERTM(itFound != m_Vec##ContainerName.end(), L"Object is not in the list!");	\
	m_Vec##ContainerName.erase(itFound);\
	}\
	\
	Obj* Class::Get##ContainerName(UINT32 iIdx) const							\
	{																	\
	GAE_ASSERTM(iIdx < m_Vec##ContainerName.size(), L"Index out of range!");	\
	return m_Vec##ContainerName.at(iIdx);										\
	}\
	\
	bool Class::Contains##ContainerName(const Obj* p##ContainerName) const\
	{\
	return std::find(m_Vec##ContainerName.begin(), m_Vec##ContainerName.end(), p##ContainerName) != m_Vec##ContainerName.end();\
	}\
	bool Class::Is##ContainerName##Empty() const\
	{\
	return m_Vec##ContainerName.empty();\
	} \
	\
	void Class::Clear##ContainerName()\
	{\
	m_Vec##ContainerName.clear();\
	}


#endif