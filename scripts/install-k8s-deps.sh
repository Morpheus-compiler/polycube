#!/bin/bash

COLOR_RED='\033[0;31m'
COLOR_GREEN='\033[0;32m'
COLOR_YELLOW='\033[0;33m'
COLOR_OFF='\033[0m' # No Color

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

echo -e "${COLOR_YELLOW}[ INFO ] Make sure you have installed all Polycube dependencies first ${COLOR_OFF}"

[ -z ${SUDO+x} ] && SUDO='sudo'

# Release in which has been natively introduced support for golang-go (Ubuntu >= 20.04)
os_limit_major="20"
os_limit_minor="04"
read -r os_major os_minor<<<$(grep -Po 'VERSION_ID="\K.*?(?=")' /etc/os-release | sed 's/\./ /g')

$SUDO apt update

install_kubeadm() {
  echo -e "${COLOR_GREE}[ INFO ] Installing Kubeadm ${COLOR_OFF}"
  $SUDO apt install -y apt-transport-https ca-certificates curl
  $SUDO curl -fsSLo /usr/share/keyrings/kubernetes-archive-keyring.gpg https://packages.cloud.google.com/apt/doc/apt-key.gpg
  echo "deb [signed-by=/usr/share/keyrings/kubernetes-archive-keyring.gpg] https://apt.kubernetes.io/ kubernetes-xenial main" | $SUDO tee /etc/apt/sources.list.d/kubernetes.list
  $SUDO apt update
  $SUDO apt install -y kubelet kubeadm kubectl
  $SUDO apt-mark hold kubelet kubeadm kubectl
}

install_docker() {
    echo -e "${COLOR_GREE}[ INFO ] Installing Docker runtime ${COLOR_OFF}"
    $SUDO apt remove -y docker docker-engine docker.io containerd runc
    $SUDO rm -rf /etc/apt/keyrings/docker.gpg
    
    $SUDO apt update
    $SUDO apt install -y ca-certificates curl gnupg lsb-release

    $SUDO mkdir -p /etc/apt/keyrings
    curl -fsSL https://download.docker.com/linux/ubuntu/gpg | $SUDO gpg --dearmor -o /etc/apt/keyrings/docker.gpg
    echo "deb [arch=$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.gpg] https://download.docker.com/linux/ubuntu \
  $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
    $SUDO apt update
    $SUDO apt install -y docker-ce docker-ce-cli containerd.io docker-compose-plugin
    $SUDO service docker start
    $SUDO rm /etc/containerd/config.toml
    $SUDO systemctl restart containerd
}

install_golang() {
  if ! command -v go &> /dev/null
  then
    # Go is not available, so let's add to the list of required packages
    PACKAGES+="golang-go" # needed for polycubectl and pcn-k8s
    
    # Checking whether the major release is lower or the minor
    if  (( os_major < os_limit_major || ( os_major == os_limit_major && os_minor < os_limit_minor ) ))
    then
      $SUDO add-apt-repository ppa:longsleep/golang-backports -y || true
    fi

    $SUDO apt update
    $SUDO bash -c "DEBIAN_FRONTEND=noninteractive apt-get install -yq $PACKAGES"
fi
}

install_golang_latest() {
  pushd .
  rm -rf /tmp/golang &> /dev/null
  mkdir -p /tmp/golang
  cd /tmp/golang
  wget https://go.dev/dl/go1.19.2.linux-amd64.tar.gz
  $SUDO rm -rf /usr/local/go &> /dev/null
  $SUDO tar -C /usr/local -xzf go1.19.2.linux-amd64.tar.gz

  if [[ ":$PATH:" == *":/usr/local/go/bin:"* ]]; then
    return
  else
    echo 'export PATH=$PATH:/usr/local/go/bin' >> $HOME/.bashrc
    export PATH=$PATH:/usr/local/go/bin
  fi
}

install_helm() {
  if ! command -v helm &> /dev/null; then
    wget https://get.helm.sh/helm-v3.10.0-linux-amd64.tar.gz -P /tmp/helm

    pushd .
    cd /tmp/helm
    tar -zxvf helm-v3.10.0-linux-amd64.tar.gz -C /tmp/helm --strip-components 1
    $SUDO mv helm /usr/local/bin/helm
    popd

    $SUDO rm -rf /tmp/helm
  fi
}

$SUDO apt update && $SUDO apt upgrade -y
$SUDO apt update && $SUDO apt install jq -y

install_kubeadm
install_docker
install_golang
# install_golang_latest
install_helm

$SUDO swapoff -a

# Set $GOPATH, if not already set
if [[ -z "${GOPATH}" ]]; then
  mkdir -p $HOME/go
  export GOPATH=$HOME/go
  echo 'export GOPATH=$HOME/go' >> $HOME/.bashrc
fi

echo -e "${COLOR_GREE}[ INFO ] Installation completed, now you can run your k8s cluster ${COLOR_OFF}"
