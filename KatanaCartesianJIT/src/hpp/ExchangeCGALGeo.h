#pragma once
#undef interface
//#define SOL_NO_EXCEPTIONS 1
#include <FnGeolib/op/FnGeolibOp.h>
#include "BindCGAL_DefineType.h"
#undef interface
#undef GetCurrentTime
namespace Cartesian {

	void BuildSurfaceMeshFromKatana(PRE_TYPE::Mesh &meshToBuild, Foundry::Katana::GeolibCookInterface& iface);

	void SurfaceMeshToKatana(const PRE_TYPE::Mesh& mesh, Foundry::Katana::GeolibCookInterface& iface);



}