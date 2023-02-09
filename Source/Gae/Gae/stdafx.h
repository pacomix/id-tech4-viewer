// stdafx.h : Includedatei für Standardsystem-Includedateien
// oder häufig verwendete projektspezifische Includedateien,
// die nur in unregelmäßigen Abständen geändert werden.
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Selten verwendete Teile der Windows-Header nicht einbinden.
#include <Windows.h>

// STL
#include <algorithm>
#include <deque>
#include <functional>
#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
#include <locale>

// DirectX
#include <DxErr.h>
#include <d3dx9.h>
#include <dinput.h>

#include <gl/GL.h>
#include <gl/GLU.h>

// Maths
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <Eigen/LU>
// Additional Eigen typedefs
using namespace Eigen;
//USING_PART_OF_NAMESPACE_EIGEN
typedef Matrix<float, 3, 4 >	Matrix34f;

// Signal system
#include <ksignal.h>

// The Genium Application Engine
#include "Gae.h"

