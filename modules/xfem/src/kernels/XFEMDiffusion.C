/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#include "XFEMDiffusion.h"
#include "MooseMesh.h"

template <>
InputParameters
validParams<XFEMDiffusion>()
{
  InputParameters params = validParams<Diffusion>();
  params.addClassDescription(
      "Computes residual/Jacobian contribution for $(k \\nabla T, \\nabla \\psi)$ term.");
  params.addParam<std::string>("base_name",
                               "Optional parameter that allows the user to define "
                               "mutliple materials systems on the same block, "
                               "i.e. for multiple phases");
  params.set<bool>("use_displaced_mesh") = true;
  return params;
}

XFEMDiffusion::XFEMDiffusion(const InputParameters & parameters)
  : Diffusion(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _diffusivity_name(_base_name + "diffusivity"),
    _diffusivity(getMaterialProperty<Real>(_diffusivity_name))
//    _diffusion_coefficient_dT(hasMaterialProperty<Real>("diffusion_coefficient_dT")
//                                  ? &getMaterialProperty<Real>("diffusion_coefficient_dT")
//                                  : NULL)
{
}

Real
XFEMDiffusion::computeQpResidual()
{
  return _diffusivity[_qp] * Diffusion::computeQpResidual();
}

Real
XFEMDiffusion::computeQpJacobian()
{
  Real jac = _diffusivity[_qp] * Diffusion::computeQpJacobian();
//  if (_diffusion_coefficient_dT)
//    jac += (*_diffusion_coefficient_dT)[_qp] * _phi[_j][_qp] * Diffusion::computeQpResidual();
  return jac;
}
