# Nuanced_Spikes - Raspberry Pi WiFi AP Configuration

We will setting up the Raspberry Pi to act as a WiFi Access Point, with the following details:

SSID - NSpike
pw   - IROSIROS

First, do the basic setup and update all the default stuff on your Raspberry Pi. This should happen if you were connected to the internet during setup, but just in case, open a terminal window and run these two commands:
```sudo apt update```
```sudo apt full-upgrade```

Next, we'll install all the packages we need to run as a WiFi Access Point:
```
sudo apt install hostapd
sudo systemctl unmask hostapd
sudo systemctl enable hostapd
sudo apt install dnsmasq
sudo DEBIAN_FRONTEND=noninteractive apt install -y netfilter-persistent iptables-persistent
```

Now we'll configure DHCP so new devices can get IP addresses
`sudo mousepad /etc/dhcpcd.conf`
(replace mousepad with nano if you're running without the desktop, or just like terminal-based text editors)

Add the following lines to the bottom
```
interface wlan0
    static ip_address=192.168.4.1/24
    nohook wpa_supplicant
```

Now we'll make an new DNS configuration
```
sudo mv /etc/dnsmasq.conf /etc/dnsmasq.conf.orig
sudo mousepad /etc/dnsmasq.conf
```
Write the following to dnsmasq.conf:
```
interface=wlan0 # Listening interface
dhcp-range=192.168.4.2,192.168.4.20,255.255.255.0,24h
                # Pool of IP addresses served via DHCP
domain=wlan     # Local wireless DNS domain
address=/gw.wlan/192.168.4.1
                # Alias for this router
```

Now we'll configure the Access Point software
```sudo mousepad /etc/hostapd/hostapd.conf```
and add the following lines at the bottom:
```
country_code=US
interface=wlan0
ssid=NSpike
hw_mode=g
channel=7
macaddr_acl=0
auth_algs=1
ignore_broadcast_ssid=0
wpa=2
wpa_passphrase=IROSIROS
wpa_key_mgmt=WPA-PSK
wpa_pairwise=TKIP
rsn_pairwise=CCMP
```

Finally, reboot and the network should show up!
