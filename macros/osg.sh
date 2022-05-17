#!/bin/bash

pattern='config[0-9]+'
input=${1}
[[ $input =~ $pattern ]]
config=${BASH_REMATCH[0]}

pattern='Run[0-9]+'
[[ $input =~ $pattern ]]
Run=${BASH_REMATCH[0]}

pattern='Ring[0-9]+'
[[ $input =~ $pattern ]]
Ring=${BASH_REMATCH[0]}

pattern='Quartz|LowerLg|UpperLg'
[[ $input =~ $pattern ]]
HR=${BASH_REMATCH[0]}

[[ $config =~ [0-9]+ ]]
ID=${BASH_REMATCH[0]}

fname=$( printf MOLLEROpt%s_%s_%s_Config_%04d.root $Run $Ring $HR $ID)

EICSHELL="eic-shell"
cat << EOF | ${EICSHELL}
  git clone --branch feature-addJobSubmissionScript --single-branch https://github.com/rahmans1/MOLLER-IntDetectorOpticalSim
  cd MOLLER-IntDetectorOpticalSim
  mkdir build
  cd build
  cp ../../$config .
  cmake ..
  make 
  ./MOLLEROpt $config
EOF

stashcp ${fname} stash:///osgconnect/public/rahmans1/${fname}

