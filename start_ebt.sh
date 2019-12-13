/etc/init.d/firewall stop
iptables -A FORWARD -t filter -s 192.168.1.0/24 -j ACCEPT
iptables -A FORWARD -t filter -d 192.168.1.0/24 -j ACCEPT
