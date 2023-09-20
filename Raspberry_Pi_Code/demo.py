# This file is part of OpenCV Zoo project.
# It is subject to the license terms in the LICENSE file found in the same directory.
#
# Copyright (C) 2021, Shenzhen Institute of Artificial Intelligence and Robotics for Society, all rights reserved.
# Third party copyrights are property of their respective owners.

import argparse

import numpy as np
import cv2 as cv
import glob
import time
import paho.mqtt.client as mqtt
import paho.mqtt.publish as MQTTpublisher

MQTTServerIP = "127.0.0.1"
MQTTPort = 1883

CaptureWidth = 800
CaptureHeight = 480

#cutoff, in pixels, for how big a face can be before it's too close
FaceMaxHeight = 80

# Timeout, so once we see a face that's big enough,
#  we don't have to see it every frame, just once every
#  FaceTimeout seconds
FaceTimeout = 1.5

TooCloseHue = 100
NormalHue = 30



Magnification = .75

CombinedSize = ( int(Magnification *  (CaptureWidth * 3)), int(Magnification * CaptureHeight))

from yunet import YuNet

# Check OpenCV version
assert cv.__version__ >= "4.8.0", \
       "Please install latest opencv-python to try this demo: python3 -m pip install --upgrade opencv-python"

# Valid combinations of backends and targets
backend_target_pairs = [
    [cv.dnn.DNN_BACKEND_OPENCV, cv.dnn.DNN_TARGET_CPU],
    [cv.dnn.DNN_BACKEND_CUDA,   cv.dnn.DNN_TARGET_CUDA],
    [cv.dnn.DNN_BACKEND_CUDA,   cv.dnn.DNN_TARGET_CUDA_FP16],
    [cv.dnn.DNN_BACKEND_TIMVX,  cv.dnn.DNN_TARGET_NPU],
    [cv.dnn.DNN_BACKEND_CANN,   cv.dnn.DNN_TARGET_NPU]
]

parser = argparse.ArgumentParser(description='YuNet: A Fast and Accurate CNN-based Face Detector (https://github.com/ShiqiYu/libfacedetection).')
parser.add_argument('--input', '-i', type=str,
                    help='Usage: Set input to a certain image, omit if using camera.')
parser.add_argument('--model', '-m', type=str, default='face_detection_yunet_2023mar.onnx',
                    help="Usage: Set model type, defaults to 'face_detection_yunet_2023mar.onnx'.")
parser.add_argument('--backend_target', '-bt', type=int, default=0,
                    help='''Choose one of the backend-target pair to run this demo:
                        {:d}: (default) OpenCV implementation + CPU,
                        {:d}: CUDA + GPU (CUDA),
                        {:d}: CUDA + GPU (CUDA FP16),
                        {:d}: TIM-VX + NPU,
                        {:d}: CANN + NPU
                    '''.format(*[x for x in range(len(backend_target_pairs))]))
parser.add_argument('--conf_threshold', type=float, default=0.9,
                    help='Usage: Set the minimum needed confidence for the model to identify a face, defauts to 0.9. Smaller values may result in faster detection, but will limit accuracy. Filter out faces of confidence < conf_threshold.')
parser.add_argument('--nms_threshold', type=float, default=0.3,
                    help='Usage: Suppress bounding boxes of iou >= nms_threshold. Default = 0.3.')
parser.add_argument('--top_k', type=int, default=5000,
                    help='Usage: Keep top_k bounding boxes before NMS.')
parser.add_argument('--save', '-s', action='store_true',
                    help='Usage: Specify to save file with results (i.e. bounding box, confidence level). Invalid in case of camera input.')
parser.add_argument('--vis', '-v', action='store_true',
                    help='Usage: Specify to open a new window to show results. Invalid in case of camera input.')
args = parser.parse_args()


# MQTT Function, callback for when we get the CONNACK response from the server
def on_connect(client, userdata, flags, rc):
    print("Connected to MQTT Server with result code " + str(rc))
    #subscribing on connect means that if we lose connection, then reconnect,
    # we renew our subscriptions
    client.subscribe("test/#")

# MQTT Function, callback for when a PUBLISH message is received from the server
def on_message(client, userdata, msg):
    print(msg.topic + " " + str(msg.payload))
    

def checkForFaces(results):
    for det in (results if results is not None else []):
        bbox = det[0:4].astype(np.int32)
        
        faceHeight = bbox[3]
        
        print("Detected a face, " + str(faceHeight) + "px high")
        if faceHeight > FaceMaxHeight:
            return True
    
    return False
    

# Adds boxes and stuff around faces
def visualize(image, results, box_color=(0, 255, 0), text_color=(0, 0, 255), fps=None):
    output = image.copy()
    landmark_color = [
        (255,   0,   0), # right eye
        (  0,   0, 255), # left eye
        (  0, 255,   0), # nose tip
        (255,   0, 255), # right mouth corner
        (  0, 255, 255)  # left mouth corner
    ]

    if fps is not None:
        cv.putText(output, 'FPS: {:.2f}'.format(fps), (0, 15), cv.FONT_HERSHEY_SIMPLEX, 0.5, text_color)

    for det in (results if results is not None else []):
        bbox = det[0:4].astype(np.int32)
        cv.rectangle(output, (bbox[0], bbox[1]), (bbox[0]+bbox[2], bbox[1]+bbox[3]), box_color, 2)
        
       
        conf = det[-1]
        cv.putText(output, '{:.4f}'.format(conf), (bbox[0], bbox[1]+12), cv.FONT_HERSHEY_DUPLEX, 0.5, text_color)

        landmarks = det[4:14].astype(np.int32).reshape((5,2))
        for idx, landmark in enumerate(landmarks):
            cv.circle(output, landmark, 2, landmark_color[idx], 2)

    return output

if __name__ == '__main__':
    backend_id = backend_target_pairs[args.backend_target][0]
    target_id = backend_target_pairs[args.backend_target][1]
    
    # Set up MQTT
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(MQTTServerIP, MQTTPort, 60)
    client.loop_start()
    
    
    # Instantiate YuNet
    model = YuNet(modelPath=args.model,
                  inputSize=[320, 320],
                  confThreshold=args.conf_threshold,
                  nmsThreshold=args.nms_threshold,
                  topK=args.top_k,
                  backendId=backend_id,
                  targetId=target_id)

    # If input is an image
    # Omit input to call default camera
    deviceId = 0
    cameraNameList = []
    for camera in glob.glob("/dev/video?"):
        print(camera)
        cameraNameList.append(camera)
        
    cap0 = cv.VideoCapture(cameraNameList[1])
    while (not cap0.isOpened()):
        x = True
    cap1 = cv.VideoCapture(cameraNameList[3])
    while (not cap1.isOpened()):
        x = True
    cap2 = cv.VideoCapture(cameraNameList[5])
    while (not cap2.isOpened()):
        x = True
    
    captureList = []
    
    captureList.append(cap0)
    captureList.append(cap1)
    captureList.append(cap2)
    print("Buffer size")
    print(cap0.get(cv.CAP_PROP_BUFFERSIZE))
    
    for cap in captureList:
        cap.set(cv.CAP_PROP_FRAME_WIDTH, CaptureWidth)
        cap.set(cv.CAP_PROP_FRAME_HEIGHT, CaptureHeight)
    
        cap.set(cv.CAP_PROP_FOURCC, cv.VideoWriter_fourcc('M', 'J', 'P', 'G'))
        cap.set(cv.CAP_PROP_BUFFERSIZE, 1)
    
    print("Buffer size")
    print(cap0.get(cv.CAP_PROP_BUFFERSIZE))


    model.setInputSize([CombinedSize[0], CombinedSize[1]])

    #timer for tracking face detection timeouts
    loopTimer = time.time()
    lastFaceDetection = False
    
    #timer for opencv
    tm = cv.TickMeter()
    while cv.waitKey(1) < 0:
        
        print("reading cameras")
        perfCounter = time.perf_counter()
        i = 0
        # We read this twice, first to empty the buffer, second so we get fresh data from the camera
        while (i < 2):
            i += 1
            hasFrame, frame0 = cap0.read()
            hasFrame1, frame1 = cap1.read()
            hasFrame2, frame2 = cap2.read()
        if not (hasFrame and hasFrame1 and hasFrame2):
            print('No frames grabbed!')
            break
        timeElapsed = time.perf_counter() - perfCounter
        print("   Time Elapsed: ", timeElapsed)
        
        print(" Image Concatenation")
        perfCounter = time.perf_counter()
        vis = cv.hconcat([frame0, frame1, frame2])
        timeElapsed = time.perf_counter() - perfCounter
        print("   Time Elapsed: ", timeElapsed)
        
        print("resizing")
        newSize = CombinedSize
        resized = cv.resize(vis, CombinedSize, interpolation = cv.INTER_LINEAR)
        timeElapsed = time.perf_counter() - perfCounter
        print("   Time Elapsed: ", timeElapsed)
        
        # Inference
        print ("Inference")
        perfCounter = time.perf_counter()
        tm.start()
        results = model.infer(resized) # results is a tuple
        tm.stop()
        timeElapsed = time.perf_counter() - perfCounter
        print("   Time Elapsed: ", timeElapsed)
        
        # Did we see a face?
        currentFaceDetection = checkForFaces(results)
        
        # New Face detected
        if (lastFaceDetection == False and currentFaceDetection == True):
            # reset our timeout timer
            loopTimer = time.time()
            lastFaceDetection = True
            #tell the world we had a face
            MQTTpublisher.single("IROS/hue", TooCloseHue, hostname=MQTTServerIP)
        
        # Face stays detected
        elif (lastFaceDetection == True and currentFaceDetection == True):
            #reset our timeout timer
            loopTimer = time.time()
            
        # Face was detected, and now isn't
        elif (lastFaceDetection == True and currentFaceDetection == False):
            
            # keep state until our timeout has expired
            if (time.time() - loopTimer >  FaceTimeout):
                
                # once timeout is expired, send a message
                MQTTpublisher.single("IROS/hue", NormalHue, hostname=MQTTServerIP)
                # set our flags
                lastFaceDetection = False
        

        
        print("drawing visualizations")
        perfCounter = time.perf_counter()
        # Draw results on the input image
        resized = visualize(resized, results, fps=tm.getFPS())
        timeElapsed = time.perf_counter() - perfCounter
        print("   Time Elapsed: ", timeElapsed)
        
        print("drawing to screen")
        perfCounter = time.perf_counter()
        # Visualize results in a new Window
        cv.imshow('YuNet Demo', resized)
        timeElapsed = time.perf_counter() - perfCounter
        print("   Time Elapsed: ", timeElapsed)
        
        tm.reset()
        
        #press q to quit
        if cv.waitKey(25) & 0xFF == ord('q'):
            break
            
    # out of the main loop, release cameras
    cap0.release()
    cap1.release()
    cap2.release()

    # and close the window
    cv.destroyAllWindows()
    
    # end the mqtt client
    client.loop_stop()
