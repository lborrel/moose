/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "LevelSetTriMaterialProperty.h"

template <>
InputParameters
validParams<LevelSetTriMaterialProperty>()
{
  InputParameters params = validParams<LevelSetTriMaterialBase>();
  params.addClassDescription(
      "Compute the diffusion coefficient for three materials defined by two level set functions.");
  params.addRequiredParam<std::string>("prop_name",
                                       "Name for the computed material property (optional)");
  return params;
}

LevelSetTriMaterialProperty::LevelSetTriMaterialProperty(const InputParameters & parameters)
  : LevelSetTriMaterialBase(parameters),
    _trimaterial_material_prop(3),
    _prop_name(getParam<std::string>("prop_name")),
    _material_prop(declareProperty<Real>(_base_name + _prop_name))
{
  _trimaterial_material_prop[0] = &getMaterialProperty<Real>(
      getParam<std::string>("levelset_positive_positive_base") + "_" + _prop_name);
  _trimaterial_material_prop[1] = &getMaterialProperty<Real>(
      getParam<std::string>("levelset_negative_positive_base") + "_" + _prop_name);
  _trimaterial_material_prop[2] = &getMaterialProperty<Real>(
      getParam<std::string>("levelset_negative_negative_base") + "_" + _prop_name);
}

void
LevelSetTriMaterialProperty::assignQpPropertiesForLevelSetPositivePositive()
{
  _material_prop[_qp] = (*_trimaterial_material_prop[0])[_qp];
}

void
LevelSetTriMaterialProperty::assignQpPropertiesForLevelSetNegativePositive()
{
  _material_prop[_qp] = (*_trimaterial_material_prop[1])[_qp];
}

void
LevelSetTriMaterialProperty::assignQpPropertiesForLevelSetNegativeNegative()
{
  _material_prop[_qp] = (*_trimaterial_material_prop[2])[_qp];
}
