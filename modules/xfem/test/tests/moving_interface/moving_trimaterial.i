[GlobalParams]
  order = FIRST
  family = LAGRANGE
[]

[XFEM]
  qrule = volfrac
  output_cut_plane = true
[]

[UserObjects]
  [./level_set_cut_uo_1]
    type = LevelSetCutUserObject
    level_set_var = ls_1
    heal_always = true
  [../]
  [./level_set_cut_uo_2]
    type = LevelSetCutUserObject
    level_set_var = ls_2
    heal_always = true
  [../]
[]

[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 21
  ny = 21
  xmin = 0.0
  xmax = 10
  ymin = 0.0
  ymax = 10
  elem_type = QUAD4
[]

[AuxVariables]
  [./ls_1]
    order = FIRST
    family = LAGRANGE
  [../]
  [./ls_2]
    order = FIRST
    family = LAGRANGE
  [../]
[]

[AuxKernels]
  [./ls_function_1]
    type = FunctionAux
    variable = ls_1
    function = ls_func_1
  [../]
  [./ls_function_2]
    type = FunctionAux
    variable = ls_2
    function = ls_func_2
  [../]
[]

[Variables]
  [./u]
  [../]
[]

[Functions]
  [./ls_func_1]
    type = ParsedFunction
    value = 'x-9.8+0.11*t'
  [../]
  [./ls_func_2]
    type = ParsedFunction
    value = 'x-7.9+0.05*t'
  [../]
[]

[Kernels]
  [./diff]
    type = XFEMDiffusion
    variable = u
  [../]
  [./time_deriv]
    type = TimeDerivative
    variable = u
  [../]
[]

[Constraints]
  [./u_constraint_1]
    type = XFEMSingleVariableConstraint
    use_displaced_mesh = false
    variable = u
    jump = -0.3
    use_penalty = true
    alpha = 1e5
    geometric_cut_userobject = 'level_set_cut_uo_1'
  [../]
  [./u_constraint_2]
    type = XFEMSingleVariableConstraint
#     use_displace_mesh = false
    variable = u
    jump = -0.1
    use_penalty = true
    alpha = 1e5
    geometric_cut_userobject = 'level_set_cut_uo_2'
  [../]
[]

[BCs]
  [./left_u]
    type = DirichletBC
    variable = u
    boundary = left
    value = 0
  [../]
  [./right_u]
    type = DirichletBC
    variable = u
    boundary = right
    value = 1
  [../]
[]

[Materials]
  [./diffusivity_A_ox]
    type = GenericConstantMaterial
    prop_names = A_diffusion_coefficient
    prop_values = 100
  [../]

  [./diffusivity_B_alpha]
    type = GenericConstantMaterial
    prop_names = B_diffusion_coefficient
    prop_values = 1
  [../]

  [./diffusivity_C_beta]
    type = GenericConstantMaterial
    prop_names = C_diffusion_coefficient
    prop_values = 0.01
  [../]

  [./diff_combined]
    type = LevelSetTriMaterialDiffusion
    levelset_positive_positive_base = 'A'
    levelset_negative_positive_base = 'B'
    levelset_negative_negative_base = 'C'
    level_set_1_var = ls_1
    level_set_2_var = ls_2
  [../]
[]

[Executioner]
  type = Transient

  solve_type = 'PJFNK'
  # petsc_options_iname = '-ksp_gmres_restart -pc_type -pc_hypre_type -pc_hypre_boomeramg_max_iter'
  # petsc_options_value = '201                hypre    boomeramg      8'


  petsc_options_iname = '-pc_type -pc_factor_mat_solver_package'
  petsc_options_value = 'lu     superlu_dist'

  line_search = 'bt'

  #[./Predictor]
  #  type = SimplePredictor
  #  scale = 1.0
  #[../]

# controls for linear iterations
  l_max_its = 20
  l_tol = 1e-9

# controls for nonlinear iterations
  nl_max_its = 15
  nl_rel_tol = 1e-9
  nl_abs_tol = 1e-9

# time control
  start_time = 0.0
  dt = 1
  end_time = 20.0
  num_steps = 5000

  max_xfem_update = 1
[]

[Outputs]
  exodus = true
  execute_on = timestep_end
  csv = true
  [./console]
    type = Console
    perf_log = true
    output_linear = true
  [../]
[]
