# VECTORBOOK

https://vectorbook.com

## System Requirements

We will support installation requests for any user using the following system:

* Ubuntu 18.04 (LTS) x64
* nothing else has previously been installed on the system
* 4 GB of RAM
* ability to call `sudo`

Other set-up parameters are possible but not supported.

## System Overview

This codebase contains two main components:

* HTTP serving architecture - to serve JSON resposnes for the application
* Machine Learning architecture - to learn to please users

## Installation
Run the following commands in a fresh Ubuntu 18.04 install with root access:

```
cd ~
git clone https://github.com/propaganda-gold/vectorbook.git
cd ~/vectorbook
./install/run_sequence.sh
```

## Running the Rest Server

To run the debug-friendly REST server, run:

```
./run/debug_server.sh
```

