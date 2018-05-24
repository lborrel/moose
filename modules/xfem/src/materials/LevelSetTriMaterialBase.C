/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#include "LevelSetTriMaterialBase.h"
#include "AuxiliarySystem.h"
#include "MooseVariable.h"
#include "XFEM.h"

template <>
InputParameters
validParams<LevelSetTriMaterialBase>()
{
  InputParameters params = validParams<Material>();
  params.addClassDescription("Compute a material property for tri-materials (consisting of three "
                             "different materials) defined by two level set functions.");
  params.addRequiredParam<VariableName>(
      "level_set_1_var", "The name of the first level set variable used to represent the first interface");
  params.addRequiredParam<VariableName>(
      "level_set_2_var", "The name of the second level set variable used to represent the second interface");
  params.addRequiredParam<std::string>("levelset_positive_positive_base",
                                       "Base name for the material in the region where both level set functions are positive.");
  params.addRequiredParam<std::string>("levelset_negative_positive_base",
                                       "Base name for the material in the region where the first level set function is negative and the second one is positive.");
  params.addRequiredParam<std::string>("levelset_negative_negative_base",
                                       "Base name for the material in the region where both level set functions are negative.");
  params.addParam<std::string>("base_name",
                               "Base name for the computed material property (optional)");
  return params;
}

LevelSetTriMaterialBase::LevelSetTriMaterialBase(const InputParameters & parameters)
  : Material(parameters),
    _base_name(isParamValid("base_name") ? getParam<std::string>("base_name") + "_" : ""),
    _level_set_1_var_number(
        _subproblem.getVariable(_tid, parameters.get<VariableName>("level_set_1_var")).number()),
    _level_set_2_var_number(
        _subproblem.getVariable(_tid, parameters.get<VariableName>("level_set_2_var")).number()),
    _system(_subproblem.getSystem(getParam<VariableName>("level_set_1_var"))),
    _solution(_system.current_local_solution.get()),
    _use_positive_property_1(false),
    _use_positive_property_2(false)
{
  FEProblemBase * fe_problem = dynamic_cast<FEProblemBase *>(&_subproblem);

  if (fe_problem == NULL)
    mooseError("Problem casting _subproblem to FEProblemBase in XFEMMaterialStateMarkerBase");

  _xfem = MooseSharedNamespace::dynamic_pointer_cast<XFEM>(fe_problem->getXFEM());
}

void
LevelSetTriMaterialBase::computeProperties()
{
  const Node * node = _current_elem->node_ptr(0);

  dof_id_type ls_1_dof_id = node->dof_number(_system.number(), _level_set_1_var_number, 0);
  Number ls_1_node_value = (*_solution)(ls_1_dof_id);
  dof_id_type ls_2_dof_id = node->dof_number(_system.number(), _level_set_2_var_number, 0);
  Number ls_2_node_value = (*_solution)(ls_2_dof_id);

  if (_xfem->isPointInsidePhysicalDomain(_current_elem, *node))
  {
    if (ls_1_node_value > 0.0 && ls_2_node_value > 0.0)
    {
      _use_positive_property_1 = true;
      _use_positive_property_2 = true;
    }
    else if (ls_1_node_value < 0.0 && ls_2_node_value > 0.0)
    {
      _use_positive_property_1 = false;
      _use_positive_property_2 = true;
    }
    else if (ls_1_node_value < 0.0 && ls_2_node_value < 0.0)
    {
      _use_positive_property_1 = false;
      _use_positive_property_2 = false;
    }
  }
  else
  {
    if (ls_1_node_value > 0.0 && ls_2_node_value > 0.0)
    {
      _use_positive_property_1 = false;
      _use_positive_property_2 = true;
    }
    else if (ls_1_node_value < 0.0 && ls_2_node_value > 0.0)
    {
      if (-ls_1_node_value < ls_2_node_value)
      {
        _use_positive_property_1 = true;
        _use_positive_property_2 = true;
      }
      else if (ls_2_node_value < -ls_1_node_value)
      {
        _use_positive_property_1 = false;
        _use_positive_property_2 = false;
      }
    }
    else if (ls_1_node_value < 0.0 && ls_2_node_value < 0.0)
    {
      _use_positive_property_1 = false;
      _use_positive_property_2 = true;
    }
  }

  Material::computeProperties();
}

void
LevelSetTriMaterialBase::computeQpProperties()
{
  if (_use_positive_property_1 && _use_positive_property_2)
    assignQpPropertiesForLevelSetPositivePositive();
  else if (!_use_positive_property_1 && _use_positive_property_2)
  	assignQpPropertiesForLevelSetNegativePositive();
  else if (!_use_positive_property_1 && !_use_positive_property_2)
    assignQpPropertiesForLevelSetNegativeNegative();
}
