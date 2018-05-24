/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef LEVELSETTRIMATERIALDIFFUSION_H
#define LEVELSETTRIMATERIALDIFFUSION_H

#include "LevelSetTriMaterialBase.h"

// Forward Declarations
class LevelSetTriMaterialDiffusion;
class XFEM;

registerMooseObject("XFEMApp", LevelSetTriMaterialDiffusion);

template <>
InputParameters validParams<LevelSetTriMaterialDiffusion>();

/**
 * Compute the diffusion for tri-materials problem (consisting of three different materials) defined by two 
 * level set functions
 *
 */
class LevelSetTriMaterialDiffusion : public LevelSetTriMaterialBase
{
public:
  LevelSetTriMaterialDiffusion(const InputParameters & parameters);

protected:
  virtual void assignQpPropertiesForLevelSetPositivePositive();
  virtual void assignQpPropertiesForLevelSetNegativePositive();
  virtual void assignQpPropertiesForLevelSetNegativeNegative();

  /// diffusion coefficient for the two separate materials in the tri-material system
  std::vector<const MaterialProperty<Real> *> _trimaterial_diffusion_coefficient;

  /// global diffusion coefficient (switch tri-material diffusion coefficient based on the values of the two level set functions)
  MaterialProperty<Real> & _diffusion_coefficient;
};

#endif // LEVELSETTRIMATERIALDIFFUSION_H
