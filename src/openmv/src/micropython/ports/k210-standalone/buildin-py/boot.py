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

# for led
led=machine.led()
led.init()
led.left_on()
led.right_on()

tripleled=machine.ws2812()
tripleled.init()
tripleled.green()

#example1: img show
#while(True):
#	img = sensor.snapshot()
#	img = img.draw_string(20,20,"hello cannan",scale=2)
#	nop = img.show()

#example2: find circles
#roi=(80,60,160,120) #roi为中心区域160X120范围
#while(True):
#	img = sensor.snapshot()
#	for c in img.find_circles(roi):
#		img.draw_circle(c.x(),c.y(),c.r(),color=(255,0,0))
#		print(c)
#	dr = img.draw_rectangle(roi) #画出ROI
#	nop = img.show()

#examle3: find rects
#roi=(80,60,160,120) #roi为中心区域160X120范围
#while(True):
#	img = sensor.snapshot()
#	for r in img.find_rects(threshold = 10000 , roi=roi):
#		img.draw_rectangle(r.rect(), color = (255, 0, 0))
#		for p in r.corners(): img.draw_circle(p[0], p[1], 5, color = (0, 255, 0))
#		print(r)
#	dr = img.draw_rectangle(roi) #画出ROI
#	nop = img.show()

#examle4: face detect
face_cascade = image.HaarCascade("frontalface", stages=100)
print(face_cascade)
while (True):
	img = sensor.snapshot()
	objects = img.find_features(face_cascade, threshold=1.00, scale=1.1)
	for r in objects:
		img.draw_rectangle(r,color=(0,255,255))
		print(r)
	nop = img.show()
#examle5: data matrix
#while(True):
#    img = sensor.snapshot()
#    matrices = img.find_datamatrices()
#    for matrix in matrices:
#        img.draw_rectangle(matrix.rect(), color = (255, 0, 0))
#        img.draw_string(10,10,matrix.payload(),color=(0,255,255),scale=3,mono_space=False)
#        print(matrix)
#    if not matrices:
#        img.draw_string(10,10,"Nothing",color=(0,255,255),scale=3,mono_space=False)
#        print("Nothing")
#    img.show()

#exaple6: qrcode
#while(True):
#    img = sensor.snapshot()
#    for code in img.find_qrcodes():
#        img.draw_rectangle(code.rect(), color = (255, 0, 0))
#        img.draw_string(10,10,code.payload(),color=(0,255,255),scale=3,mono_space=False)
#        print(code)
#    img.show()


#buf=bytearray(320*240*2)
