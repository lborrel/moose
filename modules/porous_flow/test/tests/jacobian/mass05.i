# 2phase (PP)
# vanGenuchten, constant-bulk density for each phase, constant porosity, 3components (that exist in both phases)
# unsaturated
[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 1
  ny = 1
[]

[GlobalParams]
  PorousFlowDictator = dictator
[]

[Variables]
  [./ppwater]
  [../]
  [./ppgas]
  [../]
  [./massfrac_ph0_sp0]
  [../]
[]

[AuxVariables]
  [./massfrac_ph0_sp1]
  [../]
  [./massfrac_ph1_sp0]
  [../]
  [./massfrac_ph1_sp1]
  [../]
[]

[ICs]
  [./ppwater]
    type = RandomIC
    variable = ppwater
    min = -1
    max = 0
  [../]
  [./ppgas]
    type = RandomIC
    variable = ppgas
    min = 0
    max = 1
  [../]
  [./massfrac_ph0_sp0]
    type = RandomIC
    variable = massfrac_ph0_sp0
    min = 0
    max = 0.4
  [../]
  [./massfrac_ph0_sp1]
    type = RandomIC
    variable = massfrac_ph0_sp1
    min = 0
    max = 0.4
  [../]
  [./massfrac_ph1_sp0]
    type = RandomIC
    variable = massfrac_ph1_sp0
    min = 0
    max = 0.4
  [../]
  [./massfrac_ph1_sp1]
    type = RandomIC
    variable = massfrac_ph1_sp1
    min = 0
    max = 0.4
  [../]
[]

[Kernels]
  [./mass_sp0]
    type = PorousFlowMassTimeDerivative
    fluid_component = 0
    variable = ppwater
  [../]
  [./mass_sp1]
    type = PorousFlowMassTimeDerivative
    fluid_component = 1
    variable = ppgas
  [../]
  [./mass_sp2]
    type = PorousFlowMassTimeDerivative
    fluid_component = 2
    variable = massfrac_ph0_sp0
  [../]
[]

[UserObjects]
  [./dictator]
    type = PorousFlowDictator
    porous_flow_vars = 'ppwater ppgas massfrac_ph0_sp0'
    number_fluid_phases = 2
    number_fluid_components = 3
  [../]
  [./pc]
    type = PorousFlowCapillaryPressureVG
    m = 0.5
    alpha = 1
  [../]
[]

[Modules]
  [./FluidProperties]
    [./simple_fluid0]
      type = SimpleFluidProperties
      bulk_modulus = 1.5
      density0 = 1
      thermal_expansion = 0
    [../]
    [./simple_fluid1]
      type = SimpleFluidProperties
      bulk_modulus = 0.5
      density0 = 0.5
      thermal_expansion = 0
    [../]
  [../]
[]

[Materials]
  [./temperature]
    type = PorousFlowTemperature
    at_nodes = true
  [../]
  [./ppss]
    type = PorousFlow2PhasePP
    at_nodes = true
    phase0_porepressure = ppwater
    phase1_porepressure = ppgas
    capillary_pressure = pc
  [../]
  [./massfrac]
    type = PorousFlowMassFraction
    at_nodes = true
    mass_fraction_vars = 'massfrac_ph0_sp0 massfrac_ph0_sp1 massfrac_ph1_sp0 massfrac_ph1_sp1'
  [../]
  [./simple_fluid0]
    type = PorousFlowSingleComponentFluid
    fp = simple_fluid0
    at_nodes = true
    phase = 0
  [../]
  [./simple_fluid1]
    type = PorousFlowSingleComponentFluid
    fp = simple_fluid1
    at_nodes = true
    phase = 1
  [../]
  [./porosity]
    type = PorousFlowPorosityConst
    at_nodes = true
    porosity = 0.1
  [../]
[]

[Preconditioning]
  active = check
  [./andy]
    type = SMP
    full = true
    petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it'
    petsc_options_value = 'bcgs bjacobi 1E-15 1E-10 10000'
  [../]
  [./check]
    type = SMP
    full = true
    petsc_options_iname = '-ksp_type -pc_type -snes_atol -snes_rtol -snes_max_it -snes_type'
    petsc_options_value = 'bcgs bjacobi 1E-15 1E-10 10000 test'
  [../]
[]

[Executioner]
  type = Transient
  solve_type = Newton
  dt = 1
  end_time = 1
[]

[Outputs]
  exodus = false
[]
