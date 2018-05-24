/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "LevelSetTriMaterialDiffusion.h"

template <>
InputParameters
validParams<LevelSetTriMaterialDiffusion>()
{
  InputParameters params = validParams<LevelSetTriMaterialBase>();
  params.addClassDescription(
      "Compute the diffusion coefficient for three materials defined by two level set functions.");
  return params;
}

LevelSetTriMaterialDiffusion::LevelSetTriMaterialDiffusion(const InputParameters & parameters)
  : LevelSetTriMaterialBase(parameters),
    _trimaterial_diffusion_coefficient(3),
    _diffusion_coefficient(declareProperty<Real>(_base_name + "diffusion_coefficient"))
{
  _trimaterial_diffusion_coefficient[0] = &getMaterialProperty<Real>(
      getParam<std::string>("levelset_positive_positive_base") + "_diffusion_coefficient");
  _trimaterial_diffusion_coefficient[1] = &getMaterialProperty<Real>(
      getParam<std::string>("levelset_negative_positive_base") + "_diffusion_coefficient");
  _trimaterial_diffusion_coefficient[2] = &getMaterialProperty<Real>(
      getParam<std::string>("levelset_negative_negative_base") + "_diffusion_coefficient");
}

void
LevelSetTriMaterialDiffusion::assignQpPropertiesForLevelSetPositivePositive()
{
  _diffusion_coefficient[_qp] = (*_trimaterial_diffusion_coefficient[0])[_qp];
}

void
LevelSetTriMaterialDiffusion::assignQpPropertiesForLevelSetNegativePositive()
{
  _diffusion_coefficient[_qp] = (*_trimaterial_diffusion_coefficient[1])[_qp];
}

void
LevelSetTriMaterialDiffusion::assignQpPropertiesForLevelSetNegativeNegative()
{
  _diffusion_coefficient[_qp] = (*_trimaterial_diffusion_coefficient[2])[_qp];
}
