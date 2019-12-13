cd /home/user/kinkan
make package/server_client/compile

echo "Bild seccess"

scp bin/packages/mipsel_24kc/base/server_client_123-321_mipsel_24kc.ipk root@192.168.1.1:/root/server_client/

echo "Send seccess"

ssh root@192.168.1.1 opkg remove server_client
ssh root@192.168.1.1 opkg install /root/server_client/server_client_123-321_mipsel_24kc.ipk

echo "Bild in router (ipk) seccess"

cd -
ssh root@192.168.1.1
