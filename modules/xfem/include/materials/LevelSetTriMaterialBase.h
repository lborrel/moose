/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef LEVELSETTRIMATERIALBASE_H
#define LEVELSETTRIMATERIALBASE_H

#include "Material.h"

// Forward Declarations
class LevelSetTriMaterialBase;
class XFEM;

template <>
InputParameters validParams<LevelSetTriMaterialBase>();

/**
 * Base class for switching between materials in a tri-material system where the two interfaces are defined
 * by two level set functions.
 */
class LevelSetTriMaterialBase : public Material
{
public:
  LevelSetTriMaterialBase(const InputParameters & parameters);

protected:
  virtual void computeProperties();
  virtual void computeQpProperties();

  /**
   * assign the material properties for the region where both level set functions are negative.
   */
  virtual void assignQpPropertiesForLevelSetNegativeNegative() = 0;

  /**
   * assign the material properties for the region where the first level set function is negative and the second one is positive.
   */
  virtual void assignQpPropertiesForLevelSetNegativePositive() = 0;

  /**
   * assign the material properties for the region where both level set functions are positive.
   */
  virtual void assignQpPropertiesForLevelSetPositivePositive() = 0;

  /// global material properties
  std::string _base_name;

  /// shared pointer to XFEM
  std::shared_ptr<XFEM> _xfem;

  /// The variable number of the first level set variable we are operating on
  const unsigned int _level_set_1_var_number;

  /// The variable number of the second level set variable we are operating on
  const unsigned int _level_set_2_var_number;

  /// system reference
  const System & _system;

  /// the subproblem solution vector
  const NumericVector<Number> * _solution;

  /// use the material properties of the region where the first level set function is positive
  bool _use_positive_property_1;

  /// use the material properties of the region where the second level set function is negative
  bool _use_positive_property_2;
};

#endif // LEVELSETTRIMATERIALBASE_H
