#include "stdafx.h"

#include "MapLoader.h"

namespace Gae
{
	namespace Gifts
	{
		namespace Quake4
		{
			// general tags
			const wchar_t MapLoader::TAG_FILE_SEPARATOR			= L'/';
			const wchar_t MapLoader::TAG_PARAM_START			= L'(';
			const wchar_t MapLoader::TAG_PARAM_END				= L')';
			const wchar_t MapLoader::TAG_SPACE					= L' ';
			const wchar_t MapLoader::TAG_NEW_LINE				= L'\n';
			const wchar_t MapLoader::TAG_STRING					= L'\"';
			const wchar_t MapLoader::TAG_SENTENCE_START			= L'{';
			const wchar_t MapLoader::TAG_SENTENCE_END			= L'}';
			const std::wstring MapLoader::TAG_SENTENCE_START_STR	= L"{";
			const std::wstring MapLoader::TAG_SENTENCE_END_STR		= L"}";

			// .guide tags
			const std::wstring MapLoader::FILE_GUIDE_MATERIAL	= L"guides/material.guide";
			const std::wstring MapLoader::FILE_GUIDE_FLUIDS		= L"guides/effects.guide";
			const std::wstring MapLoader::FILE_GUIDE_EFFECTS	= L"guides/fluids.guide";

			const std::wstring MapLoader::TAG_GUIDE	= L"guide";
			

			const int MapLoader::TAG_GUIDE_CHARS	= sizeof(MapLoader::TAG_GUIDE);

			// .mtr Material tags
			const std::wstring MapLoader::TAG_MTR_BUMPMAP		= L"bumpmap";
			const std::wstring MapLoader::TAG_MTR_DIFFUSEMAP	= L"diffusemap";
			const std::wstring MapLoader::TAG_MTR_SPECULARMAP	= L"specularmap";
			const std::wstring MapLoader::TAG_MTR_HEIGHTMAP		= L"heightmap";
			const std::wstring MapLoader::TAG_MTR_TRANSLUCENT	= L"translucent";
			const std::wstring MapLoader::TAG_MTR_ALPHATEST		= L"alphatest";
			const std::wstring MapLoader::TAG_MTR_BLEND			= L"blend";
			const std::wstring MapLoader::TAG_MTR_MASKCOLOR		= L"maskcolor";
			const std::wstring MapLoader::TAG_MTR_MASKRED		= L"maskred";
			const std::wstring MapLoader::TAG_MTR_MASKGREEN		= L"maskgreen";
			const std::wstring MapLoader::TAG_MTR_MASKBLUE		= L"maskblue";
			const std::wstring MapLoader::TAG_MTR_MASKALPHA		= L"maskalpha";
			const std::wstring MapLoader::TAG_MTR_MAKEALPHA		= L"makealpha";
			const std::wstring MapLoader::TAG_MTR_ADD			= L"add";
			const std::wstring MapLoader::TAG_MTR_FILTER		= L"filter";
			const std::wstring MapLoader::TAG_MTR_MODULATE		= L"modulate";
			const std::wstring MapLoader::TAG_MTR_POLYGONOFFSET	= L"polygonOffset";
			const std::wstring MapLoader::TAG_MTR_SOLID			= L"solid";
			const std::wstring MapLoader::TAG_MTR_NONSOLID		= L"nonsolid";
			const std::wstring MapLoader::TAG_MTR_TWOSIDED		= L"twosided";
			const std::wstring MapLoader::TAG_MTR_NONE 			= L"none";

			const std::wstring MapLoader::TAG_MTR_BT_GL_ONE					= L"gl_one";
			const std::wstring MapLoader::TAG_MTR_BT_GL_ZERO				= L"gl_zero";
			const std::wstring MapLoader::TAG_MTR_BT_GL_DST_COLOR			= L"gl_dst_color";
			const std::wstring MapLoader::TAG_MTR_BT_GL_ONE_MINUS_DST_COLOR	= L"gl_one_minus_dst_color";
			const std::wstring MapLoader::TAG_MTR_BT_GL_SRC_ALPHA			= L"gl_src_alpha";
			const std::wstring MapLoader::TAG_MTR_BT_GL_ONE_MINUS_SRC_ALPHA	= L"gl_one_minus_src_alpha";
			const std::wstring MapLoader::TAG_MTR_BT_GL_DST_ALPHA			= L"gl_dst_alpha";
			const std::wstring MapLoader::TAG_MTR_BT_GL_ONE_MINUS_DST_ALPHA	= L"gl_one_minus_dst_alpha";
			const std::wstring MapLoader::TAG_MTR_BT_GL_SRC_ALPHA_SATURATE	= L"gl_src_alpha_saturate";
			const std::wstring MapLoader::TAG_MTR_BT_GL_SRC_COLOR			= L"gl_src_color";
			const std::wstring MapLoader::TAG_MTR_BT_GL_ONE_MINUS_SRC_COLOR	= L"gl_one_minus_src_color";

			const std::wstring MapLoader::TAG_MTR_MAP			= L"map";
			const std::wstring MapLoader::TAG_MTR_ADD_NORMALS	= L"addnormals";
			const std::wstring MapLoader::TAG_MTR_DOWNSIZE		= L"downsize";
			const std::wstring MapLoader::TAG_MTR_DECALMACRO	= L"decal_macro";

			// .proc tags
			const std::wstring MapLoader::TAG_PROPERTY_END		= L"*/";
			const std::wstring MapLoader::TAG_MODEL				= L"model";
			const std::wstring MapLoader::TAG_MODEL_NAME		= L"name";
			const std::wstring MapLoader::TAG_SURFACE_COUNT		= L"numSurfaces";
			const std::wstring MapLoader::TAG_SURFACE				= L"surface";
			const std::wstring MapLoader::TAG_SURFACE_VERTEX_COUNT	= L"numVerts";
			const std::wstring MapLoader::TAG_SURFACE_INDEX_COUNT	= L"numIndexes";
			const std::wstring MapLoader::TAG_NODES				= L"nodes";
			const std::wstring MapLoader::TAG_NODES_COUNT		= L"numNodes";
			const std::wstring MapLoader::TAG_NODES_FIRST		= L"node 0";
			const std::wstring MapLoader::TAG_PORTALS		= L"interAreaPortals";
			const std::wstring MapLoader::TAG_PORTALS_PORTAL= L"iap";
			const std::wstring MapLoader::TAG_FUNC_STATIC = L"func_static";
			const std::wstring MapLoader::TAG_FUNC = L"func_";

			const std::wstring MapLoader::TAG_SHADOWVOL	= L"shadowModel";

			// .map tags
			const std::wstring MapLoader::TAG_MAP_PLAYER_SPAWN	= L"info_player_start";
			const std::wstring MapLoader::TAG_MAP_CLASSNAME = L"classname";
			const std::wstring MapLoader::TAG_MAP_NAME = L"name";
			const std::wstring MapLoader::TAG_MAP_LIGHT = L"light";
			const std::wstring MapLoader::TAG_MAP_ORIGIN = L"origin";
			const std::wstring MapLoader::TAG_MAP_COLOR = L"_color";
			const std::wstring MapLoader::TAG_MAP_LIGHT_RADIUS = L"light_radius";
			const std::wstring MapLoader::TAG_MAP_LIGHT_CENTER = L"light_center";

			SINGLETON_IMPL(MapLoader);

			// Singleton interface
			bool MapLoader::Init()
			{
#ifdef _DEBUG
				m_uVertexTotal = 0;
				m_uPolygonTotal = 0;
				m_uIndexTotal = 0;
				m_uSurfacesTotal = 0;
#endif
				m_pBSP = NULL;
				return TRUE;
			}

			bool MapLoader::Uninit()
			{
				GAE_DELETESAFE(m_pBSP);
				return TRUE;
			}

			void MapLoader::ParseGuide(Gae::Core::Util::Parser& ParserGuide)
			{
				std::wstring strGuide;
				std::wstring strGuideName;
				int iNestedSentences = 0;
				ParserGuide.SetDelimitersToken(L"\n\t(){},");

				while (ParserGuide.GetNextToken(TAG_GUIDE) != Gae::Core::Util::Parser::TAG_END)
				{
					//strGuide.append(ParserGuide.GetToken());
					strGuide.append(ParserGuide.GetNextToken());
					strGuideName.append(ParserGuide.GetToken());

					// Reads until the end of the params
					while(ParserGuide.GetNextDelimiter() != TAG_PARAM_END)
					{
						strGuide.append(1, ParserGuide.GetDelimiter());
						strGuide.append(ParserGuide.GetNextToken());
					}
					strGuide.append(1, ParserGuide.GetDelimiter());	// Adds the last TAG_PARAM_END delimiter

					do 
					{	// Stores until we find the first TAG_SENTENCE_START char
						strGuide.append(1, ParserGuide.GetCurrentChar());
					} while(ParserGuide.GetNextChar() != TAG_SENTENCE_START);

					GAE_ASSERTM(ParserGuide.GetCurrentChar() == TAG_SENTENCE_START, L"{ expected");
					do	// Start counting nested sentences. We will stop when the counter
					{	// reaches 0.
						strGuide.append(1, ParserGuide.GetCurrentChar());

						if(ParserGuide.GetCurrentChar() == TAG_SENTENCE_END)
						{
							--iNestedSentences;
						}
						else if(ParserGuide.GetCurrentChar() == TAG_SENTENCE_START)
						{
							++iNestedSentences;
						}
					} while (iNestedSentences && ParserGuide.GetNextChar() != Gae::Core::Util::Parser::TAG_CHAR_END);

					// Lets store the guide declaration into the map.
					GAE_ASSERTM(m_MapGuide.find(Gae::Core::Util::CRC32(strGuideName)) == m_MapGuide.end(), L"Error! The guide shouldn't be in the map already!\n");
					m_MapGuide[Gae::Core::Util::CRC32(strGuideName)] = strGuide;
#ifdef _DEBUG
					{	// Parsed guide
						std::wstring wsParsedGuide(strGuide.length(), L' ');
						std::copy(strGuide.begin(), strGuide.end(), wsParsedGuide.begin());

						GAE_TRACE(L"Parsed file: \n%s\n", wsParsedGuide.c_str());
					}
#endif
					strGuide.clear();
					strGuideName.clear();
				}

				ParserGuide.Reset();
			}

			void MapLoader::ParseMtr(const std::wstring& wsFilename)
			{
				using namespace Gae::Core::Util;

				// Check if the file exists.
				bool bFileExist = FileExist(wsFilename);
				GAE_ASSERTM(bFileExist, L"The file can not be opened: [%s]\n", wsFilename.c_str());
				if(!bFileExist) return;

				// Create the required textures for each material file by using
				// the material's map.
				Parser parser;
				parser.UseFile(wsFilename, TRUE);
				parser.SetDelimitersToken(L"=*(){},\"\n\t");

				const std::wstring& strToken = parser.GetToken();	// It will hold temporary tokens.
				std::wstring strMaterialDefinition;	// It will hold a full material definition.
				MapMaterial::iterator itFound = m_MapMaterial.end();

				while(true)
				{
					// Skips until we find a "guide" declaration, added material declaration or reaches the end of the file.
					/*
					while(parser.GetNextToken() != Parser::TAG_END
						&& 
						strToken != TAG_GUIDE
						&& ((itFound = m_MapMaterial.find(CRC32(strToken))) == m_MapMaterial.end())
					);
					*/

					parser.SetDelimitersToken(L"=*(),\"\n\t");
					int iSentenceNumber = 0;	// Counter for nested sentences.
					do
					{	// Gets the next material.
						do 
						{
							parser.GetNextToken();

							if(strToken == TAG_SENTENCE_START_STR)
							{
								++iSentenceNumber;
							}
							else if(strToken == TAG_SENTENCE_END_STR)
							{
								--iSentenceNumber;
							}
						} while (iSentenceNumber && /*(iSentenceNumber != 0 && */strToken != Parser::TAG_END)/*)*/;
					} while ((itFound = m_MapMaterial.find(CRC32(strToken))) == m_MapMaterial.end() && (strToken != TAG_GUIDE) && (strToken != Parser::TAG_END));

					if(strToken == Parser::TAG_END) return;
					UINT32 uMat = 0;

 
					parser.SetDelimitersToken(L"=*(){},\"\n\t");
					if(strToken != TAG_GUIDE)
					{	// Material declaration. Store it directly. 
						std::wstring strMatName = strToken;
						uMat = CRC32(strToken);	// Gets material's name.

#ifdef _DEBUG
						static MapMaterialFiles mapMtrFiles;
						mapMtrFiles[strMatName];
						mapMtrFiles[strMatName].push_back(wsFilename);

						bool bMatExist = !(m_MapMaterialDefinition.find(uMat) == m_MapMaterialDefinition.end());
						GAE_ASSERTM(!bMatExist, L"Incorrect! Only one material definition should exist!\nContinue to see the duplicated entries.\n");
						if(bMatExist)
						{
							GAE_TRACE(L"Material: [%s]\n", strMatName.c_str());
							for each(const std::wstring& strFile in mapMtrFiles[strMatName])
							{
								GAE_TRACE(L"\tFile:\t[%s]\n", strFile.c_str());
							}
							GAE_ASSERTM(false, L"Take a look at the .mtr files now.\n");
						}
#endif
						strMaterialDefinition.append(strToken);
						
#ifdef _DEBUG
						//GAE_ASSERTM(strToken != L"_default", L"Debug!");
#endif
						do 
						{	// Stores until we find the first TAG_SENTENCE_START char
							strMaterialDefinition.append(1, parser.GetCurrentChar());
						} while(parser.GetNextChar() != TAG_SENTENCE_START);
						strMaterialDefinition.append(1, parser.GetCurrentChar());

						iSentenceNumber = 1;
						do 
						{
							strMaterialDefinition.append(1, parser.GetNextChar());

							if(parser.GetCurrentChar() == TAG_SENTENCE_START)
							{
								++iSentenceNumber;
							}
							else if(parser.GetCurrentChar() == TAG_SENTENCE_END)
							{
								--iSentenceNumber;
							}
						} while (iSentenceNumber);
						parser.GetNextChar();	// If now we call parser.GetNextToken() and the curling brace is not set in the parser's delimiters the parser will return again the curling brace because GetNextChar method returns the next character in the string and leaves the read pointer in that character. GetNextToken start looking for the token at the current read pointer's position.
					}
					else 
					{	// Guide declaration. Now we have to instantiate each guide declaration.
						GAE_ASSERTM(parser.GetToken() == TAG_GUIDE, L"It is not a guide!!! Wwwwhat is it then? DEBUG!!!!: [%s]\n");
						std::wstring strMatName = parser.GetNextToken();
						uMat = CRC32(strMatName);	// Gets material's name.
						UINT32 uGuide = CRC32(parser.GetNextToken());	// Gets guide's name
						GAE_ASSERTM(m_MapMaterialDefinition.find(uMat) == m_MapMaterialDefinition.end(), L"Incorrect! Only one material definition should exist!\n");
						GAE_ASSERTM(m_MapGuide.find(uGuide) != m_MapGuide.end(), L"Error!!! The guide was not parsed before!!! Panic!!! noooooo!!!! :'(\n\tCheck the ParseGuide method.\n");

						if(m_MapGuide.find(uGuide) != m_MapGuide.end())
						{
							// Extract parameters
							VecParams vecParams;
							do
							{
								// Sometimes it is not an String...
								//vecParams.push_back(parser.GetNextString());
								vecParams.push_back(parser.GetNextToken());
								// skips the quotes since some parameters has quotes and others doesn't
								while(parser.GetCurrentChar() == TAG_STRING && parser.GetNextChar() == TAG_STRING);
							} while(/*parser.GetChar() != TAG_PARAM_END && */parser.GetNextDelimiter() != TAG_PARAM_END);

							strMaterialDefinition = GetExpandedMaterialGuide(uGuide, strMatName, vecParams);
						}
						else
						{
							GAE_ASSERTM(false, L"You should never see this!");
						}
					}

					AddMaterialDefinition(uMat, strMaterialDefinition);
					strMaterialDefinition.clear();
				}
			}

			void MapLoader::AddMaterialDefinition(UINT32 uMat, const std::wstring& strMaterialDefinition)
			{
				if(strMaterialDefinition.empty()) return;

				m_MapMaterialDefinition[uMat] = strMaterialDefinition;

				GAE_TRACE(L"\nAdded material definition:\n%s\n", strMaterialDefinition.c_str());
			}

			std::wstring& MapLoader::GetExpandedMaterialGuide(UINT32 uGuide, const std::wstring& strMatName, const VecParams& vecParams)
			{
				using namespace Gae::Core::Util;
				GAE_ASSERTM(m_MapGuide.find(uGuide) != m_MapGuide.end(), L"Error!!! The guide was not parsed before!!! Panic!!! noooooo!!!! :'( \n");

				static std::wstring strExpanded;	// we do not want to make two copies of the object when returning it...
				strExpanded.clear();
				Gae::Core::Util::Parser parser;
				parser.UseString(m_MapGuide[uGuide]);
				parser.SetDelimitersToken(L"(){},");

				VecParams vecGuideParamTags;
				// Retrieve the parameters from the guide's description.

				while(parser.GetNextDelimiter() != TAG_PARAM_END)
				{
					vecGuideParamTags.push_back(parser.GetNextToken());
				}
				// Check the given parameters match the parameter count of the guide's description.
				GAE_ASSERTM(vecGuideParamTags.size() == vecParams.size(), L"Incorrect parameter count! Check the guide parsing!\n");

				for(UINT32 i = 0; i < vecParams.size(); ++i)
				{
					parser.Reset();
					parser.GetNextToken(vecGuideParamTags.at(i));
					parser.ReplaceAll(vecParams.at(i));
				}
				parser.Reset();

				// Once we made the whole replacement we copy the required content
				// into the return string.

				strExpanded.append(strMatName);
				strExpanded.append(1, parser.GetNextDelimiter(TAG_NEW_LINE));
				strExpanded.append(1, parser.GetNextDelimiter(TAG_SENTENCE_START));

				do 
				{
					strExpanded.append(1, parser.GetCurrentChar());
				} while (parser.GetNextChar() != Parser::TAG_CHAR_END);
				strExpanded.append(1, parser.GetCurrentChar());

				GAE_TRACE(L"\nExpanded guide material:\n%s\n", strExpanded.c_str());
				return strExpanded;

			}

			Gae::Graphics::Material* MapLoader::GetMaterial(const std::wstring& strMatName)
			{
				// Creates the material if it doesn't exist yet and return it. The material will be later configured. This method only creates the object.
				GAE_ASSERTM(strMatName != TAG_GUIDE, L"Trying to add a guide material instead of a real material!!!");

				UINT32 uMatNameCRC32 = Gae::Core::Util::CRC32(strMatName.c_str());

				//GAE_ASSERTM(m_MapMaterial.find(uMatNameCRC32) != m_MapMaterial.end(), L"Material doesn't exist: [%s]\n", strMatName.c_str());
				
				if(m_MapMaterial.find(uMatNameCRC32) == m_MapMaterial.end())
				{
					m_MapMaterial[uMatNameCRC32] = new Gae::Graphics::Material;
					//m_MapMaterial[uMatNameCRC32]->m_wsTAG = strMatName;
					m_MapMaterial[uMatNameCRC32]->SetTAG(strMatName);

					// some default params
					m_MapMaterial[uMatNameCRC32]->FlagSet(Gae::Graphics::Material::M_SOLID);
				}

				return m_MapMaterial[uMatNameCRC32];
			}

			void MapLoader::ProcessMaterials()
			{
				MapMaterial::iterator itMatObj = m_MapMaterial.begin();

				for(MapMaterial::iterator it = m_MapMaterial.begin(), itEnd = m_MapMaterial.end(); it != itEnd; ++it)
				{
					using namespace Gae::Core::Util;
					using namespace Gae::Graphics;

					Material* pMat = it->second;

					GAE_TRACE(L"Processing material: %s\n", pMat->GetTAG().c_str());
					//GAE_ASSERTM(pMat->GetTAG() != L"textures/base_light/striplight2_break", L"Debug!");

					//GAE_ASSERTM(m_MapMaterialDefinition.find(it->first) != m_MapMaterialDefinition.end(), L"The material wasn't populated in the ParseMtr method!!");

					// If the material doesn't exist at least there must be a texture with a .tga
					// or .dds extension to be used directly as a diffuse map.
					// TODO: Create a default resource for this case.
					if(m_MapMaterialDefinition.find(it->first) == m_MapMaterialDefinition.end())
					{	
						GAE_WARNINGM(false, L"Material definition doesn't exist!!!\nTrying getting the textures with the same material's name.");
						// Try to look if a texture with the same filename exists for this non-existent material.
						//std::wstring wsTextureFilename = pMat->m_wsTAG;
						std::wstring wsTextureFilename = pMat->GetTAG();
						if(AddTextureExtension(wsTextureFilename, TE_DIFFUSE))
						{
							Texture* pTexture = new Texture;
							pTexture->SetFilename(wsTextureFilename);
							pMat->SetMapDiffuse(pTexture);
						}
						else if(AddTextureExtension(wsTextureFilename, TE_NONE))
						{
							Texture* pTexture = new Texture;
							pTexture->SetFilename(wsTextureFilename);
							pMat->SetMapDiffuse(pTexture);
						}

						//wsTextureFilename = pMat->m_wsTAG;
						wsTextureFilename = pMat->GetTAG();
						if(AddTextureExtension(wsTextureFilename, TE_SPECULAR))
						{
							Texture* pTexture = new Texture;
							pTexture->SetFilename(wsTextureFilename);
							pMat->SetMapSpecular(pTexture);
						}

						//wsTextureFilename = pMat->m_wsTAG;
						wsTextureFilename = pMat->GetTAG();
						if(AddTextureExtension(wsTextureFilename, TE_BUMP))
						{
							Texture* pTexture = new Texture;
							pTexture->SetFilename(wsTextureFilename);
							pMat->SetMapNormal(pTexture);
						}
					}
					else
					{
						// Initializes the parser with the material plain text content.
						Parser parser;
						parser.UseString(m_MapMaterialDefinition[it->first]);
						parser.SetDelimitersToken(L"\n\t(),");

						std::wstring strBumpMap;
						std::wstring strDiffuseMap;
						std::wstring strSpecularMap;
						std::wstring strQerImageEditor;	// It is only used in the case we do not find any valid diffuse texture. The QerImageEditor parameter will be the diffuse instead.

						// \note strToken will always have a reference to the last token found by the parser starting from now on.
						const std::wstring& strToken = parser.GetNextToken(TAG_SENTENCE_START_STR); // Advance to the main sentence. This is just after the Material's name.
						/* Typical material's definition
							textures/command_ship/csa_wall13c
							{
								qer_EditorImage	textures/command_ship/csa_wall13b_d.tga
								bumpMap			textures/command_ship/csa_wall13b_local.tga
								diffuseMap		textures/command_ship/csa_wall13b_d.tga
								specularMap		textures/command_ship/csa_wall13b_s.tga
								{
									blend		add
									map			textures/command_ship/csa_wall13b_add.tga
									red			parm0
									green		parm1
									blue		parm2
								}
							}
						*/
						while(parser.GetNextToken() != Parser::TAG_END)
						{	// Search for the keywords.
							if(strToken == TAG_SENTENCE_START_STR || strToken == L"{if")	// The "{if" is a hack.
							{	// Blend stage. The next token MUST be a "blend" keyword.

								// Look for the "blend" keyword.
								//while(parser.GetNextToken() != TAG_MTR_BLEND && strToken != TAG_SENTENCE_END_STR);
								//GAE_ASSERTM(0 == strToken.compare(TAG_MTR_BLEND), L"Wrong blend stage definition!\nMaterial:\n%s", m_MapMaterialDefinition[it->first].c_str());

								bool bMaskColor = false;
								bool bMaskRed = false;
								bool bMaskGreen = false;
								bool bMaskBlue = false;
								bool bMaskAlpha = false;
								while(parser.GetNextToken() != TAG_SENTENCE_END_STR)
								{
#ifdef _DEBUG
									if(std::find(m_VecMatWrongBlendStageDefinition.begin(), m_VecMatWrongBlendStageDefinition.end(), pMat) == m_VecMatWrongBlendStageDefinition.end())
									{
										//GAE_ASSERTM(strToken == TAG_MTR_BLEND || strToken == TAG_MTR_MASKCOLOR || strToken == TAG_MTR_MASKALPHA, L"Wrong blend stage definition!\nMaterial:\n%s", m_MapMaterialDefinition[it->first].c_str());
										if (strToken != TAG_MTR_BLEND && strToken != TAG_MTR_MASKCOLOR && strToken != TAG_MTR_MASKALPHA)
										{
											GAE_TRACE(L"Unsupported blend stage definition!\nMaterial:\n%s", m_MapMaterialDefinition[it->first].c_str());
										}
										m_VecMatWrongBlendStageDefinition.push_back(pMat);
									}
									else
									{
										continue;
									}
#endif
									if(strToken == TAG_MTR_MASKCOLOR)
									{
										bMaskColor = true;

										parser.GetNextToken();	// Map keyword
										GAE_ASSERTM(strToken == TAG_MTR_MAP, L"Map keyword expected!");


										bool bMakeAlpha = false;
										if(strToken == TAG_MTR_MAP)
										{
											parser.GetNextToken();	// Map's diffuse texture name or makealpha usually.
											if(strToken == TAG_MTR_MAKEALPHA)
											{
												bMakeAlpha = true;
												parser.GetNextToken();	// This line should retrieve the filename
											}
											std::wstring wsFilename = strToken; 
											if(AddTextureExtension(wsFilename))
											{
												Material::E_BLEND_TYPE eBlendSrc = Material::BT_ONE;
												Material::E_BLEND_TYPE eBlendDst = Material::BT_ONE;

												// Lets create the blending stage.
												Material::BlendStage* pBlendStage = new Material::BlendStage;
												pBlendStage->m_dwBlendSrc = Material::GetRendererBlendValue(eBlendSrc);
												pBlendStage->m_dwBlendDst = Material::GetRendererBlendValue(eBlendDst);

												Texture* pTexture = new Texture;
												pTexture->SetFilename(wsFilename);

												pBlendStage->m_pTexture = pTexture;

												// Check for additional values. Alpha testing, etc...
												while(parser.GetNextToken() != TAG_SENTENCE_END_STR)
												{
													if(strToken == TAG_MTR_ALPHATEST)
													{
														//pMat->SetAlphaTestValue(TRUE);
														pMat->FlagSet(Material::M_ALPHATEST);
														pMat->SetAlphaTestValue(((float) _wtof(parser.GetNextNumber().c_str())) * 255.0f);
													}
												}

												// Add the blend stage to the material.
												pMat->AddBlendStage(pBlendStage);
												pMat->FlagSet(Material::M_BLEND);
												pBlendStage->m_bMakeAlpha = bMakeAlpha;

												if(bMaskColor)
												{
													pBlendStage->m_dwMaskComponent &= ~(1 << 0);	// Red
													pBlendStage->m_dwMaskComponent &= ~(1 << 1);	// Green
													pBlendStage->m_dwMaskComponent &= ~(1 << 2);	// Blue
												}

												if(bMaskAlpha)
												{
													pBlendStage->m_dwMaskComponent &= ~(1 << 3);	// Alpha
												}

											}
											else
											{
												GAE_ASSERTM(false, L"The filename and none of its variants doesn't exist: [%s]", wsFilename.c_str());
											}
										}

									}
									else if(strToken == TAG_MTR_MASKALPHA)
									{
										bMaskAlpha = true;
									}
									else if(strToken == TAG_MTR_BLEND)
									{	/* Extracted from www.iddevnet.com
										link: http://www.iddevnet.com/doom3/materials.php

										blend		gl_src_alpha	gl_one_minus_src_alpha
										add			gl_one			gl_one
										filter		gl_dst_color	gl_zero
										modulate	gl_dst_color	gl_zero
										none		gl_zero			gl_one
										bumpmap		Normal map
										diffusemap	Diffuse map
										specularmap	Specular map
										*/
										if(parser.GetNextToken() == TAG_MTR_BUMPMAP)
										{
											// TODO: Implement the heightmap method
											GAE_ASSERTM(NULL == pMat->GetMapNormal(), L"Error! The bump map was already created!!!\nConsider to add more rendering stages for the materials...\nMaterial:\n%s", m_MapMaterialDefinition[it->first].c_str());

											if(NULL == pMat->GetMapNormal())
											{
												parser.GetNextToken();
												GAE_ASSERTM(strToken == TAG_MTR_MAP, L"Wrong blend bumpmap definition!\n\tExpected: map\n");

												// TODO: Implement the addnormals method here.
												if(strToken == TAG_MTR_MAP)
												{
													parser.GetNextToken(); // Gets the next token: Addnormal/Heightmap method or texture file.
													if(strToken == TAG_MTR_ADD_NORMALS)
													{
														// TODO: call the addnormals method here.

														// At the moment we only load the _local.tga file here instead of computing
														// a normal map starting from a height map.
														parser.GetNextToken();

														Texture* pTexture = new Texture;
														std::wstring wsFilename = strToken;
//														// TODO - mirar bien esto porque parece que no se recupera la extensión correcta de la textura
														AddTextureExtension(wsFilename);
														GAE_ASSERTM(FileExist(wsFilename), L"File doesn't exist!! PANIC!!!");
														pTexture->SetFilename(wsFilename);
														pMat->SetMapNormal(pTexture);

														parser.GetNextToken();

														if(strToken == TAG_MTR_HEIGHTMAP)
														{
															// TODO: call the heightmap method here. <- Creates a normalmap starting from an heightmap.

															// At the moment we only load the _local.tga file here instead of computing
															// a normal map starting starting from an height map.

															parser.GetNextToken();

															Texture* pTexture = new Texture;
															std::wstring wsFilename = strToken;
															AddTextureExtension(wsFilename);
															GAE_ASSERTM(FileExist(wsFilename), L"File doesn't exist!! PANIC!!!");
															pTexture->SetFilename(wsFilename);
															pMat->SetMapHeight(pTexture);

															// \TODO: Check if this is ok. Although I have never found a bumpmap blend definition with anything else than a map.
															parser.GetNextToken(TAG_SENTENCE_END_STR);
														}
													}
													else if(strToken == TAG_MTR_HEIGHTMAP)
													{
														// TODO: call the heightmap method here. <- Creates a normalmap starting from an heightmap.

														// At the moment we only load the _local.tga file here instead of computing
														// a normal map starting starting from an height map.

														parser.GetNextToken();

														Texture* pTexture = new Texture;
														std::wstring wsFilename = strToken;
														AddTextureExtension(wsFilename);
														GAE_ASSERTM(FileExist(wsFilename), L"File doesn't exist!! PANIC!!!");
														pTexture->SetFilename(wsFilename);
														pMat->SetMapHeight(pTexture);

														// \TODO: Check if this is ok. Although I have never found a bumpmap blend definition with anything else than a map.
														parser.GetNextToken(TAG_SENTENCE_END_STR);
													}
													else
													{
														Texture* pTexture = new Texture;
														std::wstring wsFilename = strToken;
														AddTextureExtension(wsFilename);
														GAE_ASSERTM(FileExist(wsFilename), L"File doesn't exist!! PANIC!!!");
														pTexture->SetFilename(wsFilename);
														pMat->SetMapNormal(pTexture);
													}
												}

												//pMat->FlagSet(Material::M_BLEND);
											}

											//parser.GetNextToken(TAG_SENTENCE_END_STR);
										}
										else if(strToken == TAG_MTR_DIFFUSEMAP)
										{
											GAE_ASSERTM(NULL == pMat->GetMapDiffuse(), L"Error! The diffuse map was already created!!!\nConsider to add more rendering stages for the materials...\nMaterial:\n%s", m_MapMaterialDefinition[it->first].c_str());

											if(NULL == pMat->GetMapDiffuse())
											{
												parser.GetNextToken();
												GAE_ASSERTM(strToken == TAG_MTR_MAP, L"Wrong blend diffusemap definition!\n\tExpected: map\n");

												parser.GetNextToken();

												// Ignore the "downsize" token if it exists. I couldn't find any doc in www.iddevnet.com
												if(strToken == TAG_MTR_DOWNSIZE)
												{
													parser.GetNextToken();
												}

												/*
												if(!AddTextureExtension(wsFilename))
												{
												wsFilename = pMat->m_wsTAG;
												wsFilename.append(L".tga");
												GAE_ASSERTM(FileExist(wsFilename), L"File doesn't exist!! PANIC!!!");
												}
												*/
												std::wstring wsFilename = strToken;
												if(AddTextureExtension(wsFilename))
												{
													Texture* pTexture = new Texture;
													pTexture->SetFilename(wsFilename);
													pMat->SetMapDiffuse(pTexture);

													// Check for additional values
													while(parser.GetNextToken() != TAG_SENTENCE_END_STR)
													{
														if(strToken == TAG_MTR_ALPHATEST)
														{
															pMat->FlagSet(Material::M_ALPHATEST);
															pMat->SetAlphaTestValue(((float) _wtof(parser.GetNextNumber().c_str())) * 255.0f);
														}
													}

													//pMat->FlagSet(Material::M_BLEND);
												}
											}

											// Skips until the end of the blend sentence
											//parser.GetNextToken(TAG_SENTENCE_END_STR);

										}
										else if(strToken == TAG_MTR_SPECULARMAP)
										{
											GAE_ASSERTM(NULL == pMat->GetMapSpecular(), L"Error! The specular map was already created!!!\nConsider to add more rendering stages for the materials...\nMaterial:\n%s", m_MapMaterialDefinition[it->first].c_str());

											if(NULL == pMat->GetMapSpecular())
											{
												parser.GetNextToken();
												GAE_ASSERTM(strToken == TAG_MTR_MAP, L"Wrong blend specularmap definition!\n\tExpected: map\n");

												parser.GetNextToken();

												// Ignore the "downsize" token if it exists. I couldn't find any doc in www.iddevnet.com
												if(strToken == TAG_MTR_DOWNSIZE)
												{
													parser.GetNextToken();
												}

												Texture* pTexture = new Texture;
												std::wstring wsFilename = strToken;
												AddTextureExtension(wsFilename);
												GAE_ASSERTM(FileExist(wsFilename), L"File doesn't exist!! PANIC!!!");
												pTexture->SetFilename(wsFilename);
												pMat->SetMapSpecular(pTexture);

												//pMat->FlagSet(Material::M_BLEND);
											}

											// Skips until the end of the blend sentence
											//parser.GetNextToken(TAG_SENTENCE_END_STR);

										}
										else 
										{
											Material::E_BLEND_TYPE eBlendSrc;
											Material::E_BLEND_TYPE eBlendDst;

											if(strToken == TAG_MTR_BLEND)
											{	// blend	gl_src_alpha	gl_one_minus_src_alpha
												//pMat->SetBlendSrcDst(Material::BT_SRC_ALPHA, Material::BT_ONE_MINUS_SRC_ALPHA);
												eBlendSrc = Material::BT_SRC_ALPHA;
												eBlendDst = Material::BT_ONE_MINUS_SRC_ALPHA;
											}
											else if(strToken == TAG_MTR_ADD)
											{	// add	gl_one	gl_one
												//pMat->SetBlendSrcDst(Material::BT_ONE, Material::BT_ONE);
												eBlendSrc = Material::BT_ONE;
												eBlendDst = Material::BT_ONE;
											}
											else if(strToken == TAG_MTR_FILTER)
											{	// filter	gl_dst_color	gl_zero
												//pMat->SetBlendSrcDst(Material::BT_DST_COLOR, Material::BT_ZERO);
												eBlendSrc = Material::BT_DST_COLOR;
												eBlendDst = Material::BT_ZERO;
											}
											else if(strToken == TAG_MTR_MODULATE)
											{	// modulate	gl_dst_color	gl_zero
												//pMat->SetBlendSrcDst(Material::BT_DST_COLOR, Material::BT_ZERO);
												eBlendSrc = Material::BT_DST_COLOR;
												eBlendDst = Material::BT_ZERO;
											}
											else if(strToken == TAG_MTR_NONE)
											{	// none	gl_zero	gl_one
												//pMat->SetBlendSrcDst(Material::BT_ZERO, Material::BT_ONE);
												eBlendSrc = Material::BT_ZERO;
												eBlendDst = Material::BT_ONE;
											}
											else
											{	// Looks for the combination of SourceBlend and DestinationBlend
												//pMat->SetBlendSrcDst(GetBlendType(strToken), GetBlendType(parser.GetNextToken()));
												eBlendSrc = GetBlendType(strToken);
												eBlendDst = GetBlendType(parser.GetNextToken());
											}
											/*
											#ifdef _DEBUG
											else
											{
											GAE_ASSERTM(false, L"No blend type supported at the moment: %s\n", strToken.c_str());
											// Skip until the end of the blend pass
											parser.GetNextToken(TAG_SENTENCE_END_STR);
											}
											#endif
											*/

											// Advance to the map keyword or to the end of the sentence.
											//while(parser.GetNextToken() != TAG_MTR_MAP && strToken != TAG_SENTENCE_END_STR);

											//GAE_ASSERTM(strToken == TAG_MTR_MAP, L"Next token must be a MAP keyword!!! Unsupported blending stage!!! Token: [%s]\nMaterial:\n%s", strToken.c_str(), m_MapMaterialDefinition[it->first].c_str());

											// Lets create the blending stage.
											Material::BlendStage* pBlendStage = new Material::BlendStage;
											pBlendStage->m_dwBlendSrc = Material::GetRendererBlendValue(eBlendSrc);
											pBlendStage->m_dwBlendDst = Material::GetRendererBlendValue(eBlendDst);

											while(parser.GetNextToken() != TAG_SENTENCE_END_STR)
											{
												if(strToken == TAG_MTR_MASKCOLOR)
												{
													bMaskColor = true;
												}
												else if(strToken == TAG_MTR_MASKALPHA)
												{
													bMaskAlpha = true;
												}
												else if(strToken == TAG_MTR_MAP)
												{
													parser.GetNextToken();	// Map's diffuse texture name

													bool bMakeAlpha = false;
													if(strToken == TAG_MTR_MAKEALPHA)
													{
														bMakeAlpha = true;
														parser.GetNextToken();	// This line should retrieve the filename
													}
													std::wstring wsFilename = strToken; 
													if(AddTextureExtension(wsFilename))
													{
														Texture* pTexture = new Texture;
														pTexture->SetFilename(wsFilename);
														//pMat->SetMapDiffuse(pTexture);
														pBlendStage->m_pTexture = pTexture;

														// Check for additional values. Alpha testing, etc...
														while(parser.GetNextToken() != TAG_SENTENCE_END_STR)
														{
															if(strToken == TAG_MTR_ALPHATEST)
															{
																//pMat->SetAlphaTestValue(TRUE);
																pMat->FlagSet(Material::M_ALPHATEST);
																pMat->SetAlphaTestValue(((float) _wtof(parser.GetNextNumber().c_str())) * 255.0f);
															}
														}

														// Add the blend stage to the material.
														pMat->AddBlendStage(pBlendStage);
														pMat->FlagSet(Material::M_BLEND);
														pBlendStage->m_bMakeAlpha = bMakeAlpha;
													}
													else
													{
														GAE_ASSERTM(false, L"The filename and none of its variants doesn't exist: [%s]", wsFilename.c_str());
														GAE_DELETESAFE(pBlendStage);
													}
												}

												if(strToken == TAG_SENTENCE_END_STR) break;
											}

											if(pBlendStage)
											{
												if(bMaskColor)
												{
													pBlendStage->m_dwMaskComponent &= ~(1 << 0);	// Red
													pBlendStage->m_dwMaskComponent &= ~(1 << 1);	// Green
													pBlendStage->m_dwMaskComponent &= ~(1 << 2);	// Blue
												}

												if(bMaskAlpha)
												{
													pBlendStage->m_dwMaskComponent &= ~(1 << 3);	// Alpha
												}
											}
										}
										/*
										#ifdef _DEBUG
										else
										{
										GAE_TRACE(L"No blend type supported at the moment: %s\n", strToken.c_str());
										// Skip until the end of the blend pass
										parser.GetNextToken(TAG_SENTENCE_END_STR);
										}
										#endif
										*/
									}
									/*
									else
									{
										//GAE_ASSERTM(false, L"Wrong blend stage definition!");
										GAE_TRACE(L"Wrong blend stage definition!\n");
									}
									*/
									if(strToken == TAG_SENTENCE_END_STR) break;
								}
							}
							else if(strToken == TAG_MTR_POLYGONOFFSET)
							{
								pMat->SetPolygonOffset(1);
							}
							else if(strToken == TAG_MTR_SOLID)
							{
								pMat->FlagSet(Material::M_SOLID);
							}
							else if(strToken == TAG_MTR_NONSOLID)
							{
								pMat->FlagUnset(Material::M_SOLID);
							}
							else if(strToken == TAG_MTR_TWOSIDED)
							{
								pMat->FlagSet(Material::M_TWOSIDED);
							}
							else if(strToken == TAG_MTR_DECALMACRO)
							{
								pMat->FlagSet(Material::M_DECAL);
								pMat->SetPolygonOffset(1);
								pMat->FlagUnset(Material::M_SOLID);
							}
							else if(strToken == TAG_MTR_TRANSLUCENT)
							{
								pMat->FlagSet(Material::M_BLEND);
								pMat->FlagUnset(Material::M_SOLID);
							}
							else if(strToken == L"qer_editorimage")
							{
								strQerImageEditor = parser.GetNextToken();
							}
							else if(strToken == TAG_MTR_DIFFUSEMAP)
							{
								GAE_ASSERTM(NULL == pMat->GetMapDiffuse(), L"Error! The diffuse map was already created!!!\n");

								parser.GetNextToken();

								// Ignore the "downsize" token if it exists. I couldn't find any doc in www.iddevnet.com
								if(strToken == TAG_MTR_DOWNSIZE)
								{
									parser.GetNextToken();
								}

								std::wstring wsFilename = strToken;
								if(AddTextureExtension(wsFilename))
								{
									Texture* pTexture = new Texture;
									pTexture->SetFilename(wsFilename);
									pMat->SetMapDiffuse(pTexture);
								}

							}
							else if(strToken == TAG_MTR_SPECULARMAP)
							{
								GAE_ASSERTM(NULL == pMat->GetMapSpecular(), L"Error! The specular map was already created!!!\n");

								parser.GetNextToken();

								// Ignore the "downsize" token if it exists. I couldn't find any doc in www.iddevnet.com
								if(strToken == TAG_MTR_DOWNSIZE)
								{
									parser.GetNextToken();
								}

								Texture* pTexture = new Texture;
								std::wstring wsFilename = strToken;
								AddTextureExtension(wsFilename);
								GAE_ASSERTM(FileExist(wsFilename), L"File doesn't exist!! PANIC!!!");
								pTexture->SetFilename(wsFilename);
								pMat->SetMapSpecular(pTexture);
							}
							else if(strToken == TAG_MTR_BUMPMAP)
							{
								GAE_ASSERTM(NULL == pMat->GetMapNormal(), L"Error! The bump map was already created!!!\n");

								// TODO: Implement the addnormals method here.
								//parser.GetNextDelimiter();	// Skips until the parameters start.
								parser.GetNextToken(); // Gets the normalmap file.
								if(strToken == TAG_MTR_ADD_NORMALS)
								{
									// TODO: call the addnormals method here.

									// At the moment we only load the _local.tga file here instead of computing
									// a normal map starting starting from an height map.
									parser.GetNextToken();

									Texture* pTexture = new Texture;
									std::wstring wsFilename = strToken;
									AddTextureExtension(wsFilename);
									GAE_ASSERTM(FileExist(wsFilename), L"File doesn't exist!! PANIC!!!");
									pTexture->SetFilename(wsFilename);
									pMat->SetMapNormal(pTexture);

									parser.GetNextToken();

									if(strToken == TAG_MTR_HEIGHTMAP)
									{
										// TODO: call the heightmap method here. <- Creates a normalmap starting from an heightmap.

										// At the moment we only load the _local.tga file here instead of computing
										// a normal map starting starting from an height map.

										parser.GetNextToken();

										Texture* pTexture = new Texture;
										std::wstring wsFilename = strToken;
										AddTextureExtension(wsFilename);
										GAE_ASSERTM(FileExist(wsFilename), L"File doesn't exist!! PANIC!!!");
										pTexture->SetFilename(wsFilename);
										pMat->SetMapHeight(pTexture);
									}
								}
								else if(strToken == TAG_MTR_HEIGHTMAP)
								{
									// TODO: call the heightmap method here. <- Creates a normalmap starting from an heightmap.

									// At the moment we only load the _local.tga file here instead of computing
									// a normal map starting starting from an height map.

									parser.GetNextToken();

									Texture* pTexture = new Texture;
									std::wstring wsFilename = strToken;
									AddTextureExtension(wsFilename);
									GAE_ASSERTM(FileExist(wsFilename), L"File doesn't exist!! PANIC!!!");
									pTexture->SetFilename(wsFilename);
									pMat->SetMapHeight(pTexture);
								}
								else
								{
									Texture* pTexture = new Texture;
									std::wstring wsFilename = strToken;
									AddTextureExtension(wsFilename);
									GAE_ASSERTM(FileExist(wsFilename), L"File doesn't exist!! PANIC!!!");
									pTexture->SetFilename(wsFilename);
									pMat->SetMapNormal(pTexture);
								}

								parser.GetNextDelimiter(TAG_NEW_LINE);	// Leave the read pointer ready for the next token.
							}
						}
						/*
						if(!pMat->GetMapDiffuse() && !strQerImageEditor.empty())
						{
							Texture* pTexture = new Texture;
							std::wstring wsFilename = strQerImageEditor;
							AddTextureExtension(wsFilename);
							GAE_ASSERTM(FileExist(wsFilename), L"File doesn't exist!! PANIC!!!");
							pTexture->SetFilename(wsFilename);
							pMat->SetMapDiffuse(pTexture);
						}
						*/
					}
				}
			}

			Graphics::Material::E_BLEND_TYPE MapLoader::GetBlendType(const std::wstring& wsBlendType) const
			{
				using namespace Graphics;
				if(wsBlendType == TAG_MTR_BT_GL_ONE)
				{
					return Material::BT_ONE;
				}
				else if(wsBlendType == TAG_MTR_BT_GL_ZERO)
				{
					return Material::BT_ZERO;
				} 
				else if(wsBlendType == TAG_MTR_BT_GL_DST_COLOR)
				{
					return Material::BT_DST_COLOR;
				} 
				else if(wsBlendType == TAG_MTR_BT_GL_ONE_MINUS_DST_COLOR)
				{
					return Material::BT_ONE_MINUS_DST_COLOR;
				} 
				else if(wsBlendType == TAG_MTR_BT_GL_SRC_ALPHA)
				{
					return Material::BT_SRC_ALPHA;
				} 
				else if(wsBlendType == TAG_MTR_BT_GL_ONE_MINUS_SRC_ALPHA)
				{
					return Material::BT_ONE_MINUS_SRC_ALPHA;
				} 
				else if(wsBlendType == TAG_MTR_BT_GL_DST_ALPHA)
				{
					return Material::BT_DST_ALPHA;
				} 
				else if(wsBlendType == TAG_MTR_BT_GL_ONE_MINUS_DST_ALPHA)
				{
					return Material::BT_ONE_MINUS_DST_ALPHA;
				} 
				else if(wsBlendType == TAG_MTR_BT_GL_SRC_ALPHA_SATURATE)
				{
					return Material::BT_SRC_ALPHA_SATURATE;
				} 
				else if(wsBlendType == TAG_MTR_BT_GL_SRC_COLOR)
				{
					return Material::BT_SRC_COLOR;
				} 
				else if(wsBlendType == TAG_MTR_BT_GL_ONE_MINUS_SRC_COLOR)
				{
					return Material::BT_ONE_MINUS_SRC_COLOR;
				}
				GAE_ASSERTM(false, L"Invalid blend type!!!!");
			}

			/// \brief Add the texture extension.
			///			It adds the texture extension for the given filename. It first looks for the file. If it doesn't
			///		exists looks if the file has an extension and changes it to .dds or .tga. If the file can not be still
			///		found 
			bool MapLoader::AddTextureExtension(std::wstring& strFilename, const E_TEXTURE_EXTENSION eTE/* = TE_NONE*/)
			{
				std::wstring wsSource = strFilename;
				if(strFilename.find(L'.', 0) == std::wstring::npos)
				{
					wsSource.append(eTE == TE_DIFFUSE ? L"_d" : eTE == TE_SPECULAR ? L"_s" : eTE == TE_BUMP ? L"_local" : L"").append(L".tga");
					//wsSource.append(L".tga");
					if(!FileExist(wsSource))
					{
						GAE_WARNINGM(false, L"File doesn't exist: [%s]\nTrying next...\n", wsSource.c_str());
						wsSource = strFilename;
						//wsSource.append(L".dds");
						wsSource.append(eTE == TE_DIFFUSE ? L"_d" : eTE == TE_SPECULAR ? L"_s" : eTE == TE_BUMP ? L"_local" : L"").append(L".dds");
						if(!FileExist(wsSource))
						{
							GAE_WARNINGM(false, L"File doesn't exist: [%s]", wsSource.c_str());
							return false;
						}
					}

					strFilename = wsSource;
				}

				return true;
			}

			void MapLoader::GenerateTBN()
			{
				// Tangent and Binormal equation:
				//
				//		(v3 - v1) * (p2 - p1) - (v2 - v1) * (p3 - p1)
				//	T = ---------------------------------------------
				//		(u2 - u1) * (v3 - v1) - (v2 - v1) * (u3 - u1)
				//
				// 		(u3 - u1) * (p2 - p1) - (u2 - u1) * (p3 - p1)
				// 	B = ---------------------------------------------
				// 		(v2 - v1) * (u3 - u1) - (u2 - u1) * (v3 - v1)
				//

				using namespace Gae::Graphics;
				for each(MultiMesh* pMultiMesh in m_VecModel)
				{
					//GAE_TRACE(L"TBN MultiMesh: [%s]\n", pMultiMesh->m_wsTAG.c_str());
					GAE_TRACE(L"TBN MultiMesh: [%s]\n", pMultiMesh->GetTAG().c_str());
					for each(Mesh* pMesh in pMultiMesh->GetMeshes())
					{
						//GAE_TRACE(L"\tSurface: [%s]\n", pMesh->m_wsTAG.c_str());
						GAE_TRACE(L"\tSurface: [%s]\n", pMesh->GetTAG().c_str());

						Geometry::VertexColNormUVTB* pVtx = (Geometry::VertexColNormUVTB*) pMesh->GetVertexBuffer()->GetData();
						UINT32* pIdx = (UINT32*) (pMesh->GetIndexBuffer()->GetData());
						UINT32 uIdxCount = (pMesh->GetIndexBuffer()->GetCount());
						 
						// Iterate through every triangle.
						for(UINT32 u = 0; u < uIdxCount; u += 3)
						{
							UINT32 uIdx1 = pIdx[u + 0];
							UINT32 uIdx2 = pIdx[u + 1];
							UINT32 uIdx3 = pIdx[u + 2];
							Geometry::VertexColNormUVTB* pVtx1 = pVtx + uIdx1;
							Geometry::VertexColNormUVTB* pVtx2 = pVtx + uIdx2;
							Geometry::VertexColNormUVTB* pVtx3 = pVtx + uIdx3;

							const Vector3f& v3fPos1 = pVtx1->m_v3fPosition;
							const Vector3f& v3fPos2 = pVtx2->m_v3fPosition;
							const Vector3f& v3fPos3 = pVtx3->m_v3fPosition;

							Vector3f v3fPos3MinusPos1 = v3fPos3 - v3fPos1;
							Vector3f v3fPos2MinusPos1 = v3fPos2 - v3fPos1;

							float fTexV3MinusTexV1 = pVtx3->m_v2fUV(1) - pVtx1->m_v2fUV(1);
							float fTexV2MinusTexV1 = pVtx2->m_v2fUV(1) - pVtx1->m_v2fUV(1);
							
							float fTexU3MinusTexU1 = pVtx3->m_v2fUV(0) - pVtx1->m_v2fUV(0);
							float fTexU2MinusTexU1 = pVtx2->m_v2fUV(0) - pVtx1->m_v2fUV(0);

							Vector3f v3fTangent = 
								((fTexV3MinusTexV1 * v3fPos2MinusPos1) - (fTexV2MinusTexV1 * v3fPos3MinusPos1))
								/	// -------------------------------------------------------------------------
								((fTexU2MinusTexU1 * fTexV3MinusTexV1) - (fTexV2MinusTexV1 * fTexU3MinusTexU1));
							
							Vector3f v3fBinormal =
								((fTexU3MinusTexU1 * v3fPos2MinusPos1) - (fTexU2MinusTexU1 * v3fPos3MinusPos1))
								/	// -------------------------------------------------------------------------
								((fTexV2MinusTexV1 * fTexU3MinusTexU1) - (fTexU2MinusTexU1 * fTexV3MinusTexV1));

							// Obtain the Gram-Schmidt orthonormal vector's set.
							// First calculate the tangent taking as reference the normal.
							v3fTangent = v3fTangent - (((pVtx1->m_v3fNormal.dot(v3fTangent)) / pVtx1->m_v3fNormal.squaredNorm()) * pVtx1->m_v3fNormal);

							// Invert the binormal if the coordinate system's hand is different.
							bool bRightHanded = false;
							if(v3fTangent.cross(v3fBinormal).dot(pVtx1->m_v3fNormal) >= 0.0f)
							{
								bRightHanded = true;
							}

							// And now calculate the binormal using the cross product.
							v3fBinormal = v3fTangent.cross(pVtx1->m_v3fNormal);

							if(bRightHanded)
							{
								v3fBinormal *= -1.0f;
							}

							// Assign the result to the three vertexes.
							pVtx1->m_v3fTangent = v3fTangent.normalized();
							pVtx1->m_v3fBinormal = v3fBinormal.normalized();

							pVtx2->m_v3fTangent = v3fTangent.normalized();
							pVtx2->m_v3fBinormal = v3fBinormal.normalized();

							pVtx3->m_v3fTangent = v3fTangent.normalized();
							pVtx3->m_v3fBinormal = v3fBinormal.normalized();
						}
					}
				}
			}

			void MapLoader::LoadLevel(const std::wstring& wsFilename)
			{
				using namespace Gae::Core::Util;

				std::wstring wsFilenameProc = wsFilename + L".proc";
				std::wstring wsFilenameMap = wsFilename + L".map";

				std::wstring wsGuideText;
				std::wstring wsMaterialText;
				std::wstring wsLevelText;

				Parser ParserMatGuide;
				Parser ParserMatFluids;
				Parser ParserMatEffects;
				Parser ParserProc;
				Parser ParserMap;

				ParserMatGuide.UseFile(FILE_GUIDE_MATERIAL, TRUE);
				ParserMatFluids.UseFile(FILE_GUIDE_FLUIDS, TRUE);
				ParserMatEffects.UseFile(FILE_GUIDE_EFFECTS, TRUE);
				ParserProc.UseFile(wsFilenameProc, FALSE, FALSE);
				ParserMap.UseFile(wsFilenameMap, TRUE);

				// Stores the material's template definitions.
				ParseGuide(ParserMatGuide);
				ParseGuide(ParserMatFluids);
				ParseGuide(ParserMatEffects);

				ParseMap(ParserMap);	// Extract map's items info. Lights goes here.
				ParseProc(ParserProc);	// Extract the geometry info. Areas and shadow volumes.

				GenerateTBN();	// Calculate the tangent and binormal for the vertexes.

				// I do not know how to obtain the files. I can not find a common pattern
				// to extract the .mtr file starting from the material's name.
				// \TODO Use filesystem to populate .mtr files through the material folder instead of having this big hardcoded list.

				VecMaterialFilename vecMtrFiles;
				VecMaterialFilename vecMtrFilesToParse;
				vecMtrFiles.push_back(L"materials/banners.mtr");
				vecMtrFiles.push_back(L"materials/canyon.mtr");
				vecMtrFiles.push_back(L"materials/characters.mtr");
				vecMtrFiles.push_back(L"materials/command_ship.mtr");
				vecMtrFiles.push_back(L"materials/consoles.mtr");
				vecMtrFiles.push_back(L"materials/core.mtr");
				vecMtrFiles.push_back(L"materials/decals.mtr");
				vecMtrFiles.push_back(L"materials/decals_ofs.mtr");
				vecMtrFiles.push_back(L"materials/flares.mtr");
				vecMtrFiles.push_back(L"materials/fluids.mtr");
				vecMtrFiles.push_back(L"materials/gfx.mtr");
				vecMtrFiles.push_back(L"materials/glass.mtr");
				vecMtrFiles.push_back(L"materials/gui.mtr");
				vecMtrFiles.push_back(L"materials/hardwareshaders.mtr");
				vecMtrFiles.push_back(L"materials/holo_guis.mtr");
				vecMtrFiles.push_back(L"materials/hub.mtr");
				vecMtrFiles.push_back(L"materials/hud.mtr");
				vecMtrFiles.push_back(L"materials/idtemp.mtr");
				vecMtrFiles.push_back(L"materials/invisible.mtr");
				vecMtrFiles.push_back(L"materials/items.mtr");
				vecMtrFiles.push_back(L"materials/lights.mtr");
				vecMtrFiles.push_back(L"materials/mapobjects_debris.mtr");
				vecMtrFiles.push_back(L"materials/mapobjects_marine.mtr");
				vecMtrFiles.push_back(L"materials/mapobjects_mp.mtr");
				vecMtrFiles.push_back(L"materials/marine_flag_long.mtr");
				vecMtrFiles.push_back(L"materials/marine_guis.mtr");
				vecMtrFiles.push_back(L"materials/medlab.mtr");
				vecMtrFiles.push_back(L"materials/modelshader_guis.mtr");
				vecMtrFiles.push_back(L"materials/monsters.mtr");
				vecMtrFiles.push_back(L"materials/mptextures.mtr");
				vecMtrFiles.push_back(L"materials/multiplayer.mtr");
				vecMtrFiles.push_back(L"materials/rock.mtr");
				vecMtrFiles.push_back(L"materials/shaderdemo.mtr");
				vecMtrFiles.push_back(L"materials/skies.mtr");
				//vecMtrFiles.push_back(L"materials/storm4dm3.mtr");
				vecMtrFiles.push_back(L"materials/strogg_flag_long.mtr");
				vecMtrFiles.push_back(L"materials/strogg_guis.mtr");
				vecMtrFiles.push_back(L"materials/stroyent.mtr");
				vecMtrFiles.push_back(L"materials/tables.mtr");
				vecMtrFiles.push_back(L"materials/temp_guis.mtr");
				vecMtrFiles.push_back(L"materials/terminal.mtr");
				vecMtrFiles.push_back(L"materials/test.mtr");
				vecMtrFiles.push_back(L"materials/tram.mtr");
				vecMtrFiles.push_back(L"materials/vehicles.mtr");
				vecMtrFiles.push_back(L"materials/weapons.mtr");
				vecMtrFiles.push_back(L"materials/common/common.mtr");
				vecMtrFiles.push_back(L"materials/common/common_floors.mtr");
				vecMtrFiles.push_back(L"materials/common/common_guis.mtr");
				vecMtrFiles.push_back(L"materials/common/common_lights.mtr");
				vecMtrFiles.push_back(L"materials/common/common_misc.mtr");
				vecMtrFiles.push_back(L"materials/common/common_trims.mtr");
				vecMtrFiles.push_back(L"materials/common/common_walls.mtr");
				vecMtrFiles.push_back(L"materials/effects/electricity.mtr");
				vecMtrFiles.push_back(L"materials/effects/energy_sparks.mtr");
				vecMtrFiles.push_back(L"materials/effects/e_decals.mtr");
				vecMtrFiles.push_back(L"materials/effects/e_models.mtr");
				vecMtrFiles.push_back(L"materials/effects/e_monsters.mtr");
				vecMtrFiles.push_back(L"materials/effects/fire.mtr");
				vecMtrFiles.push_back(L"materials/effects/fluids_drips.mtr");
				vecMtrFiles.push_back(L"materials/effects/gore_spray.mtr");
				vecMtrFiles.push_back(L"materials/effects/inview.mtr");
				vecMtrFiles.push_back(L"materials/effects/particles_shapes.mtr");
				vecMtrFiles.push_back(L"materials/effects/ships.mtr");
				vecMtrFiles.push_back(L"materials/effects/smoke.mtr");
				vecMtrFiles.push_back(L"materials/effects/test.mtr");
				vecMtrFiles.push_back(L"materials/effects/weapons.mtr");
				vecMtrFiles.push_back(L"materials/mapobjects_strogg/barrels_containers.mtr");
				vecMtrFiles.push_back(L"materials/mapobjects_strogg/console.mtr");
				vecMtrFiles.push_back(L"materials/mapobjects_strogg/doors.mtr");
				vecMtrFiles.push_back(L"materials/mapobjects_strogg/levels.mtr");
				vecMtrFiles.push_back(L"materials/mapobjects_strogg/lights_s.mtr");
				vecMtrFiles.push_back(L"materials/mapobjects_strogg/machines.mtr");
				vecMtrFiles.push_back(L"materials/mapobjects_strogg/misc_equipment.mtr");
				vecMtrFiles.push_back(L"materials/mapobjects_strogg/monitors.mtr");
				vecMtrFiles.push_back(L"materials/mapobjects_strogg/outdoor.mtr");
				vecMtrFiles.push_back(L"materials/mapobjects_strogg/pipes_wires.mtr");
				vecMtrFiles.push_back(L"materials/mapobjects_strogg/stream.mtr");
				vecMtrFiles.push_back(L"materials/mapobjects_strogg/structural.mtr");
				vecMtrFiles.push_back(L"materials/mapobjects_strogg/windows.mtr");

				// Doom 3
				vecMtrFiles.push_back(L"materials/aaduffyTest.mtr");
				vecMtrFiles.push_back(L"materials/alphalabs.mtr");
				vecMtrFiles.push_back(L"materials/andy.mtr");
				vecMtrFiles.push_back(L"materials/atest.mtr");
				vecMtrFiles.push_back(L"materials/base_door.mtr");
				vecMtrFiles.push_back(L"materials/base_floor.mtr");
				vecMtrFiles.push_back(L"materials/base_light.mtr");
				vecMtrFiles.push_back(L"materials/base_trim.mtr");
				vecMtrFiles.push_back(L"materials/base_wall.mtr");
				vecMtrFiles.push_back(L"materials/caves.mtr");
				vecMtrFiles.push_back(L"materials/characters.mtr");
				vecMtrFiles.push_back(L"materials/char_common.mtr");
				vecMtrFiles.push_back(L"materials/decals.mtr");
				vecMtrFiles.push_back(L"materials/door.mtr");
				vecMtrFiles.push_back(L"materials/duffyTest.mtr");
				vecMtrFiles.push_back(L"materials/fogs.mtr");
				vecMtrFiles.push_back(L"materials/gfx.mtr");
				vecMtrFiles.push_back(L"materials/glass.mtr");
				vecMtrFiles.push_back(L"materials/hell.mtr");
				vecMtrFiles.push_back(L"materials/invisible.mtr");
				vecMtrFiles.push_back(L"materials/jerry.mtr");
				vecMtrFiles.push_back(L"materials/kentest.mtr");
				vecMtrFiles.push_back(L"materials/ktest.mtr");
				vecMtrFiles.push_back(L"materials/lab.mtr");
				vecMtrFiles.push_back(L"materials/lab_floor.mtr");
				vecMtrFiles.push_back(L"materials/lights.mtr");
				vecMtrFiles.push_back(L"materials/mal.mtr");
				vecMtrFiles.push_back(L"materials/mapobjects.mtr");
				vecMtrFiles.push_back(L"materials/Maxtextures.mtr");
				vecMtrFiles.push_back(L"materials/monsters.mtr");
				vecMtrFiles.push_back(L"materials/morgue.mtr");
				vecMtrFiles.push_back(L"materials/mp_gui.mtr");
				vecMtrFiles.push_back(L"materials/mp_lights.mtr");
				vecMtrFiles.push_back(L"materials/mre.mtr");
				vecMtrFiles.push_back(L"materials/object.mtr");
				vecMtrFiles.push_back(L"materials/outside.mtr");
				vecMtrFiles.push_back(L"materials/patd.mtr");
				vecMtrFiles.push_back(L"materials/patricktest.mtr");
				vecMtrFiles.push_back(L"materials/pdtest.mtr");
				vecMtrFiles.push_back(L"materials/phook.mtr");
				vecMtrFiles.push_back(L"materials/recyc_door.mtr");
				vecMtrFiles.push_back(L"materials/recyc_floor.mtr");
				vecMtrFiles.push_back(L"materials/recyc_light.mtr");
				vecMtrFiles.push_back(L"materials/recyc_trim.mtr");
				vecMtrFiles.push_back(L"materials/recyc_wall.mtr");
				vecMtrFiles.push_back(L"materials/rock.mtr");
				vecMtrFiles.push_back(L"materials/senetemp.mtr");
				vecMtrFiles.push_back(L"materials/sfx.mtr");
				vecMtrFiles.push_back(L"materials/sfx_multiplayer.mtr");
				vecMtrFiles.push_back(L"materials/shaderDemo.mtr");
				vecMtrFiles.push_back(L"materials/skies.mtr");
				vecMtrFiles.push_back(L"materials/sound.mtr");
				vecMtrFiles.push_back(L"materials/specialcase.mtr");
				vecMtrFiles.push_back(L"materials/stevetest.mtr");
				vecMtrFiles.push_back(L"materials/stone.mtr");
				vecMtrFiles.push_back(L"materials/testInteractions.mtr");
				vecMtrFiles.push_back(L"materials/tim.mtr");
				vecMtrFiles.push_back(L"materials/tools.mtr");
				vecMtrFiles.push_back(L"materials/vehicles.mtr");
				vecMtrFiles.push_back(L"materials/video.mtr");
				vecMtrFiles.push_back(L"materials/vp_materials.mtr");
				vecMtrFiles.push_back(L"materials/washroom.mtr");
				vecMtrFiles.push_back(L"materials/weapons.mtr");
				vecMtrFiles.push_back(L"materials/zgraeme.mtr");

				for each(const std::wstring& wsMtrFile in vecMtrFiles)
				{	// Avoid duplicated entries.
					std::wstring wsMtrFilename = wsMtrFile;
					transform(wsMtrFilename.begin(), wsMtrFilename.end(), wsMtrFilename.begin(), towlower);
					if(std::find(vecMtrFilesToParse.begin(), vecMtrFilesToParse.end(), wsMtrFilename) != vecMtrFilesToParse.end() || !FileExist(wsMtrFilename))	continue;
					vecMtrFilesToParse.push_back(wsMtrFilename);
				}
				vecMtrFiles.clear();

				for each(const std::wstring& wsMtrFile in vecMtrFilesToParse)
				{	// Parse each .mtr file. It stores an individual entry for each material's
					// description found. If a material is using a guide template (material
					// template definition) it will expand the template with the given parameters
					// and also store it like a common material.
					ParseMtr(wsMtrFile);
				}

				// Analyzes the material description and configures the Material objects with it.
				ProcessMaterials();
				
#ifdef _DEBUG
				// Checks that every level model has a material.
				CheckModelsMaterial();
#endif

				// Finally add the meshes to the universe manager.
				Gae::Core::Universe::Manager& universeManager = Gae::Core::Universe::Manager::Singleton();
				// Add the models to the universe manager in order to display the level.
				for(MapLoader::VecModel::const_iterator it = m_VecModel.begin(), itEnd = m_VecModel.end(); it != itEnd; ++it)
				{
					// Only add those meshes that has the area specified in the name.
					Gae::Graphics::MultiMesh* pMultiMesh = *it;

					//size_t stFoundPos = pMultiMesh->m_wsTAG.find(L"_area");
					size_t stFoundPos = pMultiMesh->GetTAG().find(L"_area");
					if(stFoundPos != std::wstring::npos)
					{
						//int iArea = _wtoi(pMultiMesh->m_wsTAG.substr(5).c_str());
						int iArea = _wtoi(pMultiMesh->GetTAG().substr(5).c_str());
						universeManager.AddStaticMultiMesh(*it, iArea);
					}
					else
					{	// Test if we have a func_static_/_door/_xxxxxxxxxxx model. Usually a light mesh (a bulb, a lamp, a led, etc...), a door, etc...
						// Extracts the origin and rotation info from the std::map.
						//MapFuncStaticOrigin::const_iterator mapOriginIt = m_MapFuncOrigin.find(Gae::Core::Util::CRC32(pMultiMesh->m_wsTAG));
						MapFuncStaticOrigin::const_iterator mapOriginIt = m_MapFuncOrigin.find(pMultiMesh->GetTagCRC32());
						GAE_ASSERTM(mapOriginIt != m_MapFuncOrigin.end(), L"The model doesn't have any correspondence in the .map file. Check this!\n\tModel: [%s]", pMultiMesh->GetTAG().c_str());
						if(mapOriginIt != m_MapFuncOrigin.end())
						{
							const Vector3f& v3fAux = mapOriginIt->second;
							const Gae::Core::Universe::IPortalAreaDetermination* pAreaDetermination = universeManager.GetPortalAreaDetermination();
							int iArea = pAreaDetermination ? pAreaDetermination->GetAreaPortal(v3fAux) : 0;
							GAE_ASSERTM(iArea != -1, L"The model is not lying inside any area!!!");

							for each(Gae::Graphics::Mesh* pMesh in pMultiMesh->GetMeshes())
							{
								using namespace Gae::Graphics;
								Geometry::VertexColNormUVTB* pVtx = (Geometry::VertexColNormUVTB*) pMesh->GetVertexBuffer()->GetData();
								UINT32 uVtxCount = (pMesh->GetVertexBuffer()->GetCount());

								// TODO: Review this point. Actually only the position is being taken in account. Maybe also a rotation is used.
								// Apply the direct transformation to avoid setting world transformation matrix each time.
								for(UINT32 u = 0; u < uVtxCount; ++u, ++pVtx)
								{	//Iterate through every vertex.
									pVtx->m_v3fPosition += v3fAux;
								}
								pMesh->ComputeAABB();
							}

							if(iArea != -1)
							{	// Add the mesh directly to an area and initialize it.
								universeManager.AddStaticMultiMesh(pMultiMesh, iArea);
							}
							else
							{	// Look for a valid area based in the vertex position of the mesh and initialize it.
								universeManager.AddStaticMultiMesh(pMultiMesh);
							}
						}
						else
						{
							// Try adding the mesh directly. Inside AddStaticMesh method a per-vertex are determination is done.
							for each(Gae::Graphics::Mesh* pMesh in pMultiMesh->GetMeshes())
							{
								pMesh->ComputeAABB();
								universeManager.AddStaticMesh(pMesh);
							}
						} 
					}
				}

				ConfigureStaticLights();

				// Not needed anymore. The shadow volumes are associated inside the light objects. Still here in order to initialize correctly the shadow volume meshes.
				for(MapLoader::VecShadowVolume::const_iterator it = m_VecShadowVolume.begin(), itEnd = m_VecShadowVolume.end(); it != itEnd; ++it)
				{
					universeManager.AddShadowModel(*it);
				}

				for(MapLoader::VecLight::const_iterator it = m_VecLight.begin(), itEnd = m_VecLight.end(); it != itEnd; ++it)
				{
					universeManager.AddLightIntoCorrectArea(*it);
					universeManager.AddLight(*it);
				}

				// Set the starting point of the main camera.
				/*
				for(int i = 0; i < 2; ++i)
				{
					Gae::Entities::Camera* pCam = universeManager.GetCamera(universeManager.CreateCamera());
					pCam->Init();
					pCam->SetPosition(GetSpawnPosition());
				}
				*/
				const MapLoader::VecSpawnPoints& vecSpawnPoints = GetSpawnPositions();
				for(size_t i = 0, uCount = vecSpawnPoints.size(); i < uCount; ++i)
				{
					Gae::Entities::Camera* pCam = universeManager.GetCamera(universeManager.CreateCamera());
					pCam->Init();
					pCam->SetPosition(vecSpawnPoints.at(i));
				}
			}

#ifdef _DEBUG
			void MapLoader::CheckModelsMaterial()
			{
				for each(Gae::Graphics::MultiMesh* pMultiMesh in m_VecModel)
				{
					size_t stMatCount = pMultiMesh->GetMeshes().size();
					for each(Gae::Graphics::Mesh* pMesh in pMultiMesh->GetMeshes())
					{
						GAE_ASSERTM(NULL != pMesh->GetMaterial(), L"The mesh has no material: [%s]\n", pMesh->GetTAG().c_str());

						Gae::Graphics::Material* pMat = pMesh->GetMaterial();

						if(NULL == pMat->GetMapDiffuse())
						{
							//GAE_TRACE(L"Material has no diffuse map: [%s]\n", pMat->m_wsTAG.c_str());
							--stMatCount;
						}
					}
					if(0 == stMatCount)
					{
						GAE_TRACE(L"Multimesh has not any diffuse map: [%s]\n", pMultiMesh->GetTAG().c_str());
					}
				}
			}
#endif

			void MapLoader::ParseProc(Gae::Core::Util::Parser& ParserProc)
			{
				using namespace Gae::Core::Util;
				using namespace Gae::Graphics;
				using namespace Gae::Graphics::Geometry;
				using namespace Gae::Core::Resources;

				Gae::Core::Universe::Manager& universeManager = Gae::Core::Universe::Manager::Singleton();

				ParserProc.SetDelimitersToken(L"=*(){},\"\n");

				{	// Read portals, creates the portal and store it in m_VecPortal.
					ParserProc.Reset();

					const std::wstring& wsToken = ParserProc.GetNextToken(TAG_PORTALS);
					GAE_ASSERTM(Parser::TAG_END != wsToken, L"Error processing .proc file!");

					if(Parser::TAG_END == wsToken)
					{
						universeManager.CreatePortalAreas(1);
					}
					else // if (Parser::TAG_END != wsToken)
					{

						UINT32 uAreas = (UINT32) _wtoi(ParserProc.GetNextNumber().c_str());

						universeManager.CreatePortalAreas(uAreas);

						UINT32 uPortalCount = (UINT32) _wtoi(ParserProc.GetNextNumber().c_str());
						m_VecPortal.reserve(uPortalCount);
						for(UINT32 uPortal = 0; uPortal < uPortalCount; ++uPortal)
						{
							ParserProc.GetNextToken(TAG_PORTALS_PORTAL);
							GAE_ASSERTM(_wtoi(ParserProc.GetNextNumber().c_str()) == uPortal, L"Error while parsing portals. One of the portals was skipped while parsing!");
							ParserProc.GetNextDelimiter();

							int iVertexCount = _wtoi(ParserProc.GetNextNumber().c_str());
							int iIndexCount = 12;	// At the moment we will only draw a quad.

							// Lets create the mesh for the portal
							Mesh* pMesh = new Mesh;
							VertexBuffer* pVB = new VertexBuffer(VT_COLOR, iVertexCount); // Only color and position.
							IndexBuffer* pIB = new IndexBuffer(iIndexCount);
							VertexBuffer::VecVertex vecVertex;
							IndexBuffer::VecIndex vecIndex;
							vecIndex.reserve(iIndexCount);
							vecVertex.reserve(iVertexCount);

							int iPosArea = _wtoi(ParserProc.GetNextNumber().c_str());
							int iNegArea = _wtoi(ParserProc.GetNextNumber().c_str());

							DWORD dwColor[4] = {
								D3DCOLOR_ARGB(255, 255, 0, 0),
								D3DCOLOR_ARGB(255, 0, 255, 0),
								D3DCOLOR_ARGB(255, 0, 0, 255),
								D3DCOLOR_ARGB(255, 255, 0, 255)
							};

							// Read the vertexes
							for(int i = 0; i < iVertexCount; ++i)
							{
								Vertex* pVertex = NULL;
								// Advance until the next vertex.
								ParserProc.GetNextDelimiter(TAG_PARAM_START);

								float vertexX = (float) _wtof(ParserProc.GetNextNumber().c_str());
								float vertexY = (float) _wtof(ParserProc.GetNextNumber().c_str());
								float vertexZ = (float) _wtof(ParserProc.GetNextNumber().c_str());

								VertexCol* pVtx = new VertexCol;
								pVtx->m_v3fPosition << vertexX, vertexZ, vertexY;
								pVtx->m_dwColor =  dwColor[i % 4];
								pVertex = pVtx;

								vecVertex.push_back(pVertex);
							}

							//(const_cast<Gae::Graphics::Geometry::Plane&>(pPortal->GetPlane())).SetFromPoints(vecVertex.at(0)->m_v3fPosition, vecVertex.at(1)->m_v3fPosition, vecVertex.at(2)->m_v3fPosition);

							// Adds the indexes - two sided
							vecIndex.push_back(0);
							vecIndex.push_back(1);
							vecIndex.push_back(2);
							vecIndex.push_back(0);
							vecIndex.push_back(2);
							vecIndex.push_back(3);

							vecIndex.push_back(2);
							vecIndex.push_back(1);
							vecIndex.push_back(0);
							vecIndex.push_back(3);
							vecIndex.push_back(2);
							vecIndex.push_back(0);

							// Sets the data into the mesh for later displaying.
							pVB->SetData(vecVertex);
							pIB->SetData(vecIndex);

							pMesh->SetVertexBuffer(pVB);
							pMesh->SetIndexBuffer(pIB);

							pMesh->ComputeAABB();

							Gae::Core::Universe::Portal* pPortal = new Gae::Core::Universe::Portal;
							pPortal->SetMesh(pMesh);
							pPortal->SetPositiveSideAreaIndex(iPosArea);
							pPortal->SetNegativeSideAreaIndex(iNegArea);
							m_VecPortal.push_back(pPortal);

						}

						for(UINT32 i = 0; i < m_VecPortal.size(); ++i)
						{
							universeManager.AddPortal(m_VecPortal.at(i));
						}
					}
				}

				{	// Read the BSP nodes.
					//ParserProc.Reset();	// We are at the end of the file. Reset the read pointer.

					GAE_ASSERTM(NULL == m_pBSP, L"BspTree has been already created!");

					const std::wstring& wsToken = ParserProc.GetNextToken(TAG_NODES);
					GAE_ASSERTM(Parser::TAG_END != wsToken, L"Error processing .proc file!");

					if(Parser::TAG_END != wsToken)
					{
						ParserProc.GetNextToken(TAG_NODES_COUNT);
						GAE_ASSERTM(Parser::TAG_END != wsToken, L"Error processing .proc file!");
						UINT32 uCount = _wtoi(ParserProc.GetNextNumber().c_str());

						m_pBSP = new BSPTree(uCount);

						ParserProc.GetNextDelimiter(TAG_PARAM_END);
						ParserProc.GetNextDelimiter(TAG_PARAM_END);
						for(UINT32 i = 0; i < uCount; ++i)
						{
							ParserProc.GetNextDelimiter(TAG_PARAM_START);
							float fA = (float)_wtof(ParserProc.GetNextNumber().c_str());
							float fB = (float)_wtof(ParserProc.GetNextNumber().c_str());
							float fC = (float)_wtof(ParserProc.GetNextNumber().c_str());
							float fD = (float)_wtof(ParserProc.GetNextNumber().c_str());

							m_pBSP->m_pRoot[i].m_Plane.m_v3fNormal << fA, fC, fB;
							m_pBSP->m_pRoot[i].m_Plane.m_fDistance = fD;

							m_pBSP->m_pRoot[i].m_iChildPos = _wtoi(ParserProc.GetNextNumber().c_str());
							m_pBSP->m_pRoot[i].m_iChildNeg = _wtoi(ParserProc.GetNextNumber().c_str());
						}

						universeManager.SetPortalAreaDetermination(m_pBSP);
					}
				}

				while(true)
				{	// Read ShadowVolumes.
					//ParserProc.Reset();

					const std::wstring& wsToken = ParserProc.GetNextToken(TAG_SHADOWVOL);
					if(Parser::TAG_END == wsToken) break;	// No more shadow volumes.

					ShadowVolume* pShadowVol = new ShadowVolume;

					/* name = */ /* numVerts = */ /* noCaps = */ /* noFrontCaps = */ /* numIndexes = */ /* planeBits = */
					std::wstring strModelName = ParserProc.GetNextString();
					GAE_TRACE(L"ShadowVolume: [%s]\n", strModelName.c_str());

					int iVertexCount = _wtoi(ParserProc.GetNextNumber().c_str());
					int iCountCaps = _wtoi(ParserProc.GetNextNumber().c_str());
					int iCountCapsFront = _wtoi(ParserProc.GetNextNumber().c_str());
					int iIndexCount = _wtoi(ParserProc.GetNextNumber().c_str());
					int iPlaneBits = _wtoi(ParserProc.GetNextNumber().c_str());

					Mesh* pMesh = new Mesh;
					VertexBuffer* pVB = new VertexBuffer(/*VT_COLOR*/0, iVertexCount);
					IndexBuffer* pIB = new IndexBuffer(iIndexCount);
					VertexBuffer::VecVertex vecVertex;
					IndexBuffer::VecIndex vecIndex;
					vecIndex.reserve(iIndexCount);
					vecVertex.reserve(iVertexCount);

					// Process the vertexes
					for(int i = 0; i < iVertexCount; ++i)
					{	
						ParserProc.GetNextDelimiter(TAG_PARAM_START);	// Advance until the next vertex.

						float vertexX = (float) _wtof(ParserProc.GetNextNumber().c_str());
						float vertexY = (float) _wtof(ParserProc.GetNextNumber().c_str());
						float vertexZ = (float) _wtof(ParserProc.GetNextNumber().c_str());

						//VertexCol* pVtx = new VertexCol;	// Colored vertex.
						Vertex* pVtx = new Vertex;	// Colored vertex.
						pVtx->m_v3fPosition << vertexX, vertexZ, vertexY;
						//pVtx->m_dwColor = D3DCOLOR_ARGB(255, 255, 255, 255); // black color for shadow volumes

						Vertex* pVertex = pVtx;
						vecVertex.push_back(pVertex);

						// Advance until the end of the vertex data.
						ParserProc.GetNextDelimiter(TAG_PARAM_END);
					}

					// Process the indexes
					for(int i = 0; i < iIndexCount; ++i)
					{
						vecIndex.push_back(_wtoi(ParserProc.GetNextNumber().c_str()));
					}

					pVB->SetData(vecVertex);
					pIB->SetData(vecIndex);

					//pMesh->m_wsTAG = strModelName;
					pMesh->SetTAG(strModelName);
					pMesh->SetVertexBuffer(pVB);
					pMesh->SetIndexBuffer(pIB);
					pMesh->ComputeAABB();

					pShadowVol->SetCountCaps(iCountCaps);
					pShadowVol->SetCountCapsFront(iCountCapsFront);
					pShadowVol->SetPlaneBits(iPlaneBits);
					pShadowVol->SetMesh(pMesh);

					m_VecShadowVolume.push_back(pShadowVol);

					//Destroy the intermediate vertex and index vector.
					while(!vecVertex.empty())
					{
						VertexBuffer::VecVertex::iterator it = vecVertex.begin();
						GAE_DELETESAFE(*it);
						vecVertex.erase(it);
					}
					vecIndex.clear();

					GAE_TRACE(L"\n");
				}

				const std::wstring& wsProcText = ParserProc.GetFileText();
				{	// Populates and create the models.
					ParserProc.Reset();
					UINT32 uCountModel = 0;
					while(ParserProc.GetNextToken(TAG_MODEL) != Parser::TAG_END && ++uCountModel);
					m_VecModel.reserve(uCountModel);
					GAE_TRACE(L"Models found: [%u]\n", uCountModel);
					for(UINT32 i = 0; i < uCountModel; ++i)
					{
						m_VecModel.push_back(new Gae::Graphics::MultiMesh);
					}
				}

				{	// Read the geometry
					ParserProc.Reset();	// We are at the end of the file. Reset the read pointer.
					UINT32 uIdxModel = 0;
					while(ParserProc.GetNextToken(TAG_MODEL) != Parser::TAG_END)
					{
						using namespace Gae::Graphics;
						using namespace Gae::Graphics::Geometry;
						using namespace Gae::Core::Resources;

						MultiMesh* pModel = m_VecModel.at(uIdxModel);

						{	// Write down the name
							std::wstring strModelName = ParserProc.GetNextString();

							GAE_TRACE(L"Model found:\t[%s]\n", strModelName.c_str());

							//pModel->m_wsTAG = strModelName;
							pModel->SetTAG(strModelName);
						}

						{	// Count the surfaces.
							int iSurfaceCount = _wtoi(ParserProc.GetNextNumber().c_str());

							// If we do not have any surface ignore the model.
							if(iSurfaceCount == 0) continue;

							uIdxModel++;

							pModel->SetMeshCount(iSurfaceCount);
							for(int i = 0; i < iSurfaceCount; ++i)
							{	// store the geometry data for each surface.
								ParserProc.GetNextToken(TAG_SURFACE);
								GAE_ASSERTM(_wtoi(ParserProc.GetNextNumber().c_str()) == i, L"PANIC! Surface number doesn't correspond with the current surface being created!!");

								std::wstring strMaterial = ParserProc.GetNextString();
								int iVertexCount = _wtoi(ParserProc.GetNextNumber().c_str());
								int iIndexCount = _wtoi(ParserProc.GetNextNumber().c_str());

								Mesh* pMesh = new Mesh;
								VertexBuffer* pVB = new VertexBuffer(VT_COLOR | VT_NORMAL | VT_UV_COORD | VT_TANGENT | VT_BINORMAL, iVertexCount);
								IndexBuffer* pIB = new IndexBuffer(iIndexCount);
								VertexBuffer::VecVertex vecVertex;
								IndexBuffer::VecIndex vecIndex;
								vecIndex.reserve(iIndexCount);
								vecVertex.reserve(iVertexCount);
#ifdef _DEBUG
								m_uVertexTotal += iVertexCount;
								m_uIndexTotal += iIndexCount;
								m_uSurfacesTotal++;

								GAE_TRACE(L"\n");
								GAE_TRACE(L"\tSurface [%i]:\n", i);

								GAE_TRACE(L"\t\tMaterial:\t[%s]\n", strMaterial.c_str());
								GAE_TRACE(L"\t\tVertex Count:\t[%i]\n", iVertexCount);
								GAE_TRACE(L"\t\tIndex Count:\t[%i]\n", iIndexCount);


								//pMesh->m_wsTAG = strMaterial;
								pMesh->SetTAG(strMaterial);
#endif

								// Adds the .mtr file of this material to be further parsed.
								pMesh->SetMaterial(GetMaterial(strMaterial));

								for(int i = 0; i < iVertexCount; ++i)
								{
									Vertex* pVertex = NULL;
									// Advance until the next vertex.
									ParserProc.GetNextDelimiter(TAG_PARAM_START);

									float vertexX = (float) _wtof(ParserProc.GetNextNumber().c_str());
									float vertexY = (float) _wtof(ParserProc.GetNextNumber().c_str());
									float vertexZ = (float) _wtof(ParserProc.GetNextNumber().c_str());

									float vertexU = (float) _wtof(ParserProc.GetNextNumber().c_str());
									float vertexV = (float) _wtof(ParserProc.GetNextNumber().c_str());

									float vertexNormX = (float) _wtof(ParserProc.GetNextNumber().c_str());
									float vertexNormY = (float) _wtof(ParserProc.GetNextNumber().c_str());
									float vertexNormZ = (float) _wtof(ParserProc.GetNextNumber().c_str());

									VertexColNormUVTB* pVtx = new VertexColNormUVTB;
									pVtx->m_v3fPosition << vertexX, vertexZ, vertexY;
									pVtx->m_v2fUV << vertexU, vertexV;
									pVtx->m_v3fNormal << vertexNormX, vertexNormZ, vertexNormY;
									//pVtx->m_v3fNormal << 0.0f, 1.0f, 0.0f;	// Testing...
									pVtx->m_dwColor = D3DCOLOR_XRGB(255,255,255);
									pVertex = pVtx;

									vecVertex.push_back(pVertex);

									// Advance until the next vertex.
									ParserProc.GetNextDelimiter(TAG_PARAM_END);
								}


								for(int i = 0; i < iIndexCount; ++i)
								{
									vecIndex.push_back(_wtoi(ParserProc.GetNextNumber().c_str()));
								}

#ifdef _DEBUG
								// Test for degenerated triangles.
								for(int i = 0; i < iIndexCount; i += 3)
								{
									int iIdx1 = vecIndex[i+0];
									int iIdx2 = vecIndex[i+1];
									int iIdx3 = vecIndex[i+2];

									GAE_ASSERTM(iIdx1 != iIdx2 && iIdx1 != iIdx3 && iIdx2 != iIdx3, L"Degenerated triangle found!");

								}
#endif

								pVB->SetData(vecVertex);
								pIB->SetData(vecIndex);
								pMesh->SetVertexBuffer(pVB);
								pMesh->SetIndexBuffer(pIB);
								
								pMesh->ComputeAABB();

								pModel->AddMesh(pMesh);

								//Destroy the intermediate vertex and index vector.
								while(!vecVertex.empty())
								{
									VertexBuffer::VecVertex::iterator it = vecVertex.begin();
									GAE_DELETESAFE(*it);
									vecVertex.erase(it);
								}
								vecIndex.clear();

								GAE_TRACE(L"\n");
							}
						}
					}

					// Erase the unused level models.
					uIdxModel = (UINT32) m_VecModel.size() - uIdxModel;
					while(uIdxModel--)
					{
						GAE_DELETESAFE(*(m_VecModel.end()-1));
						m_VecModel.erase(m_VecModel.end()-1);
					}

#ifdef _DEBUG		// Some stats.
					m_uPolygonTotal = m_uIndexTotal / 3;
					GAE_TRACE(L"Surfaces: [%u]\tVertex: [%u]\tPolygons: [%u]\n", m_uSurfacesTotal, m_uVertexTotal, m_uPolygonTotal);
#endif
				}
			}

			void MapLoader::ParseMap(Gae::Core::Util::Parser& parser)
			{
				using namespace Gae::Core::Util;

				parser.SetDelimitersToken(L"=*(),\"\n");
				const std::wstring& strLastToken = parser.GetToken();

				while(parser.GetNextToken(TAG_MAP_CLASSNAME) != Parser::TAG_END)
				{
					parser.GetNextToken();
					if(strLastToken == TAG_MAP_PLAYER_SPAWN)
					{
						//GAE_TRACE(L"Found player spawn: [%f, %f, %f]\n", m_v3fSpawn(0), m_v3fSpawn(1), m_v3fSpawn(2));
						float fX = (float) _wtof(parser.GetNextNumber().c_str());
						float fY = (float) _wtof(parser.GetNextNumber().c_str());
						float fZ = (float) _wtof(parser.GetNextNumber().c_str());

						Vector3f v3fSpawn;
						v3fSpawn << fX, fZ, fY;
						m_VecSpawnPoints.push_back(v3fSpawn);
						GAE_TRACE(L"Found and added player spawn point: [%f, %f, %f]\n", v3fSpawn(0), v3fSpawn(1), v3fSpawn(2));
					}
					/*
					else if(strLastToken == TAG_FUNC_STATIC)
					{
						parser.GetNextToken(TAG_MODEL);
						UINT32 uFuncStaticModel = Gae::Core::Util::CRC32(parser.GetNextToken());

						while(parser.GetNextToken() != TAG_SENTENCE_END_STR)
						{
							if(strLastToken == TAG_MAP_ORIGIN)
							{
								Vector3f v3fAux;
								float fX = (float) _wtof(parser.GetNextNumber().c_str());
								float fY = (float) _wtof(parser.GetNextNumber().c_str());
								float fZ = (float) _wtof(parser.GetNextNumber().c_str());

								v3fAux << fX, fZ, fY;

								m_MapFuncStaticOrigin[uFuncStaticModel] = v3fAux;
								break;
							}
						}
					}
					*/
					else if(strLastToken.find(TAG_FUNC) == 0/* strLastToken == TAG_FUNC*/)	// If the class name starts with "func_"
					{
						//parser.GetNextToken(TAG_MODEL);
						//while(parser.GetNextToken() != TAG_MODEL && strLastToken != TAG_SENTENCE_END_STR);
						while(parser.GetNextToken() != TAG_MAP_NAME && strLastToken != TAG_SENTENCE_END_STR);

						if(strLastToken != TAG_SENTENCE_END_STR)
						{	// If we have a valid func_ model parse it! Get the origin and the rest of the fields.
							UINT32 uFuncStaticModel = Gae::Core::Util::CRC32(parser.GetNextToken());
							GAE_ASSERTM(strLastToken != L"func_static_16187", L"Debug!");
							while(parser.GetNextToken() != TAG_SENTENCE_END_STR)
							{	// \TODO Check also for color and the other fields...
								if(strLastToken == TAG_MAP_ORIGIN)
								{
									Vector3f v3fAux;
									float fX = (float) _wtof(parser.GetNextNumber().c_str());
									float fY = (float) _wtof(parser.GetNextNumber().c_str());
									float fZ = (float) _wtof(parser.GetNextNumber().c_str());

									v3fAux << fX, fZ, fY;

									GAE_ASSERTM(m_MapFuncOrigin.find(uFuncStaticModel) == m_MapFuncOrigin.end(), L"Model entity's entry was alread added! Impossible! Wrong .map file entry!");
									m_MapFuncOrigin[uFuncStaticModel] = v3fAux;
									break;
								}
							}
						}
					}
					else if(strLastToken == TAG_MAP_LIGHT)
					{
						/*
						"classname" "light"
						"name" "light_128"
						"origin" "288 320 -1344"
						"nodynamicshadows" "0"
						"noshadows" "0"
						"nospecular" "0"
						"nodiffuse" "0"
						"falloff" "0"
						"texture" "lights/ambientlight_nofall"
						"_color" "0.22 0.22 0.22"
						"light_radius" "3168 3072 5312"
						*/
						Gae::Entities::Light* pLight = new Gae::Entities::Light;
						UINT32 uFuncStaticModel = 0;
						while(parser.GetNextToken() != TAG_SENTENCE_END_STR)
						{
							// \TODO Check also for color and the other fields...
							if(strLastToken == TAG_MAP_NAME)
							{
								pLight->SetName(parser.GetNextToken());

								GAE_ASSERTM(m_MapFuncOrigin.find(uFuncStaticModel) == m_MapFuncOrigin.end(), L"Model entity's entry was alread added! Impossible! Wrong .map file entry!");
								uFuncStaticModel = Gae::Core::Util::CRC32(strLastToken);
							}
							/*
							else if(strLastToken == TAG_MODEL)
							{
								uFuncStaticModel = Gae::Core::Util::CRC32(parser.GetNextToken());
							}
							*/
							else if(strLastToken == TAG_MAP_ORIGIN)
							{
								Vector3f v3fAux;
								float fX = (float) _wtof(parser.GetNextNumber().c_str());
								float fY = (float) _wtof(parser.GetNextNumber().c_str());
								float fZ = (float) _wtof(parser.GetNextNumber().c_str());

								v3fAux << fX, fZ, fY;
								pLight->SetOrigin(v3fAux);
							}
							else if(strLastToken == TAG_MAP_COLOR)
							{
								Vector3f v3fAux;
								float fX = (float) _wtof(parser.GetNextNumber().c_str());
								float fY = (float) _wtof(parser.GetNextNumber().c_str());
								float fZ = (float) _wtof(parser.GetNextNumber().c_str());

								v3fAux << fX, fY, fZ; 
								pLight->SetColor(v3fAux);
							}
							else if(strLastToken == TAG_MAP_LIGHT_RADIUS)
							{
								Vector3f v3fAux;
								float fX = (float) _wtof(parser.GetNextNumber().c_str());
								float fY = (float) _wtof(parser.GetNextNumber().c_str());
								float fZ = (float) _wtof(parser.GetNextNumber().c_str());

								v3fAux << fX, fZ, fY;

								pLight->SetRadius(v3fAux);
							}
							else if(strLastToken == TAG_MAP_LIGHT_CENTER)
							{
								Vector3f v3fAux;
								float fX = (float) _wtof(parser.GetNextNumber().c_str());
								float fY = (float) _wtof(parser.GetNextNumber().c_str());
								float fZ = (float) _wtof(parser.GetNextNumber().c_str());

								v3fAux << fX, fZ, fY;
								pLight->SetCenter(v3fAux);
							}
						}

						// If we have a valid model for this light's entity add it for further processing of the final position.
						//GAE_ASSERTM(uFuncStaticModel != 3031046753, L"Debug!");
						if(uFuncStaticModel != 0)
						{
							GAE_ASSERTM(m_MapFuncOrigin.find(uFuncStaticModel) == m_MapFuncOrigin.end(), L"Model entity's entry was alread added! Impossible! Wrong .map file entry!");
							m_MapFuncOrigin[uFuncStaticModel] = pLight->GetOrigin();
						}

						/*
						if(pLight->GetRadius().norm() <= 1000.0f)
						{
						*/
							if(std::find(m_VecLight.begin(), m_VecLight.end(), pLight) == m_VecLight.end())
							{
								m_VecLight.push_back(pLight);
							}
						/*
						}
						*/
					}
					else	// It is not a light or any other entity we are interested in so try to process any other supported param.
					{
						while(parser.GetNextToken() != TAG_MAP_NAME && strLastToken != TAG_SENTENCE_END_STR);

						if(strLastToken != TAG_SENTENCE_END_STR)
						{	// If we have a valid entity... parse it! Get the fields.
							UINT32 uFuncStaticModel = Gae::Core::Util::CRC32(parser.GetNextToken());

							while(parser.GetNextToken() != TAG_SENTENCE_END_STR)
							{	// \TODO Check also for color and the other fields...
								if(strLastToken == TAG_MAP_ORIGIN)
								{
									Vector3f v3fAux;
									float fX = (float) _wtof(parser.GetNextNumber().c_str());
									float fY = (float) _wtof(parser.GetNextNumber().c_str());
									float fZ = (float) _wtof(parser.GetNextNumber().c_str());

									v3fAux << fX, fZ, fY;

									GAE_ASSERTM(m_MapFuncOrigin.find(uFuncStaticModel) == m_MapFuncOrigin.end(), L"Model entity's entry was alread added! Impossible! Wrong .map file entry!");
									m_MapFuncOrigin[uFuncStaticModel] = v3fAux;
									break;
								}
							}
						}
					}
				}
			}

			void MapLoader::ConfigureStaticLights()
			{
				using namespace Gae::Graphics;
				using namespace Gae::Graphics::Geometry;
				using namespace Gae::Entities;

				// See what shadow volume pertains to the light.
				for each(const ShadowVolume* pShadowVolume in m_VecShadowVolume)
				{
					//const std::wstring& wsShadowName = pShadowVolume->GetMesh()->m_wsTAG;
					const std::wstring& wsShadowName = pShadowVolume->GetMesh()->GetTAG();
					for each(Light* pLight in m_VecLight)
					{
						const std::wstring& wsLightName = pLight->GetName();
						if(wsShadowName.find(wsLightName, wsShadowName.size() - wsLightName.size()/* - 1*/) != std::wstring::npos)
						{
							pLight->SetShadowVolume(pShadowVolume);
							break;
						}
					}
				}

				// Populate the models a light lights and associate the light with these models.
				for each(Light* pLight in m_VecLight)
				{
					for each(const MultiMesh* pMultiMesh in m_VecModel)
					{
						const MultiMesh::VecMeshes& vecMeshes = pMultiMesh->GetMeshes();
						for each(Mesh* pMesh in vecMeshes)
						{
							if(pLight->IsMeshAffected(pMesh))
							{
								pLight->AddMesh(pMesh);
								pMesh->AddLight(pLight);
							}
						}
					}
					pLight->ComputeMeshOrder();
				}

				// Compute the AABB of the light to use it later for frustum culling.
				for each(Light* pLight in m_VecLight)
				{
					pLight->ComputeAABB();
				}
			}
		}
	}
}