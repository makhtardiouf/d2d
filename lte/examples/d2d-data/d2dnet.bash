#!/bin/bash

echo "Run as root to configure Tun/Tap virtual netdevices..."

# Reset
tunctl -d veth0
tunctl -d veth1
tunctl -d veth2

# Create virtual net devices

  tunctl -u makhtar -g makhtar -t veth0
  tunctl -u makhtar -g makhtar -t veth1
  tunctl -u makhtar -g makhtar -t veth2
  
 # Bridge eth0 with veth0
  
  # add bridge
  #brctl addbr br0
  
  # add interface to bridge
  #brctl addif br0 eth0
  
  # Set bridge ip to a real IP in your local network 
  # ifconfig br0 203.255.56.53 netmask 255.255.255.0 promisc up

  # Set ip and promiscuous mode
  # 0.0.0.0/8 = Current network (only valid as source address)
  # ifconfig veth0 0.0.0.0/8 promisc up
  
  ifconfig veth0 hw ether 00:00:00:59:00:aa  # SGW 00:00:00:59:00:aa
  ifconfig veth1 hw ether 00:00:00:eb:00     # eNB  00:00:00:eb:00
  ifconfig veth2 hw ether 00:00:00:00:00:03  # ProSeFcn
  
  ifconfig veth0 inet6 add 2001::a00:0/64 promisc multicast up
  ifconfig veth0 inet6 add fe80::200:ff:fe59:aa
  
  ifconfig veth1 inet6 add 2001::c00:0/64 promisc multicast up  
  ifconfig veth1 inet6 add fe80::200:ff:feeb:2
  
  ifconfig veth2 inet6 add 2001:800::/64 promisc multicast up 
  ifconfig veth2 inet6 add fe80::200:ff:fe00:3

  #brctl addif br0 veth0
  
echo "done."
