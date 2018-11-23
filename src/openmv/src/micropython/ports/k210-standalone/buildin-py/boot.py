import gc
import platform
import uos
import os
import machine
import common
import image
import sensor

# init pin map
pin_init=common.pin_init()
pin_init.init()

# run usr init.py file
file_list = os.ls()
for i in range(len(file_list)):
    if file_list[i] == '/init.py':
        import init
#lcd init
st=machine.nt35310()
st.init()
#ov init
ov=machine.ov5640()
ov.init()
#ov.lcdshow()

#demo=machine.demo_face_detect()
#demo.init()

# for led
led=machine.led()
led.init()
led.left_on()
led.right_on()

tripleled=machine.ws2812()
tripleled.init()
tripleled.green()

#while(True):
#	img = sensor.snapshot()
#	nop = img.show()

#roi=(80,60,160,120) #roi为中心区域160X120范围

#while(True):
#	img = sensor.snapshot()
#	for c in img.find_circles(roi):
#		img.draw_circle(c.x(),c.y(),c.r(),color=(255,0,0))
#		print(c)
#	dr = img.draw_rectangle(roi) #画出ROI
#	nop = img.show()


#roi=(80,60,160,120) #roi为中心区域160X120范围

#while(True):
#	img = sensor.snapshot()
#	for r in img.find_rects(threshold = 10000):
#		img.draw_rectangle(r.rect(), color = (255, 0, 0))
#		for p in r.corners(): img.draw_circle(p[0], p[1], 5, color = (0, 255, 0))
#		print(r)
#	dr = img.draw_rectangle(roi) #画出ROI
#	nop = img.show()

#buf=bytearray(320*240*2)
