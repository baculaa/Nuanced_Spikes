#Installing OpenCV and getting cameras working

We're going to make sure our pip installation is up-to-date, then install and check openCV. Open the terminal, and type in the following command:

```pip install --upgrade pip```
```pip install opencv-python```
```pip install paho-mqtt```


To check that the installation worked, open

```python```
```import cv2```'
```cv2.__version__```

Now we install the MQTT broker, Mosquitto

```sudo apt install -y mosquitto mosquitto-clients```

add the following lines to the mosquitto broker config

```sudo mousepad /etc/mosquitto/mosquitto.conf```
```
listener 1883
allow_anonymous true
```
set the broker to run on startup

```sudo systemctl enable mosquitto.service```
