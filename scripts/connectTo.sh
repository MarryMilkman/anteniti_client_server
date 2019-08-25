
echo $1
echo $2

pass=$1
login="root"
host=$2

sshpass    -p $pass ssh -t $login@$host "echo 'hello'"
