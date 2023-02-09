#ifndef MapLoader_h__
#define MapLoader_h__

namespace Gae
{
	namespace Core
	{
		namespace Universe
		{
			class Portal;
		}
	}

	namespace Entities
	{
		class Light;
	}

	namespace Graphics
	{
		class Mesh;
		class Material;
	}

	namespace Gifts
	{
		namespace Quake4
		{
			class BSPTree;

			class MapLoader
			{
				SINGLETON_DECL(MapLoader);

			public:
				typedef std::vector<Gae::Graphics::MultiMesh* > VecModel;
				typedef std::vector<Gae::Entities::Light* > VecLight;
				typedef std::vector<Gae::Core::Universe::Portal* > VecPortal;
				typedef std::vector<Gae::Graphics::Geometry::ShadowVolume* > VecShadowVolume;
				typedef std::vector< Vector3f > VecSpawnPoints;

				void LoadLevel(const std::wstring& lpwszFilename);
				inline VecModel& GetStaticGeometry();
				//inline const Vector3f& GetSpawnPosition() const;
				inline const VecSpawnPoints& GetSpawnPositions() const;

			private:
				enum E_TEXTURE_EXTENSION
				{
					TE_NONE = 0,
					TE_DIFFUSE,
					TE_SPECULAR,
					TE_BUMP,
				};
				// Material .mtr files used in this level map.
				typedef std::vector< const std::wstring > VecMaterialFilename;
				typedef std::vector< const std::wstring > VecParams;

				// Material map. Material's name <-> material object
				typedef std::map< UINT32/* mat. name's CRC32 */, Gae::Graphics::Material* > MapMaterial;

				// Guide map. Guide's name <-> Guide description for later parsing.
				// Material description map. Material's name <-> Material's description.
				typedef std::map< UINT32/* name's CRC32 */, std::wstring /* implementation */> MapGuide, MapMaterialDefinition;

				typedef std::map< UINT32/* func_static_. name's CRC32 */, Vector3f > MapFuncStaticOrigin;

#ifdef _DEBUG
				typedef std::map< std::wstring, VecParams > MapMaterialFiles;	// For debug purposes. For helping in debugging the parsing of the Material files it is useful to see if the materials are not being parsed correctly. In example to discover double material declarations that in theory it should never exists.
				typedef std::vector< Graphics::Material* > VecMaterial; // Debug purpose. 
#endif

				// ------------------- Parser tokens. -------------------
				// General
				static const wchar_t TAG_SPACE;
				static const wchar_t TAG_FILE_SEPARATOR;
				static const wchar_t TAG_PARAM_START;
				static const wchar_t TAG_PARAM_END;				
				static const wchar_t TAG_NEW_LINE;
				static const wchar_t TAG_STRING;
				static const wchar_t TAG_SENTENCE_START;
				static const wchar_t TAG_SENTENCE_END;

				static const std::wstring TAG_SENTENCE_START_STR;
				static const std::wstring TAG_SENTENCE_END_STR;

				// .guide Material guide
				static const std::wstring FILE_GUIDE_MATERIAL;
				static const std::wstring FILE_GUIDE_FLUIDS;
				static const std::wstring FILE_GUIDE_EFFECTS;
				static const std::wstring TAG_GUIDE;

				static const int TAG_GUIDE_CHARS;
				
				// .mtr Material files
				static const std::wstring TAG_MTR_BUMPMAP;
				static const std::wstring TAG_MTR_DIFFUSEMAP;
				static const std::wstring TAG_MTR_SPECULARMAP;
				static const std::wstring TAG_MTR_HEIGHTMAP;
				static const std::wstring TAG_MTR_TRANSLUCENT;
				static const std::wstring TAG_MTR_ALPHATEST;
				static const std::wstring TAG_MTR_BLEND;
				static const std::wstring TAG_MTR_MASKCOLOR;
				static const std::wstring TAG_MTR_MASKRED;
				static const std::wstring TAG_MTR_MASKGREEN;
				static const std::wstring TAG_MTR_MASKBLUE;
				static const std::wstring TAG_MTR_MASKALPHA;
				static const std::wstring TAG_MTR_MAKEALPHA;
				static const std::wstring TAG_MTR_ADD;
				static const std::wstring TAG_MTR_FILTER;
				static const std::wstring TAG_MTR_MODULATE;
				static const std::wstring TAG_MTR_NONE;
				static const std::wstring TAG_MTR_MAP;
				static const std::wstring TAG_MTR_ADD_NORMALS;
				static const std::wstring TAG_MTR_DOWNSIZE;
				static const std::wstring TAG_MTR_DECALMACRO;
				static const std::wstring TAG_MTR_POLYGONOFFSET;
				static const std::wstring TAG_MTR_SOLID;
				static const std::wstring TAG_MTR_TWOSIDED;
				static const std::wstring TAG_MTR_NONSOLID;

				static const std::wstring TAG_MTR_BT_GL_ONE;
				static const std::wstring TAG_MTR_BT_GL_ZERO;
				static const std::wstring TAG_MTR_BT_GL_DST_COLOR;
				static const std::wstring TAG_MTR_BT_GL_ONE_MINUS_DST_COLOR;
				static const std::wstring TAG_MTR_BT_GL_SRC_ALPHA;
				static const std::wstring TAG_MTR_BT_GL_ONE_MINUS_SRC_ALPHA;
				static const std::wstring TAG_MTR_BT_GL_DST_ALPHA;
				static const std::wstring TAG_MTR_BT_GL_ONE_MINUS_DST_ALPHA;
				static const std::wstring TAG_MTR_BT_GL_SRC_ALPHA_SATURATE;
				static const std::wstring TAG_MTR_BT_GL_SRC_COLOR;
				static const std::wstring TAG_MTR_BT_GL_ONE_MINUS_SRC_COLOR;

				// .proc file
				static const std::wstring TAG_PROPERTY_END;
				static const std::wstring TAG_MODEL;
				static const std::wstring TAG_MODEL_NAME;
				static const std::wstring TAG_SURFACE_COUNT;
				static const std::wstring TAG_SURFACE;
				static const std::wstring TAG_SURFACE_VERTEX_COUNT;
				static const std::wstring TAG_SURFACE_INDEX_COUNT;
				static const std::wstring TAG_VERTEX_START;
				static const std::wstring TAG_VERTEX_END;
				static const std::wstring TAG_NODES;
				static const std::wstring TAG_NODES_COUNT;
				static const std::wstring TAG_NODES_FIRST;
				static const std::wstring TAG_PORTALS;
				static const std::wstring TAG_PORTALS_PORTAL;
				static const std::wstring TAG_FUNC_STATIC;
				static const std::wstring TAG_FUNC;

				static const std::wstring TAG_SHADOWVOL;

				// .map file
				static const std::wstring TAG_MAP_PLAYER_SPAWN;
				static const std::wstring TAG_MAP_CLASSNAME;
				static const std::wstring TAG_MAP_NAME;
				static const std::wstring TAG_MAP_LIGHT;
				static const std::wstring TAG_MAP_ORIGIN;
				static const std::wstring TAG_MAP_COLOR;
				static const std::wstring TAG_MAP_LIGHT_RADIUS;
				static const std::wstring TAG_MAP_LIGHT_CENTER;

				// -------------------------------------------------------

				// ------------ Load methods --------------
				// .guide file
				std::wstring m_wsFilenameGuide;
				MapGuide m_MapGuide;
				void ParseGuide(Gae::Core::Util::Parser& ParserGuide);

				// .map file
				void ParseMap(Gae::Core::Util::Parser& parser);

				// .proc file
				void ParseProc(Gae::Core::Util::Parser& ParserProc);

				// .mtr files
				VecMaterialFilename m_VecMtrFilename;	// .mtr files to parse used in the .proc (level) file.
				// m_VecMtrFilename doesn't work so we need to add manually the whole *.mtr by hand...
				//VecMaterialFilename m_VecMtrFiles;

				MapMaterialDefinition m_MapMaterialDefinition;	//!< Map that holds a reference to each material text definition starting from the material's name.
				MapMaterial m_MapMaterial;	//!< Map that holds a reference to each Material object starting from the material's name.

				void ParseMtr(const std::wstring& wsFilename);	//!< Parse a .mtr file. It stores the material definition if the material's name was previously found in the .proc file.
				//std::wstring ExtractMtrFilename(const std::string& wsMatName);	// Return the .mtr filename starting from the material's name
				//void AddMtrFilename(const std::wstring& strFilename);	//!< Adds (if it doesn't exist) the material's (.mtr) filename to the list of .mtr files to be parsed.

				Gae::Graphics::Material*  GetMaterial(const std::wstring& strMatName);	// Adds an entry to the material's map if it doesn't exist and return it.
				void AddMaterialDefinition(UINT32 uMat, const std::wstring& strMaterialDefinition); //!< Adds the passed material definition into the map's material definition.
				std::wstring& GetExpandedMaterialGuide(UINT32 uGuide, const std::wstring& strMatName, const VecParams& vecParams); //!< Expand a material guide (it is like a macro) based on the given guide's parameters.

				// Material creation.
				void ProcessMaterials();	//!< Fill the Material objects with the correct parameters based on the material definition.
				bool AddTextureExtension(std::wstring& strFilename, const E_TEXTURE_EXTENSION eTE = TE_NONE); //!< Adds .tga or .dds extension if the texture doesn't exist.

				// Misc.
				inline bool FileExist(const std::wstring& wsFilename) const;

				// Lights config.
				void ConfigureStaticLights();

				// TBN calculation
				void GenerateTBN();	//!< Calculates the Tangent and Binormal vector of the triangle.

				// Returns a valid blend mode enum (Material::E_BLEND_TYPE)
				Graphics::Material::E_BLEND_TYPE GetBlendType(const std::wstring& wsBlendType) const;

				VecPortal m_VecPortal;

				MapFuncStaticOrigin m_MapFuncOrigin;

				VecModel m_VecModel;	//!< Vector of level's models. Created inside ParseProc.
				VecLight m_VecLight;	//!< Vector of level's models. Created inside ParseProc.
				VecShadowVolume m_VecShadowVolume;	//!< Vector of ShadowVolumes
				//Vector3f m_v3fSpawn;	//!< Player's spawn point.
				VecSpawnPoints m_VecSpawnPoints;	//!< Player's spawn points.
				
#ifdef _DEBUG
				void CheckModelsMaterial();
				UINT32 m_uVertexTotal;
				UINT32 m_uIndexTotal;
				UINT32 m_uPolygonTotal;
				UINT32 m_uSurfacesTotal;
				VecMaterial m_VecMatWrongBlendStageDefinition; //debug. holds a vector of materials with wrong blend stage definitions. To be implemented during the parsing.
#endif

				BSPTree* m_pBSP;


			};
		}
	}
}
#include "MapLoader.inl"
#endif // MapLoader_h__