#!/bin/bash

if [ $# -lt 1 ]; then
    echo "No arguments supplied"
    echo "The first argument should be the user"
    echo "[Optional] The second argument is the path where to install all the services"
    exit 1
fi

USER_HOME=$(getent passwd $1 | awk -F: '{ print $6 }')

if [ -z "${USER_HOME}" ]; then
    echo "Unknown user $1"
    exit 1
fi

DEPS_PATH="/local"
if [ ! -z "$2" ]; then
    DEPS_PATH="$2"
fi

INSTALL_K8S_SCRIPT="install-k8s-deps.sh"
INSTALL_K8S_SCRIPT_URL="https://raw.githubusercontent.com/Morpheus-compiler/polycube/morpheus-k8s/scripts/${INSTALL_K8S_SCRIPT}"

POLYCUBE_GIT_REPO="https://github.com/Morpheus-compiler/polycube.git"

BPFTOOL_GIT_REPO="https://github.com/libbpf/bpftool.git"
BPFTOOL_PATCH="bpftool-link.patch"
BPFTOOL_PATCH_URL="https://raw.githubusercontent.com/Morpheus-compiler/polycube/morpheus-k8s/src/components/k8s/cloudlab/${BPFTOOL_PATCH}"

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
    mkdir -p ${DEPS_PATH} 
    wget -nc ${INSTALL_K8S_SCRIPT_URL} -P ${DEPS_PATH}/
    chmod +x ${DEPS_PATH}/${INSTALL_K8S_SCRIPT}
    ${DEPS_PATH}/${INSTALL_K8S_SCRIPT}
    source ${USER_HOME}/.bashrc
    echo 'export PATH=$PATH:/usr/local/go/bin' >> /root/.bashrc
    export PATH=$PATH:/usr/local/go/bin
    popd
}

install_polycubectl() {
    pushd .
    git clone --branch morpheus-k8s ${POLYCUBE_GIT_REPO} ${DEPS_PATH}/polycube
    cd ${DEPS_PATH}/polycube/src/polycubectl 
    cmake . 
    $SUDO ./install.sh
    popd
}

install_bpftool() {
    pushd .
    git clone --recurse-submodules ${BPFTOOL_GIT_REPO} ${DEPS_PATH}/bpftool
    cd ${DEPS_PATH}/bpftool/src
    wget -O ${BPFTOOL_PATCH_URL}
    git apply ${BPFTOOL_PATCH}
    make
    $SUDO make install
    popd
}

install_ssh_keys() {
    pushd .

    if [[ -f "$USER_HOME/.ssh/ansible" ]]; then
        echo "ansible key file already exists"
        return
    fi

    if ! command -v geni-get &> /dev/null; then
        echo "Command geni-get not available"
        return
    fi

    # Create the user SSH directory, just in case.
    mkdir ${USER_HOME}/.ssh && chmod 700 ${USER_HOME}/.ssh

    # Retrieve the server-generated RSA private key.
    geni-get key > $USER_HOME/.ssh/ansible
    chmod 600 $USER_HOME/.ssh/ansible

    # Derive the corresponding public key portion.
    ssh-keygen -y -f $USER_HOME/.ssh/ansible > $USER_HOME/.ssh/ansible.pub

    # If you want to permit login authenticated by the auto-generated key,
    # then append the public half to the authorized_keys file:
    grep -q -f $USER_HOME/.ssh/ansible.pub $USER_HOME/.ssh/authorized_keys || cat $USER_HOME/.ssh/ansible.pub >> $USER_HOME/.ssh/authorized_keys
    popd
}

$SUDO mkdir -p ${DEPS_PATH} 
$SUDO chown -R smiano:$(id -g) ${DEPS_PATH}

install_k8s_deps

$SUDO chown -R smiano:$(id -g) ${DEPS_PATH}
$SUDO mkdir -p /var/log/polycube
$SUDO chown -R smiano:$(id -g) /var/log/polycube

install_bpftool
install_ssh_keys

if grep -q "swapoff" "${USER_HOME}/.bashrc"; then
    echo "Swap already disabled"
    exit 0
else
    echo "sudo swapoff -a" >> ${USER_HOME}/.bashrc
fi

source ${USER_HOME}/.bashrc
$SUDO systemctl daemon-reload
$SUDO systemctl restart kubelet