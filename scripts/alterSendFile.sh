
echo $1
echo $2

pass=$1
login="root"
host=$2
file="./test"
text=echo `cat $file`

echo $text;

# sshpass    -p $pass ssh -t $login@$host $text
