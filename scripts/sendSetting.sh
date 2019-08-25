
pass=$1
login=$2
ip=$3
file="../ssh_server_client.txt"

sshpass -p $pass scp $file $login@$ip:/root/
