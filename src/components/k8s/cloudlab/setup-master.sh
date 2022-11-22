#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

function valid_ip()
{
    local  ip=$1
    local  stat=1

    if [[ $ip =~ ^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$ ]]; then
        OIFS=$IFS
        IFS='.'
        ip=($ip)
        IFS=$OIFS
        [[ ${ip[0]} -le 255 && ${ip[1]} -le 255 \
            && ${ip[2]} -le 255 && ${ip[3]} -le 255 ]]
        stat=$?
    fi
    return $stat
}

if [ $# -lt 3 ]; then
    echo "No arguments supplied"
    echo "The first argument should be the user"
    echo "The second argument should be the number of worker nodes"
    echo "The third argument should be the address of the master"
    echo "[Optional] The forth argument is the path where to install all the services"
    exit 1
fi

re='^[0-9]+$'
if ! [[ $2 =~ $re ]] ; then
   echo "error: First argument is not a number" >&2
   exit 1
fi

ip_addr=$3

if valid_ip $ip_addr; then
    echo "IP is valid"
else 
    echo "Not a valid IP ${ip_addr}"
    exit 1
fi

USER_HOME=$(getent passwd $1 | awk -F: '{ print $6 }')

if [ -z "${USER_HOME}" ]; then
    echo "Unknown user $1"
    exit 1
fi

DEPS_PATH="/local"
if [ ! -z "$4" ]; then
    DEPS_PATH="$4"
fi

INSTALL_K8S_SCRIPT="install-k8s-deps.sh"
INSTALL_K8S_SCRIPT_URL="https://raw.githubusercontent.com/Morpheus-compiler/polycube/morpheus-k8s/scripts/${INSTALL_K8S_SCRIPT}"

POLYKUBE_GIT_REPO="https://github.com/polycube-network/polykube.git"
POLYKUBE_PATCH_URL="https://raw.githubusercontent.com/Morpheus-compiler/polycube/morpheus-k8s/src/components/k8s/polykube-cni/polykube-cni.patch"

POLYCUBE_GIT_REPO="https://github.com/Morpheus-compiler/polycube.git"

K8S_BENCH_SUITE_GIT_REPO="https://github.com/sebymiano/k8s-bench-suite.git"

BPFTOOL_GIT_REPO="https://github.com/libbpf/bpftool.git"
BPFTOOL_PATCH="bpftool-link.patch"
BPFTOOL_PATCH_URL="https://raw.githubusercontent.com/Morpheus-compiler/polycube/morpheus-k8s/src/components/k8s/cloudlab/${BPFTOOL_PATCH}"

ANSIBLE_MASTER_URL="https://raw.githubusercontent.com/Morpheus-compiler/polycube/morpheus-k8s/src/components/k8s/cloudlab/master.yaml"
ANSIBLE_WORKERS_URL="https://raw.githubusercontent.com/Morpheus-compiler/polycube/morpheus-k8s/src/components/k8s/cloudlab/workers.yaml"
ANSIBLE_USER="smiano"

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
    wget -nc ${INSTALL_K8S_SCRIPT_URL} -P ${DEPS_PATH}/
    chmod +x ${DEPS_PATH}/${INSTALL_K8S_SCRIPT}
    ${DEPS_PATH}/${INSTALL_K8S_SCRIPT}
    source $USER_HOME/.profile
    echo 'export PATH=$PATH:/usr/local/go/bin' >> $HOME/.profile
    export PATH=$PATH:/usr/local/go/bin
    popd
}

install_polykube_cni() {
    pushd .
    git clone ${POLYKUBE_GIT_REPO} ${DEPS_PATH}/polykube
    wget -nc ${POLYKUBE_PATCH_URL} -P ${DEPS_PATH}/polykube/
    cd ${DEPS_PATH}/polykube 
    git apply polykube-cni.patch
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

install_k8s_bench_suite() {
    pushd .
    git clone --branch morpheus-k8s ${K8S_BENCH_SUITE_GIT_REPO} ${DEPS_PATH}/k8s-bench-suite
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
    mkdir $USER_HOME/.ssh && chmod 700 $USER_HOME/.ssh

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

nextip(){
    IP=$1
    IP_HEX=$(printf '%.2X%.2X%.2X%.2X\n' `echo $IP | sed -e 's/\./ /g'`)
    NEXT_IP_HEX=$(printf %.8X `echo $(( 0x$IP_HEX + 1 ))`)
    NEXT_IP=$(printf '%d.%d.%d.%d\n' `echo $NEXT_IP_HEX | sed -r 's/(..)/0x\1 /g'`)
    echo "$NEXT_IP"
}

generate_ansible_host_file() {
    local num_nodes=$1
    local ip_master=$2

    num_nodes=$((num_nodes-1))

    pushd .
    mkdir -p ${DEPS_PATH}/kube-cluster
    cd ${DEPS_PATH}/kube-cluster

    if [[ -f "${DEPS_PATH}/kube-cluster/hosts" ]]; then
        echo "The host file already exists"
        return
    fi

    echo "[masters]" > ${DEPS_PATH}/kube-cluster/hosts
    echo "master ansible_host=${ip_master} ansible_user=${ANSIBLE_USER}" >> ${DEPS_PATH}/kube-cluster/hosts

    if [ $num_nodes -gt 0 ]; then
        echo "" >> ${DEPS_PATH}/kube-cluster/hosts
        echo "[workers]" >> ${DEPS_PATH}/kube-cluster/hosts
        IP=$ip_master
        for i in $(seq 1 $num_nodes); do
            IP=$(nextip $IP)
            echo $IP
            echo "worker${i} ansible_host=${IP} ansible_user=${ANSIBLE_USER}" >> ${DEPS_PATH}/kube-cluster/hosts
        done
    fi

    echo "" >> ${DEPS_PATH}/kube-cluster/hosts
    echo "[all:vars]" >> ${DEPS_PATH}/kube-cluster/hosts
    echo "ansible_python_interpreter=/usr/bin/python3" >> ${DEPS_PATH}/kube-cluster/hosts
    echo "ansible_ssh_common_args='-o StrictHostKeyChecking=no'" >> ${DEPS_PATH}/kube-cluster/hosts
    echo "ansible_ssh_private_key_file=$USER_HOME/.ssh/ansible" >> ${DEPS_PATH}/kube-cluster/hosts
    popd 
}

$SUDO mkdir -p ${DEPS_PATH} 
$SUDO chown -R smiano:$(id -g) ${DEPS_PATH}
$SUDO mkdir -p /var/log/polycube
$SUDO chown -R smiano:$(id -g) /var/log/polycube

install_k8s_deps
install_polykube_cni
install_polycubectl
install_k8s_bench_suite

$SUDO cp ${DEPS_PATH}/polycube/src/components/k8s/polykube-cni/enable-morpheus-all-nodes.sh /usr/local/bin/enable-morpheus-all-nodes

install_bpftool
install_ssh_keys

ip_addr=$3

generate_ansible_host_file $2 $ip_addr

if grep -q "swapoff" "$USER_HOME/.profile"; then
    echo "Swap already disabled"
    exit 0
else
    echo "sudo swapoff -a" >> $USER_HOME/.profile
fi

source $USER_HOME/.profile
$SUDO systemctl daemon-reload
$SUDO systemctl restart kubelet