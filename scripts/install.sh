#!/bin/bash

COLOR_GREEN='\033[0;32m'
COLOR_RED='\033[0;31m'
COLOR_OFF='\033[0m' # No Color

# This script compiles and installs polycube and its dependencies

function print_system_info {
  echo "***********************SYSTEM INFO*************************************"
  echo "kernel version:" $(uname -r)
  echo "linux release info:" $(cat /etc/*-release)
  echo "g++ version:" $(g++ --version)
  echo "gcc version:" $(gcc --version)
  echo "golang-version:" $(go version)
  # TODO: what else is important to gather?
}

function error_message {
  set +x
  echo
  echo 'Error during installation'
  print_system_info
  exit 1
}
function success_message {
  set +x
  echo
  echo 'Installation completed successfully'
  echo
  echo 'You can now start the polycube daemon:'
  echo '  manually:     sudo polycubed -d'
  echo '  with systemd: sudo systemctl start polycubed'
  echo 'and then interact with it using the Polycube command line:'
  echo '  polycubectl -h'
  echo
  exit 0
}
trap error_message ERR

function show_help() {
usage="$(basename "$0") [mode]
Polycube installation script

mode:
  pcn-iptables: install only pcn-iptables service and related components
  pcn-k8s: install only pcn-k8s (only to be used with docker build)
  update: updates the source code of polycube before *default* install
  default: install all available polycube services"
echo "$usage"
echo
}

while getopts h option; do
 case "${option}" in
 h|\?)
  show_help
  exit 0
 esac
done

echo "Use 'install.sh -h' to show advanced installation options."

MODE=$1

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

[ -z ${SUDO+x} ] && SUDO='sudo'
[ -z ${WORKDIR+x} ] && WORKDIR=$DIR/../deps
[ -z ${MODE+x} ] && MODE='default'

if [ -d "$WORKDIR" ]; then
  mkdir -p $WORKDIR
fi

# print bash commands and their arguments as they are executed
set -x
# exit immediately if a command exits with a non-zero status
set -e

git submodule init
git submodule update --recursive

source scripts/pre-requirements.sh

echo "Install Prometheus-cpp"
cd $DIR/../src/libs/prometheus-cpp
set +e
if [ -d _build ]; then
    $SUDO rm -rf _build
fi
mkdir _build
cd _build
cmake .. -DBUILD_SHARED_LIBS=ON
make -j $(getconf _NPROCESSORS_ONLN)
$SUDO make install


echo "Install polycube"
cd $DIR/..
if [ "$INSTALL_CLEAN_POLYCUBE" == true ] ; then
  $SUDO rm -rf build
fi

if [ "$MODE" == "update" ] ; then
  echo "Update to the latest polycube source code"
  # Commands below update also the code in this repository and
  # all the submodules, so that you install the most recent code
  # from the current branch (default: master)
  git pull
  git submodule update --init --recursive

  # Print the last commit in the current branch, just to make sure
  # we're compiling exactly what we expect
  git log -1
fi

rm -rf build
mkdir -p build && cd build

CMAKE_CMD="cmake .. -DCMAKE_C_COMPILER=/usr/bin/gcc-9 \
                    -DCMAKE_CXX_COMPILER=/usr/bin/g++-9 \
                    -DLLVM_DIR="${WORKDIR}"/clang+llvm10/lib/cmake/llvm"
#-Dfolly_DIR=${FOLLY_INSTALL_DIR} \
#-Dfmt_DIR=${FMT_INSTALL_DIR} \
#-Dgflags_DIR=${GFLAGS_INSTALL_DIR}
#"
# depending on the mode different services are enabled
$CMAKE_CMD -DENABLE_PCN_IPTABLES=OFF

make -j $(getconf _NPROCESSORS_ONLN)
$SUDO make install

success_message
