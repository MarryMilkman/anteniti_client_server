#!/bin/sh

#Usage ./ebt_main.sh C0:38:96:98:6C:3B 192.168.1.2 192.168.1.0 L 192.168.100.133 A (or D to delete)

#External for CLI
MAC="$1"     #Client MAC
IP="$2"      #Destination client IP
NET="$3"     #Network
FLAG="$4"    #Network L, G, S, C (Local, Guest, Smart, Custom)
SELFIP="$5"  #Client IP
ACT="-$6"    #"A" to Add rule or "D" to Delete rule


#Internal (Depends to External)
GWRULE="`ebtables -L|grep "IPv4 -s $MAC --ip-dst 192.168.1.1 -j ACCEPT"`"
NETRULE="`ebtables -L|grep "IPv4 -s $MAC --ip-dst $NET/24 -j DROP"`"
MACIP="`ebtables -L|grep "IPv4 -s ! $MAC --ip-src $SELFIP -j DROP"`"

#ebtables -A INPUT -p IPv4 -s C0:38:96:98:6C:3B --ip-dst 192.168.1.151 -j ACCEPT

#Main Net
if [ "$ACT" = "-A" ]

then

    if [ "$4" = "L" ]

    then
        ipset -N deflan nethash
        ipset -A deflan $SELFIP
        ipset --create deflan_eb macipmap --network $NET/24
        ipset --add deflan_eb $SELFIP,$MAC
        iptables -D  POSTROUTING -t nat -o eth1 -m set --match-set deflan src -j MASQUERADE
        iptables -A  POSTROUTING -t nat -o eth1 -m set --match-set deflan src -j MASQUERADE
    fi

else

if [ "$ACT" = "-D" ]

then

    if [ "$4" = "L" ]

    then
        ipset -D deflan $SELFIP
        ipset --del deflan_eb $SELFIP,$MAC
    fi

fi

fi
#End section

#Guest Net
if [ "$ACT" = "-A" ]

then

    if [ "$4" = "G" ]

    then
        ipset -N guestlan nethash
        ipset -A guestlan $SELFIP
        ipset --create guestlan_eb macipmap --network $NET/24
        ipset --add guestlan_eb $SELFIP,$MAC
        iptables -D  POSTROUTING -t nat -o eth1 -m set --match-set guestlan src -j MASQUERADE
        iptables -A  POSTROUTING -t nat -o eth1 -m set --match-set guestlan src -j MASQUERADE
    fi

else

if [ "$ACT" = "-D" ]

then

    if [ "$4" = "G" ]

    then
        ipset -D guestlan $SELFIP
        ipset --del guestlan_eb $SELFIP,$MAC
    fi

fi

fi
#end section
#Smart Net
if [ "$ACT" = "-A" ]

then

    if [ "$4" = "S" ]

    then
        ipset -N smartlan nethash
        ipset -A smartlan $SELFIP
        ipset --create smartlan_eb macipmap --network $NET/24
        ipset --add smartlan_eb $SELFIP,$MAC
        iptables -D  POSTROUTING -t nat -o eth1 -m set --match-set smartlan src -j MASQUERADE
        iptables -A  POSTROUTING -t nat -o eth1 -m set --match-set smartlan src -j MASQUERADE
    fi

else

if [ "$ACT" = "-D" ]

then

    if [ "$4" = "S" ]

    then
        ipset -D smartlan $SELFIP
        ipset --del smartlan_eb $SELFIP,$MAC
    fi

fi

fi
#End section

#MAC & IP

if [ "$ACT" = "-A" ]

then

    if [ "`echo $MACIP`" == "`ebtables -L|grep "IPv4 -s ! $MAC --ip-src $SELFIP -j DROP"`" ]

    then

	ebtables $ACT INPUT -p IPv4 --ip-src $SELFIP -s '!' $MAC -j DROP

    echo "Mac Ip rule add"

    fi

else

if [ "$ACT" = "-D" ]

    then

	ebtables $ACT INPUT -p IPv4 --ip-src $SELFIP -s '!' $MAC -j DROP

    echo "Mac Ip rule del"

    fi
fi
#End section

#Rule for acces to Master and Satellites

if [ "$ACT" = "-A" ]

then

    if [ "`echo $GWRULE`" == "`ebtables -L|grep "IPv4 -s $MAC --ip-dst 192.168.1.1 -j ACCEPT"`" ]

    then
        ebtables -D INPUT -p IPv4 -s $MAC --ip-dst $NET/24 -j DROP
        ebtables $ACT INPUT -p IPv4 -s $MAC --ip-dst 192.168.1.1 -j ACCEPT
        ebtables $ACT INPUT -p IPv4 -s $MAC --ip-dst 192.168.1.2 -j ACCEPT
        ebtables $ACT INPUT -p IPv4 -s $MAC --ip-dst 192.168.1.3 -j ACCEPT
        ebtables $ACT INPUT -p IPv4 -s $MAC --ip-dst 192.168.1.4 -j ACCEPT
        ebtables $ACT INPUT -p IPv4 -s $MAC --ip-dst 192.168.1.5 -j ACCEPT
        ebtables -A INPUT -p IPv4 -s $MAC --ip-dst $NET/24 -j DROP
    echo "Master add"

    fi

else
    
if [ "$ACT" = "-D" ]

	then
        ebtables -D INPUT -p IPv4 -s $MAC --ip-dst $NET/24 -j DROP
        ebtables $ACT INPUT -p IPv4 -s $MAC --ip-dst 192.168.1.1 -j ACCEPT
	ebtables $ACT INPUT -p IPv4 -s $MAC --ip-dst 192.168.1.2 -j ACCEPT
        ebtables $ACT INPUT -p IPv4 -s $MAC --ip-dst 192.168.1.3 -j ACCEPT
        ebtables $ACT INPUT -p IPv4 -s $MAC --ip-dst 192.168.1.4 -j ACCEPT
        ebtables $ACT INPUT -p IPv4 -s $MAC --ip-dst 192.168.1.5 -j ACCEPT
        ebtables -A INPUT -p IPv4 -s $MAC --ip-dst $NET/24 -j DROP
        echo "Master dell"

    fi
fi

#End section


echo $MAC $IP $NET/24 $FLAG $SELFIP $NETRULE
