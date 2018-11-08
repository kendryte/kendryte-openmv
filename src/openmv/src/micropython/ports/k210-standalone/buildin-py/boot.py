import gc
import platform
import uos
import os
import machine
import common

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
ov.lcdshow()

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

buf=bytearray(320*240*2)