/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef LEVELSETMULTIDIFFUSIONMATERIAL_H
#define LEVELSETMULTIDIFFUSIONMATERIAL_H

#include "Material.h"

// Forward Declarations
class LevelSetMultiDiffusionMaterial;

template <>
InputParameters validParams<LevelSetMultiDiffusionMaterial>();

/**
 * Construct a global strain from the phase strains in a manner that is consistent
 * with the construction of the global elastic energy by DerivativeMultiPhaseMaterial.
 */
class LevelSetMultiDiffusionMaterial : public Material
{
public:
  LevelSetMultiDiffusionMaterial(const InputParameters & parameters);

protected:
  virtual void computeQpProperties();

  const VariableValue & _ls;

  // phase material properties
  std::vector<std::string> _phase_base;
  std::vector<const MaterialProperty<Real> *> _phase_diffusivity;

  // global material properties
  std::string _base_name;
  MaterialProperty<Real> & _diffusivity;
};

#endif // LEVELSETMULTIDIFFUSIONMATERIAL_H
