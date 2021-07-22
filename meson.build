project('RTSoftSummerSchool2021-practice', 'cpp',
  default_options:
  [
    'cpp_std=none',
    'werror=true',
    'warning_level=3',
    'optimization=3'
  ]
)
add_project_arguments([ '-std=c++20' ], language : 'cpp') 


opencv = dependency('opencv')

executable('RTSoftSummerSchool2021-practice',
  [
    'src/main.cpp',
    'src/LineDetection.cpp'
  ],
  dependencies:
  [
    opencv
  ]
)
