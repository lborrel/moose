/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "LevelSetMultiDiffusionMaterial.h"

template <>
InputParameters
validParams<LevelSetMultiDiffusionMaterial>()
{
  InputParameters params = validParams<Material>();
  params.addClassDescription("Compute a global diffusivity form multiple phase diffusivity");
  params.addRequiredCoupledVar("level_set_var", "Level set variable");
  params.addRequiredParam<std::string>("levelset_plus_base",
                                       "Base name for the diffusivity in level set plus region.");
  params.addRequiredParam<std::string>("levelset_minus_base",
                                       "Base name for the diffusivity in level set minus region.");
  params.addParam<std::string>("base_name", "Base name for the computed global diffusivity (optional)");
  return params;
}

LevelSetMultiDiffusionMaterial::LevelSetMultiDiffusionMaterial(const InputParameters & parameters)
  : Material(parameters),
    _ls(coupledValue("level_set_var")),
    _phase_base(2),
    _phase_diffusivity(2),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _diffusivity(declareProperty<Real>(_base_name + "diffusivity"))
{
  _phase_diffusivity[0] =
      &getMaterialProperty<Real>(getParam<std::string>("levelset_plus_base") + "_diffusivity");

  _phase_diffusivity[1] =
      &getMaterialProperty<Real>(getParam<std::string>("levelset_minus_base") + "_diffusivity");
}

void
LevelSetMultiDiffusionMaterial::computeQpProperties()
{
  _diffusivity[_qp] = 0;

  if (_ls[_qp] > 0)
  {
    _diffusivity[_qp] = (*_phase_diffusivity[0])[_qp];
  }
  else
  {
    _diffusivity[_qp] = (*_phase_diffusivity[1])[_qp];
  }
}
