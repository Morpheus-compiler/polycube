#!/bin/bash

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

if [ $# -ne 2 ]; then
    echo "No arguments supplied"
    echo "The first argument should be the number of worker nodes"
    echo "The second argument should be the address of the master"
    exit 1
fi

re='^[0-9]+$'
if ! [[ $1 =~ $re ]] ; then
   echo "error: First argument is not a number" >&2
   exit 1
fi

ip_addr=$2

if valid_ip $ip_addr; then
    echo "IP is valid"
else 
    echo "Not a valid IP ${ip_addr}"
    exit 1
fi


INSTALL_K8S_SCRIPT="install-k8s-deps.sh"
INSTALL_K8S_SCRIPT_URL="https://raw.githubusercontent.com/Morpheus-compiler/polycube/morpheus-k8s/scripts/${INSATLL_K8S_SCRIPT}"

POLYKUBE_GIT_REPO="https://github.com/polycube-network/polykube.git"
POLYKUBE_PATCH_URL="https://raw.githubusercontent.com/Morpheus-compiler/polycube/morpheus-k8s/src/components/k8s/polykube-cni/polykube-cni.patch"

POLYCUBE_GIT_REPO="https://github.com/Morpheus-compiler/polycube.git"

K8S_BENCH_SUITE_GIT_REPO="https://github.com/sebymiano/k8s-bench-suite.git"

BPFTOOL_GIT_REPO="https://github.com/libbpf/bpftool.git"

ANSIBLE_MASTER_URL="https://raw.githubusercontent.com/Morpheus-compiler/polycube/morpheus-k8s/src/components/k8s/cloudlab/master.yaml"
ANSIBLE_WORKERS_URL="https://raw.githubusercontent.com/Morpheus-compiler/polycube/morpheus-k8s/src/components/k8s/cloudlab/workers.yaml"
ANSIBLE_USER="smiano"

[ -z ${SUDO+x} ] && SUDO='sudo'

PACKAGES="git cmake"
PACKAGES+=" linux-generic-hwe-20.04"
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

install_polykube_cni() {
    pushd .
    git clone ${POLYKUBE_GIT_REPO} /local/polykube
    wget -nc ${POLYKUBE_PATCH_URL} -P /local/polykube/
    cd /local/polykube 
    git apply polykube-cni.patch
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

install_k8s_bench_suite() {
    pushd .
    git clone --branch morpheus-k8s ${K8S_BENCH_SUITE_GIT_REPO} /local/k8s-bench-suite
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

    if [[ -f "$HOME/.ssh/id_rsa" ]]; then
        echo "id_rsa file already exists"
        return
    fi
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
    mkdir -p /local/kube-cluster
    cd /local/kube-cluster

    if [[ -f "/local/kube-cluster/hosts" ]]; then
        echo "The host file already exists"
        return
    fi

    echo "[masters]" > /local/kube-cluster/hosts
    echo "master ansible_host=${ip_master} ansible_user=${ANSIBLE_USER}" >> /local/kube-cluster/hosts

    if [ $num_nodes -gt 0 ]; then
        echo "" >> /local/kube-cluster/hosts
        echo "[workers]" >> /local/kube-cluster/hosts
        IP=$ip_master
        for i in $(seq 1 $num_nodes); do
            IP=$(nextip $IP)
            echo $IP
            echo "worker${i} ansible_host=${IP} ansible_user=${ANSIBLE_USER}" >> /local/kube-cluster/hosts
        done
    fi

    echo "" >> /local/kube-cluster/hosts
    echo "[all:vars]" >> /local/kube-cluster/hosts
    echo "ansible_python_interpreter=/usr/bin/python3" >> /local/kube-cluster/hosts
    popd 
}

install_k8s_deps
install_polykube_cni
install_polycubectl
install_k8s_bench_suite

$SUDO chown -R smiano:$(id -g) /local
mkdir -p /var/log/polycube
$SUDO chown -R smiano:$(id -g) /var/log/polycube
$SUDO cp /local/polycube/src/components/k8s/polykube-cni/enable-morpheus-all-nodes.sh /usr/local/bin/enable-morpheus-all-nodes

install_bpftool
install_ssh_keys

ip_addr=$2

generate_ansible_host_file $1 $ip_addr
