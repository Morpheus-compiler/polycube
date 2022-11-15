#!/bin/bash

INSTALL_K8S_SCRIPT="install-k8s-deps.sh"
INSTALL_K8S_SCRIPT_URL="https://raw.githubusercontent.com/Morpheus-compiler/polycube/morpheus-k8s/scripts/${INSATLL_K8S_SCRIPT}"

POLYCUBE_GIT_REPO="https://github.com/Morpheus-compiler/polycube.git"

BPFTOOL_GIT_REPO="https://github.com/libbpf/bpftool.git"

[ -z ${SUDO+x} ] && SUDO='sudo'

PACKAGES="git cmake"
PACKAGES+="linux-generic-hwe-20.04"
PACKAGES+=" libelf zlib libbfd libcap"
PACKAGES+=" clang llvm llvm-dev"
PACKAGES+=" software-properties-common ansible"

$SUDO apt update 

$SUDO bash -c "DEBIAN_FRONTEND=noninteractive apt-get install -yq $PACKAGES"

install_k8s_deps() {
    pushd .
    mkdir -p /local 
    wget -nc ${INSTALL_K8S_SCRIPT_URL} -P /local/
    chmod +x /local/${INSTALL_K8S_SCRIPT}
    /local/${INSTALL_K8S_SCRIPT}
    popd
}

install_polycubectl() {
    pushd .
    git clone --branch morpheus-k8s ${POLYCUBE_GIT_REPO} /local/polycube
    cd /local/polycube/src/polycubectl 
    cmake . 
    $SUDO ./install.sh
    popd
}

install_bpftool() {
    pushd .
    git clone --recurse-submodules ${BPFTOOL_GIT_REPO} /local/
    cd bpftool/src
    make
    $SUDO make install
    popd
}

install_ssh_keys() {
    pushd .
    # Create the user SSH directory, just in case.
    mkdir $HOME/.ssh && chmod 700 $HOME/.ssh

    # Retrieve the server-generated RSA private key.
    geni-get key > $HOME/.ssh/id_rsa
    chmod 600 $HOME/.ssh/id_rsa

    # Derive the corresponding public key portion.
    ssh-keygen -y -f $HOME/.ssh/id_rsa > $HOME/.ssh/id_rsa.pub

    # If you want to permit login authenticated by the auto-generated key,
    # then append the public half to the authorized_keys file:
    grep -q -f $HOME/.ssh/id_rsa.pub $HOME/.ssh/authorized_keys || cat $HOME/.ssh/id_rsa.pub >> $HOME/.ssh/authorized_keys
    popd
}

install_k8s_deps

$SUDO chown -R smiano:$(id -g) /local
mkdir -p /var/log/polycube
$SUDO chown -R smiano:$(id -g) /var/log/polycube

install_bpftool
install_ssh_keys