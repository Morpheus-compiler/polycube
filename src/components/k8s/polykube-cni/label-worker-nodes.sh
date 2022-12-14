#!/bin/bash

tmp_nodes=$(kubectl get nodes -o wide | grep -v 'control-plane' | awk '{if (NR!=1) {print $1}}')

nodes_str="${tmp_nodes//$'\n'/ }"
read -a nodes <<< "$nodes_str"

for node in "${nodes[@]}"; do
    kubectl label node ${node} node-role.kubernetes.io/worker=worker
done