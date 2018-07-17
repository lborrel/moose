//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef LINESEGMENTLEVELSETAUXOXIDEMETAL_H
#define LINESEGMENTLEVELSETAUXOXIDEMETAL_H

#include "AuxKernel.h"

// Forward Declarations
class LineSegmentLevelSetAuxOxideMetal;
class MovingLineSegmentCutSetUserObjectOxideMetal;

template <>
InputParameters validParams<LineSegmentLevelSetAuxOxideMetal>();

/**
 * Function auxiliary value
 */
class LineSegmentLevelSetAuxOxideMetal : public AuxKernel
{
public:
  /**
   * Factory constructor, takes parameters so that all derived classes can be built using the same
   * constructor.
   */
  LineSegmentLevelSetAuxOxideMetal(const InputParameters & parameters);

protected:
  virtual Real computeValue() override;
  virtual void compute() override;

  Real calculateSignedDistance(Point p);

  /// Function being used to compute the value of this kernel
  const MovingLineSegmentCutSetUserObjectOxideMetal * _linesegment_uo;

  std::vector<Real> _cut_data;
};

#endif // LINESEGMENTLEVELSETAUXOXIDEMETAL_H
