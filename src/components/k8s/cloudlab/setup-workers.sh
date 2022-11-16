#!/bin/bash

if [ $# -ne 1 ]; then
    echo "No arguments supplied"
    echo "The first argument should be the user"
    exit 1
fi

USER_HOME=$(getent passwd $1 | awk -F: '{ print $6 }')

if [ -z "${USER_HOME}" ]; then
    echo "Unknown user $1"
    exit 1
fi

INSTALL_K8S_SCRIPT="install-k8s-deps.sh"
INSTALL_K8S_SCRIPT_URL="https://raw.githubusercontent.com/Morpheus-compiler/polycube/morpheus-k8s/scripts/${INSTALL_K8S_SCRIPT}"

POLYCUBE_GIT_REPO="https://github.com/Morpheus-compiler/polycube.git"

BPFTOOL_GIT_REPO="https://github.com/libbpf/bpftool.git"

[ -z ${SUDO+x} ] && SUDO='sudo'

PACKAGES="git cmake"
PACKAGES+=" linux-generic-hwe-20.04"
PACKAGES+=" libelf-dev zlib1g-dev libbfb0-dev libcap-dev libbfd-dev"
PACKAGES+=" clang llvm llvm-dev"
PACKAGES+=" software-properties-common ansible"

$SUDO apt update 
$SUDO apt upgrade -y

$SUDO bash -c "DEBIAN_FRONTEND=noninteractive apt-get install -yq $PACKAGES"

install_k8s_deps() {
    pushd .
    mkdir -p /local 
    wget -nc ${INSTALL_K8S_SCRIPT_URL} -P /local/
    chmod +x /local/${INSTALL_K8S_SCRIPT}
    /local/${INSTALL_K8S_SCRIPT}
    source ${USER_HOME}/.profile
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
    git clone --recurse-submodules ${BPFTOOL_GIT_REPO} /local/bpftool
    cd bpftool/src
    make
    $SUDO make install
    popd
}

install_ssh_keys() {
    pushd .

    if [[ -f "${USER_HOME}/.ssh/id_rsa" ]]; then
        echo "id_rsa file already exists"
        return
    fi

    # Create the user SSH directory, just in case.
    mkdir ${USER_HOME}/.ssh && chmod 700 ${USER_HOME}/.ssh

    # Retrieve the server-generated RSA private key.
    geni-get key > ${USER_HOME}/.ssh/id_rsa
    chmod 600 ${USER_HOME}/.ssh/id_rsa

    # Derive the corresponding public key portion.
    ssh-keygen -y -f ${USER_HOME}/.ssh/id_rsa > ${USER_HOME}/.ssh/id_rsa.pub

    # If you want to permit login authenticated by the auto-generated key,
    # then append the public half to the authorized_keys file:
    grep -q -f ${USER_HOME}/.ssh/id_rsa.pub ${USER_HOME}/.ssh/authorized_keys || cat ${USER_HOME}/.ssh/id_rsa.pub >> ${USER_HOME}/.ssh/authorized_keys
    popd
}

$SUDO mkdir -p /local 
$SUDO chown -R smiano:$(id -g) /local

install_k8s_deps

$SUDO chown -R smiano:$(id -g) /local
mkdir -p /var/log/polycube
$SUDO chown -R smiano:$(id -g) /var/log/polycube

install_bpftool
install_ssh_keys

if grep -q "swapoff" "${USER_HOME}/.profile"; then
    echo "Swap already disabled"
    exit 0
else
    echo "sudo swapoff -a" >> ${USER_HOME}/.profile
fi

source ${USER_HOME}/.profile
$SUDO systemctl daemon-reload
$SUDO systemctl restart kubelet