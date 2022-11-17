![Polycube](Documentation/images/polycube-logo.png )

# Polycube

[![Polycube Build](https://github.com/Morpheus-compiler/polycube/actions/workflows/polycube-test.yml/badge.svg)](https://github.com/Morpheus-compiler/polycube/actions/workflows/polycube-test.yml)
[![License](https://img.shields.io/badge/License-Apache%202.0-blue.svg)](http://www.apache.org/licenses/LICENSE-2.0)

``Polycube`` is an **open source** software framework that provides **fast** and **lightweight** **network functions** such as bridges, routers, firewalls, and others.

Polycube services, called `cubes`, can be composed to build arbitrary **service chains** and provide custom network connectivity to **namespaces**, **containers**, **virtual machines**, and **physical hosts**.

For more information, jump to the project [Documentation](https://polycube-network.readthedocs.io/en/latest/).


## Quick links
 - [Introduction to Polycube](https://polycube-network.readthedocs.io/en/latest/intro.html)
 - [Quickstart](https://polycube-network.readthedocs.io/en/latest/quickstart.html)
 - [Documentation](https://polycube-network.readthedocs.io/en/latest/)
 - [pcn-k8s - The CNI network plugin for Kubernetes](https://polycube-network.readthedocs.io/en/latest/components/k8s/pcn-kubernetes.html)
 - [pcn-iptables - A clone of Iptables based on eBPF](https://polycube-network.readthedocs.io/en/latest/components/iptables/pcn-iptables.html)

## Create Morpheus-k8s default container
You can use the following command to build the container.

```
DOCKER_BUILDKIT=1 docker build --no-cache --build-arg DEFAULT_MODE=morpheus-k8s --build-arg DEFAULT_CLONE_MODE=git -t morpheuscompiler/morpheus-polycube-k8s:latest .
```

## Licence
Polycube is licensed under the Apache License, Version 2.0 (ALv2).

