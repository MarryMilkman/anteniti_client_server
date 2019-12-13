cd /home/user/kinkan
make package/server_client/clean
make package/server_client/compile

echo "Bild seccess"


sshpass -p '11111111' scp bin/packages/mipsel_24kc/base/server_client_123-321_mipsel_24kc.ipk root@192.168.1.1:/root/server_client/

echo "Send seccess"

sshpass -p '11111111' ssh root@192.168.1.1 opkg remove server_client
sshpass -p '11111111' ssh root@192.168.1.1 opkg install /root/server_client/server_client_123-321_mipsel_24kc.ipk

echo "Bild in router (ipk) seccess"

cd -
sshpass -p '11111111' ssh root@192.168.1.1
