#!/bin/bash

COLOR_RED='\033[0;31m'
COLOR_GREEN='\033[0;32m'
COLOR_YELLOW='\033[0;33m'
COLOR_OFF='\033[0m' # No Color

ENABLE_ROUTER="true"
ENABLE_LBRP="true"
ENABLE_K8S_DISPATCHER="true"
USE_ALL_NODES="false"

function show_help() {
usage="$(basename "$0") -r -l -k
Morpheus-k8s start script

mode:
  -r: disable Morpheus on router service
  -l: disable Morpheus on load balancer service
  -k: disable Morpheus on k8s dispatcher service"
echo "$usage"
echo
}

while getopts rlkah option; do
 case "${option}" in
 h|\?)
  show_help
  exit 0
  ;;
 r) ENABLE_ROUTER="false"
	;;
 l) ENABLE_LBRP="false"
	;;
 k) ENABLE_K8S_DISPATCHER="false"
	;;
 a) USE_ALL_NODES="true"
	;;
 :)
  echo "Option -$OPTARG requires an argument." >&2
  show_help
  exit 0
  ;;
 esac
done

if [ $USE_ALL_NODES == "true" ]; then
    tmp_ips=$(kubectl get nodes -o wide | awk '{if (NR!=1) {print $6}}')
else
    tmp_ips=$(kubectl get nodes -o wide | grep -v 'control-plane' | awk '{if (NR!=1) {print $6}}')
fi

ips_str="${tmp_ips//$'\n'/ }"
read -a ips <<< "$ips_str"

if [ $ENABLE_LBRP == "true" ]; then
    # Start Morpheus on intLbrp
    for ip in "${ips[@]}"; do
        echo -e "${COLOR_GREEN}[ INFO ] Start Polykube-Morpheus on ilb0 (ip: ${ip}) ${COLOR_OFF}"
        POLYCUBECTL_URL=http://${ip}:9000/polycube/v1/ polycubectl ilb0 set start-morpheus=true
    done
fi

if [ $ENABLE_ROUTER == "true" ]; then
    # Start Morpheus on router
    for ip in "${ips[@]}"; do
        echo -e "${COLOR_GREEN}[ INFO ] Start Polykube-Morpheus on r0 (ip: ${ip}) ${COLOR_OFF}"
        POLYCUBECTL_URL=http://${ip}:9000/polycube/v1/ polycubectl r0 set start-morpheus=true
    done
fi 

if [ $ENABLE_LBRP == "true" ]; then
    # Start Morpheus on extLbrp
    for ip in "${ips[@]}"; do
        echo -e "${COLOR_GREEN}[ INFO ] Start Polykube-Morpheus on elb0 (ip: ${ip}) ${COLOR_OFF}"
        POLYCUBECTL_URL=http://${ip}:9000/polycube/v1/ polycubectl elb0 set start-morpheus=true
    done
fi

if [ $ENABLE_K8S_DISPATCHER == "true" ]; then
    # Start Morpheus on k8sdispatcher
    for ip in "${ips[@]}"; do
        echo -e "${COLOR_GREEN}[ INFO ] Start Polykube-Morpheus on k0 (ip: ${ip}) ${COLOR_OFF}"
        POLYCUBECTL_URL=http://${ip}:9000/polycube/v1/ polycubectl k0 set start-morpheus=true
    done
fi