project('lab1', 'c',
  default_options: [
    'b_ndebug=if-release',
    'buildtype=debugoptimized',
    'c_std=c89',
    'warning_level=3',
  # 'werror=true',
  ],
  license: 'GPL-3.0-or-later',
  version: '0.0.0-development',
)

architecture = get_option('architecture')
assert(architecture != '', 'Please specify architecture with -Darchitecture=')

compiler = get_option('compiler')
assert(compiler != '', 'Please specify compiler with -Dcompiler=')

subdir('subprojects/uC-CPU')
subdir('subprojects/uC-LIB')
subdir('subprojects/uC-OS3')

lab1_deps = [uccpu_deps, uclib_deps, ucos3_deps]
lab1_inc = [include_directories('include'), uccpu_inc, uclib_inc, ucos3_inc]
lab1_src = [uccpu_src, uclib_src, ucos3_src] + files(
  'src' / architecture / compiler / 'bsp_cpu.c',
  'src/os_app_hooks.c',
  'src/routeur.c',
)

executable('exe', lab1_src,
  dependencies: lab1_deps,
  include_directories: lab1_inc,
  install: true,
)
