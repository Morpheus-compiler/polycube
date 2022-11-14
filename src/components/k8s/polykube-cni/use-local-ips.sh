#!/bin/bash

KUBEADM_FILE=/var/lib/kubelet/kubeadm-flags.env

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

if [ $# -eq 0 ]; then
    echo "No arguments supplied"
    exit 1
fi

ip_addr=$1

if valid_ip $ip_addr; then 
    if grep -q "node-ip" "$KUBEADM_FILE"; then
        echo "Node ip already set"
        exit 0
    fi
    sudo sed -i 's/\(KUBELET_KUBEADM_ARGS=\)"\(.*\)"/\1"\2 --node-ip='${ip_addr}'"/' ${KUBEADM_FILE}
    sudo systemctl daemon-reload
    sudo systemctl restart kubelet
else 
    echo "Not a valid IP ${ip_addr}"
    exit 1
fi

