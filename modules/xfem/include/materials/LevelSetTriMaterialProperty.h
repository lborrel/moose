/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef LEVELSETTRIMATERIALPROPERTY_H
#define LEVELSETTRIMATERIALPROPERTY_H

#include "LevelSetTriMaterialBase.h"

// Forward Declarations
class LevelSetTriMaterialProperty;
class XFEM;

registerMooseObject("XFEMApp", LevelSetTriMaterialProperty);

template <>
InputParameters validParams<LevelSetTriMaterialProperty>();

/**
 * Compute the diffusion for tri-materials problem (consisting of three different materials) defined by two 
 * level set functions
 *
 */
class LevelSetTriMaterialProperty : public LevelSetTriMaterialBase
{
public:
  LevelSetTriMaterialProperty(const InputParameters & parameters);

protected:
  virtual void assignQpPropertiesForLevelSetPositivePositive();
  virtual void assignQpPropertiesForLevelSetNegativePositive();
  virtual void assignQpPropertiesForLevelSetNegativeNegative();

  /// diffusion coefficient for the two separate materials in the tri-material system
  std::vector<const MaterialProperty<Real> *> _trimaterial_material_prop;

  /// Property name
  std::string _prop_name;

  /// global diffusion coefficient (switch tri-material diffusion coefficient based on the values of the two level set functions)
  MaterialProperty<Real> & _material_prop;
};

#endif // LEVELSETTRIMATERIALPROPERTY_H
