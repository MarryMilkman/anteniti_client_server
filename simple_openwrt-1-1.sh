cd /home/user/zborki_for_openwrt/openwrt-ad7b64e
make package/server_client/clean
make package/server_client/compile

echo "Bild seccess"



sshpass -p '11111111' scp bin/packages/arm_cortex-a7_neon-vfpv4/base/server_client_123-321_arm_cortex-a7_neon-vfpv4.ipk root@192.168.1.3:/root/server_client/

echo "Send seccess"

sshpass -p '11111111' ssh root@192.168.1.3 opkg remove server_client
sshpass -p '11111111' ssh root@192.168.1.3 opkg install /root/server_client/server_client_123-321_arm_cortex-a7_neon-vfpv4.ipk

echo "Bild in router (ipk) seccess"

cd -
sshpass -p '11111111' ssh root@192.168.1.3
