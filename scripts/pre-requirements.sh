#!/bin/bash
if [ -z "$DIR" ]
then
    DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
fi
mkdir -p $WORKDIR

$SUDO apt update
$SUDO bash -c "apt install --allow-unauthenticated -y wget gnupg2 software-properties-common"

# Release in which has been natively introduced support for golang-go (Ubuntu >= 20.04)
os_limit_major="20"
os_limit_minor="04"
read -r os_major os_minor<<<$(grep -Po 'VERSION_ID="\K.*?(?=")' /etc/os-release | sed 's/\./ /g')


get_flex() {
  # licd $WORKDIR
  FLEX_DIR=$WORKDIR/flex

  if [ -f "$WORKDIR/flex_installed" ]; then
      return
  fi
  
  rm -rf "$FLEX_DIR"
  pushd .
  cd "$WORKDIR"
  echo -e "${COLOR_GREEN}[ INFO ] Cloning flex repo ${COLOR_OFF}"
  git clone --depth 1 --branch v2.6.4 https://github.com/westes/flex.git
  cd "$FLEX_DIR"
  ./autogen.sh
  ./configure 
  make -j $(getconf _NPROCESSORS_ONLN)
  $SUDO make install

  echo -e "${COLOR_GREEN}flex is installed ${COLOR_OFF}"
  popd
  touch "$WORKDIR/flex_installed"
}

get_llvm() {
  pushd .
  cd "$WORKDIR"
  echo -e "${COLOR_GREEN}[ INFO ] Download LLVM 10 binaries ${COLOR_OFF}"
  if [ ! -d clang+llvm10 ]; then
      mkdir clang+llvm10
      wget https://github.com/llvm/llvm-project/releases/download/llvmorg-10.0.1/clang+llvm-10.0.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz
      tar xf clang+llvm-10.0.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz -C clang+llvm10 --strip-components 1
      rm clang+llvm-10.0.1-x86_64-linux-gnu-ubuntu-16.04.tar.xz
  fi

  echo -e "${COLOR_GREEN}LLVM is installed ${COLOR_OFF}"
  popd
}

get_bison() {
  BISON_DIR=$WORKDIR/bison-3.8

  if [ -f "$WORKDIR/bison_installed" ]; then
      return
  fi

  rm -rf "$BISON_DIR"
  pushd .
  cd "$WORKDIR"
  echo -e "${COLOR_GREEN}[ INFO ] Download BISON 3.8 Binaries ${COLOR_OFF}"
  mkdir -p "$BISON_DIR"
  wget https://mirror.easyname.at/gnu/bison/bison-3.8.tar.gz
  tar -xvf bison-3.8.tar.gz -C $BISON_DIR --strip-components 1
  rm bison-3.8.tar.gz

  cd "$BISON_DIR"
  ./configure
  make -j $(getconf _NPROCESSORS_ONLN)
  $SUDO make install

  echo -e "${COLOR_GREEN}BISON is installed ${COLOR_OFF}"
  popd
  
  touch "$WORKDIR/bison_installed"
}

get_yaml_cpp() {
  # licd $WORKDIR
  YAML_CPP_DIR=$WORKDIR/yaml-cpp
  YAML_CPP_BUILD_DIR=$WORKDIR/yaml-cpp/build/

  if [ -f "$WORKDIR/yaml_cpp_installed" ]; then
      return
  fi
  
  rm -rf "$YAML_CPP_DIR"
  pushd .
  cd "$WORKDIR"
  echo -e "${COLOR_GREEN}[ INFO ] Cloning YAML C++ repo ${COLOR_OFF}"
  git clone --depth 1 --branch yaml-cpp-0.7.0 https://github.com/jbeder/yaml-cpp.git
  mkdir -p "$YAML_CPP_BUILD_DIR"
  cd "$YAML_CPP_BUILD_DIR"
  cmake ..
  make -j $(getconf _NPROCESSORS_ONLN)
  $SUDO make install

  echo -e "${COLOR_GREEN}YAML C++ is installed ${COLOR_OFF}"
  popd
  touch "$WORKDIR/yaml_cpp_installed"
}

get_libyang() {
  # licd $WORKDIR
  LIBYANG_DIR=$WORKDIR/libyang
  LIBYANG_BUILD_DIR=$WORKDIR/libyang/build/

  if [ -f "$WORKDIR/libyang_installed" ]; then
      return
  fi
  
  rm -rf "$LIBYANG_DIR"
  pushd .
  cd "$WORKDIR"
  echo -e "${COLOR_GREEN}[ INFO ] Cloning libyang repo ${COLOR_OFF}"
  git clone -b v0.14-r1 https://github.com/CESNET/libyang.git --depth 1
  mkdir -p "$LIBYANG_BUILD_DIR"
  cd "$LIBYANG_BUILD_DIR"
  cmake ..
  make -j $(getconf _NPROCESSORS_ONLN)
  $SUDO make install

  echo -e "${COLOR_GREEN}libyang is installed ${COLOR_OFF}"
  popd
  touch "$WORKDIR/libyang_installed"
}

get_pistache() {
  # licd $WORKDIR
  PISTACHE_DIR=$WORKDIR/pistache
  PISTACHE_BUILD_DIR=$WORKDIR/pistache/build/

  if [ -f "$WORKDIR/pistache_installed" ]; then
      return
  fi
  
  rm -rf "$PISTACHE_DIR"
  pushd .
  cd "$WORKDIR"
  echo -e "${COLOR_GREEN}[ INFO ] Cloning pistache repo ${COLOR_OFF}"
  git clone https://github.com/oktal/pistache.git
  cd "$PISTACHE_DIR"
  git checkout 117db02eda9d63935193ad98be813987f6c32b33
  git submodule update --init
  mkdir -p "$PISTACHE_BUILD_DIR"
  cd "$PISTACHE_BUILD_DIR"
  cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DPISTACHE_USE_SSL=ON ..
  make -j $(getconf _NPROCESSORS_ONLN)
  $SUDO make install

  echo -e "${COLOR_GREEN}pistache is installed ${COLOR_OFF}"
  popd
  touch "$WORKDIR/pistache_installed"
}

get_libtins() {
  LIBTINS_DIR=$WORKDIR/libtins
  LIBTINS_BUILD_DIR=$WORKDIR/libtins/build/

  if [ -f "$WORKDIR/libtins_installed" ]; then
      return
  fi

  rm -rf "$LIBTINS_DIR"
  pushd .
  cd "$WORKDIR"
  echo -e "${COLOR_GREEN}[ INFO ] Cloning libtins repo ${COLOR_OFF}"
  git clone --branch v3.5 https://github.com/mfontanini/libtins.git
  mkdir -p "$LIBTINS_BUILD_DIR"
  cd "$LIBTINS_BUILD_DIR"
  cmake -DLIBTINS_ENABLE_CXX11=1 \
    -DLIBTINS_BUILD_EXAMPLES=OFF -DLIBTINS_BUILD_TESTS=OFF \
    -DLIBTINS_ENABLE_DOT11=OFF -DLIBTINS_ENABLE_PCAP=OFF \
    -DLIBTINS_ENABLE_WPA2=OFF -DLIBTINS_ENABLE_WPA2_CALLBACKS=OFF ..
  make -j $(getconf _NPROCESSORS_ONLN)
  $SUDO make install
  $SUDO ldconfig

  echo -e "${COLOR_GREEN}libtins is installed ${COLOR_OFF}"
  popd
  touch "$WORKDIR/libtins_installed"
}

get_fmt() {
  FMT_DIR=$WORKDIR/fmt
  FMT_BUILD_DIR=$WORKDIR/fmt/build/

  export FMT_INSTALL_DIR=${FMT_BUILD_DIR}

  if [ -f "$WORKDIR/fmt_installed" ]; then
      return
  fi
  rm -rf "$FMT_DIR"
  pushd .
  cd "$WORKDIR"
  echo -e "${COLOR_GREEN}[ INFO ] Cloning fmt repo ${COLOR_OFF}"
  git clone https://github.com/fmtlib/fmt
  mkdir -p "$FMT_BUILD_DIR"
  cd "$FMT_BUILD_DIR"
  cmake -DCXX_STD=gnu++17                         \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo             \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON          \
    ..
  make -j $(getconf _NPROCESSORS_ONLN)
  $SUDO make install
  echo -e "${COLOR_GREEN}fmt is installed ${COLOR_OFF}"
  popd
  touch "$WORKDIR/fmt_installed"
}

get_gflags() {
  GFLAGS_DIR=$WORKDIR/gflags
  GFLAGS_BUILD_DIR=$WORKDIR/gflags/_build

  export GFLAGS_INSTALL_DIR=${GFLAGS_BUILD_DIR}

  if [ -f "${WORKDIR}/gflags_installed" ]; then
    return
  fi
  rm -rf "$GFLAGS_DIR"
  pushd .
  echo -e "${COLOR_GREEN}[ INFO ] Cloning gflags repo ${COLOR_OFF}"
  git clone https://github.com/gflags/gflags --depth 1 --branch v2.2.2 "$GFLAGS_DIR"
  echo -e "${COLOR_GREEN}[ INFO ] Building gflags ${COLOR_OFF}"
  mkdir -p "$GFLAGS_BUILD_DIR"
  cd "$GFLAGS_BUILD_DIR" || exit

  cmake  -DCXX_STD=gnu++17                        \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo             \
    -DBUILD_SHARED_LIBS=ON                        \
    -DBUILD_STATIC_LIBS=ON                        \
    ..

  make -j $(getconf _NPROCESSORS_ONLN)
  $SUDO make install
  echo -e "${COLOR_GREEN}Gflags is installed ${COLOR_OFF}"
  popd
  touch "${WORKDIR}/gflags_installed"
}

get_folly() {
  FOLLY_DIR=$WORKDIR/folly
  FOLLY_BUILD_DIR=$WORKDIR/folly/build

  export FOLLY_INSTALL_DIR=${FOLLY_BUILD_DIR}

  if [ -f "${WORKDIR}/folly_installed" ]; then
    return
  fi

  rm -rf "$FOLLY_DIR"
  $SUDO apt-get install -y       \
      g++                       \
      automake                  \
      autoconf                  \
      autoconf-archive          \
      libtool                   \
      libboost-all-dev          \
      libevent-dev              \
      libdouble-conversion-dev  \
      libgoogle-glog-dev        \
      libgflags-dev             \
      liblz4-dev                \
      liblzma-dev               \
      libsnappy-dev             \
      make                      \
      zlib1g-dev                \
      binutils-dev              \
      libjemalloc-dev           \
      libssl-dev                \
      pkg-config                \
      libiberty-dev             \
      libunwind8-dev            \
      libdwarf-dev

  pushd .
  echo -e "${COLOR_GREEN}[ INFO ] Cloning folly repo ${COLOR_OFF}"
  git clone https://github.com/facebook/folly --depth 1 "$FOLLY_DIR"
  echo -e "${COLOR_GREEN}[ INFO ] Building Folly ${COLOR_OFF}"
  mkdir -p "$FOLLY_BUILD_DIR"
  cd "$FOLLY_BUILD_DIR" || exit

  cmake  -DCXX_STD=gnu++17                        \
    -DCMAKE_BUILD_TYPE=RelWithDebInfo             \
    -DCMAKE_POSITION_INDEPENDENT_CODE=ON          \
    ..
  make -j $(getconf _NPROCESSORS_ONLN)
  $SUDO make install
  echo -e "${COLOR_GREEN}Folly is installed ${COLOR_OFF}"
  popd
  touch "${WORKDIR}/folly_installed"
}

get_prometheus_cpp() {
  PROMETHEUS_DIR=$WORKDIR/prometheus
  PROMETHEUS_BUILD_DIR=$WORKDIR/prometheus/build

  if [ -f "${WORKDIR}/prometheus_installed" ]; then
    return
  fi

  rm -rf "$PROMETHEUS_DIR"
  pushd .
  cd "$WORKDIR"
  echo -e "${COLOR_GREEN}[ INFO ] Downloading prometheus CPP ${COLOR_OFF}"
  mkdir -p "$PROMETHEUS_DIR"
  wget https://github.com/jupp0r/prometheus-cpp/releases/download/v1.0.0/prometheus-cpp-with-submodules.tar.gz
  tar xf prometheus-cpp-with-submodules.tar.gz -C prometheus --strip-components 1
  rm prometheus-cpp-with-submodules.tar.gz
  echo -e "${COLOR_GREEN}[ INFO ] Building prometheus CPP ${COLOR_OFF}"
  mkdir -p "$PROMETHEUS_BUILD_DIR"
  cd "$PROMETHEUS_BUILD_DIR"

  cmake .. -DBUILD_SHARED_LIBS=OFF -DENABLE_PUSH=OFF -DENABLE_COMPRESSION=OFF
  make -j $(getconf _NPROCESSORS_ONLN)
  $SUDO make install
  echo -e "${COLOR_GREEN}prometheus CPP is installed ${COLOR_OFF}"
  popd
  touch "${WORKDIR}/prometheus_installed"
}

$SUDO apt update
PACKAGES=""
PACKAGES+=" git git-lfs" # needed to clone dependencies
PACKAGES+=" build-essential cmake" # provides compiler and other compilation tools
PACKAGES+=" autopoint gettext texinfo help2man flex libfl-dev libelf-dev" # bcc dependencies
PACKAGES+=" libnl-route-3-dev libnl-genl-3-dev" # netlink library
PACKAGES+=" uuid-dev"
PACKAGES+=" pkg-config"
PACKAGES+=" autoconf libtool m4 automake libtinfo-dev "
PACKAGES+=" libssl-dev" # needed for certificate based security
PACKAGES+=" sudo" # needed for pcn-iptables, when building docker image
PACKAGES+=" kmod" # needed for pcn-iptables, when using lsmod to unload conntrack if not needed
PACKAGES+=" jq bash-completion" # needed for polycubectl bash autocompletion
PACKAGES+=" libpcre3-dev" # needed for libyang
PACKAGES+=" libpcap-dev" # needed for packetcapture filter
PACKAGES+=" tcpreplay" # needed for packetcapture filter

if ! command -v go &> /dev/null
then
  # Go is not available, so let's add to the list of required packages
  PACKAGES+=" golang-go" # needed for polycubectl and pcn-k8s
  
  # Checking whether the major release is lower or the minor
  if  (( os_major < os_limit_major || ( os_major == os_limit_major && os_minor < os_limit_minor ) ))
  then
    $SUDO add-apt-repository ppa:longsleep/golang-backports -y || true
  fi
fi

if [ "$MODE" == "pcn-k8s" ]; then
  PACKAGES+=" curl" # needed for pcn-k8s to download a binary
  PACKAGES+=" iptables" # only for pcn-k8s
  PACKAGES+=" iproute2" # provides bridge command that is used to add entries in vxlan device
fi

# use non interactive to avoid blocking the install script
$SUDO bash -c "DEBIAN_FRONTEND=noninteractive apt-get install -yq $PACKAGES"

current_cmake_ver="$(cmake --version | head -n1 | awk '{print $NF}')"
required_cmake_ver="3.21.0"
if [ "$(printf '%s\n' "$required_cmake_ver" "$current_cmake_ver" | sort -V | head -n1)" = "$required_cmake_ver" ]; then
  echo "Current cmake version is ${current_cmake_ver}. Skip installation"
else
  echo "Current cmake version ${current_cmake_ver} is less than required version ${required_cmake_ver}"
  echo "Install new version"

  # Install last cmake version
  $SUDO bash -c "apt-get remove --purge --auto-remove cmake -y"
  $SUDO apt update
  $SUDO bash -c "apt-get install -y software-properties-common lsb-release && apt-get clean all -y"
  $SUDO wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg > /dev/null
  $SUDO apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main" -y
  $SUDO bash -c "apt-get update && apt-get install cmake -y"
fi

# Install gcc-9
$SUDO bash -c "add-apt-repository ppa:ubuntu-toolchain-r/test -y && apt-get update && apt-get install -y gcc-9 g++-9"

get_llvm
#get_flex
get_libyang
get_pistache
get_libtins
get_fmt
get_gflags
get_folly
get_bison
get_yaml_cpp
get_prometheus_cpp

# Set $GOPATH, if not already set
if [[ -z "${GOPATH}" ]]; then
  mkdir -p $HOME/go
  export GOPATH=$HOME/go
fi
