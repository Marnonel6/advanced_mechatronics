# requires adafruit_ov7670.mpy and adafruit_st7735r.mpy in the lib folder
import time
from displayio import (
    Bitmap,
    Group,
    TileGrid,
    FourWire,
    release_displays,
    ColorConverter,
    Colorspace,
    Palette,
)
from adafruit_st7735r import ST7735R
import board
import busio
import digitalio
from adafruit_ov7670 import (
    OV7670,
    OV7670_SIZE_DIV1,
    OV7670_SIZE_DIV8,
    OV7670_SIZE_DIV16,
)
from ulab import numpy as np

release_displays()
spi = busio.SPI(clock=board.GP2, MOSI=board.GP3)
display_bus = FourWire(spi, command=board.GP0, chip_select=board.GP1, reset=None)
display = ST7735R(display_bus, width=160, height=128, rotation=90)

# Init Pico UART code
uart = busio.UART(board.GP4, board.GP5, baudrate=9600) #tx pin, rx pin

# Ensure the camera is shut down, so that it releases the SDA/SCL lines,
# then create the configuration I2C bus

with digitalio.DigitalInOut(board.GP10) as reset:
    reset.switch_to_output(False)
    time.sleep(0.001)
    bus = busio.I2C(board.GP9, board.GP8) #GP9 is SCL, GP8 is SDA

# Set up the camera (you must customize this for your board!)
cam = OV7670(
    bus,
    data_pins=[
        board.GP12,
        board.GP13,
        board.GP14,
        board.GP15,
        board.GP16,
        board.GP17,
        board.GP18,
        board.GP19,
    ],  # [16]     [org] etc
    clock=board.GP11,  # [15]     [blk]
    vsync=board.GP7,  # [10]     [brn]
    href=board.GP21,  # [27/o14] [red]
    mclk=board.GP20,  # [16/o15]
    shutdown=None,
    reset=board.GP10,
)  # [14]

width = display.width
height = display.height

bitmap = None
# Select the biggest size for which we can allocate a bitmap successfully, and
# which is not bigger than the display
for size in range(OV7670_SIZE_DIV1, OV7670_SIZE_DIV16 + 1):
    #cam.size = size # for 4Hz
    #cam.size = OV7670_SIZE_DIV16 # for 30x40, 9Hz
    cam.size = OV7670_SIZE_DIV8 # for 60x80, 9Hz
    if cam.width > width:
        continue
    if cam.height > height:
        continue
    try:
        bitmap = Bitmap(cam.width, cam.height, 65535)
        break
    except MemoryError:
        continue

#print(width, height, cam.width, cam.height)
if bitmap is None:
    raise SystemExit("Could not allocate a bitmap")
time.sleep(4)
g = Group(scale=1, x=(width - cam.width) // 2, y=(height - cam.height) // 2)
tg = TileGrid(
    bitmap, pixel_shader=ColorConverter(input_colorspace=Colorspace.BGR565_SWAPPED)
)
g.append(tg)
display.show(g)

t0 = time.monotonic_ns()
display.auto_refresh = False

# arrays to store the color data
reds = np.zeros(60,dtype=np.uint16)
greens = np.zeros(60,dtype=np.uint16)
blues = np.zeros(60,dtype=np.uint16)
bright = np.zeros(60)

# Prev_COM = 30

while True:
    cam.capture(bitmap)
    #bitmap[10,10] = 0 # set a pixel to black
    #bitmap[10,20] = 0 # [Y,X], [0,0] is bottom left

    # colors:
    #0x1F ->   0b0000000000011111 # all green
    #0x7E0 ->  0b0000011111100000 # all red
    #0xF800 -> 0b1111100000000000 # all blue

    # wait for a newline from the computer
    #input()
    row = 40 # which row to send to the computer
    # draw a red dot above the row, in the middle
    bitmap[row+1,30] = 0x3F<<5
    # force some colors to test the bits
    #for i in range(0,20):
    #    bitmap[row,i] = 0xF800 # blue
    #for i in range(20,40):
    #    bitmap[row,i] = 0x1F # green
    #for i in range(40,60):
    #    bitmap[row,i] = 0x7E0 # red
    # calculate the colors
    for i in range(0,60):
        reds[i] = ((bitmap[row,i]>>5)&0x3F)/0x3F*100
        greens[i] = ((bitmap[row,i])&0x1F)/0x1F*100
        blues[i] = (bitmap[row,i]>>11)/0x1F*100
        bright[i] = reds[i]+greens[i]+blues[i]

    # threshold to try to find the line
    #for i in range(0,60):
    #    if (reds[i]>50 and blues[i]>50):
    #        bitmap[row,i] = 0xFFFF
    #    else:
    #        bitmap[row,i] = 0x0000

    #COM = 30 # Center -> pixel 30
    #sum_color_index = 0
    #sum_color = 0
    #for i in range(0,60):
    #    if (greens[i]<50):
    #        bitmap[row,i] = 0xFFFF
    #        sum_color_index += i*1
    #        sum_color += 1
    #    else:
    #        bitmap[row,i] = 0x0000

    # Line in image
    #if sum_color!=0 and sum_color_index!=0:
    #    COM = sum_color_index/sum_color
    #else: # Line not in image
    #    COM = 1000


    """ WORKS WELL FOR BLACK ON WHITE """
    # G_High_px = 0
    # G_Low_px = 0
    # G_sum = 0
    # for i in range(0,60):
    #     G_sum += reds[i]
    # G_avg = G_sum/60

    # for i in range(0,60):
    #     if (reds[i]<G_avg):
    #         G_Low_px += 1
    #     else:
    #         G_High_px += 1

    # if G_Low_px < G_High_px:
    #     line = 0 # Low value are line pixels
    # else:
    #     line = 1 # High values are line pixels

    # COM = 30 # Center -> pixel 30
    # sum_color_index = 0
    # sum_color = 0
    # for i in range(0,60):
    #     if line == 0:
    #         if (reds[i]<G_avg):
    #             bitmap[row,i] = 0xFFFF
    #             sum_color_index += i*1
    #             sum_color += 1
    #         else:
    #             bitmap[row,i] = 0x0000

    #     else:
    #         if (reds[i]>G_avg):
    #             bitmap[row,i] = 0xFFFF
    #             sum_color_index += i*1
    #             sum_color += 1
    #         else:
    #             bitmap[row,i] = 0x0000


    # # Line in image
    # if sum_color!=0 and sum_color_index!=0:
    #     COM = sum_color_index/sum_color
    # else: # Line not in image9
    #     COM = 1000


    """ Choose minima beteen 3 colors """
    R_High_px = 0
    R_Low_px = 0
    R_sum = 0
    G_High_px = 0
    G_Low_px = 0
    G_sum = 0
    B_High_px = 0
    B_Low_px = 0
    B_sum = 0
    for i in range(0,60):
        R_sum += reds[i]
        G_sum += greens[i]
        B_sum += blues[i]
    R_avg = R_sum/60
    G_avg = G_sum/60
    B_avg = B_sum/60

    for i in range(0,60):
        if (reds[i]<R_avg):
            R_Low_px += 1
        elif (reds[i]>R_avg):
            R_High_px += 1

        if (greens[i]<G_avg):
            G_Low_px += 1
        elif (greens[i]>G_avg):
            G_High_px += 1

        if (blues[i]<B_avg):
            B_Low_px += 1
        elif (blues[i]>B_avg):
            B_High_px += 1

    if R_Low_px < R_High_px:
        min_red = R_Low_px # Low value are line pixels
        i_min_red = 0
    else:
        min_red = R_High_px # High values are line pixels
        i_min_red = 1

    if G_Low_px < G_High_px:
        min_green = G_Low_px # Low value are line pixels
        i_min_green = 0
    else:
        min_green = G_High_px # High values are line pixels
        i_min_green = 1
    if B_Low_px < B_High_px:
        min_blue = B_Low_px # Low value are line pixels
        i_min_blue = 0
    else:
        min_blue = B_High_px # High values are line pixels
        i_min_blue = 1

    if min_red <= min_green:
        if min_red <= min_blue:
            min_color = min_red
            i_min = i_min_red
            color = 'red'
    elif min_green <= min_blue:
        min_color = min_green
        i_min = i_min_green
        color = 'green'
    else:
        min_color = min_blue
        i_min = i_min_blue
        color = 'blue'

    COM = 30 # Center -> pixel 30
    sum_color_index = 0
    sum_color = 0
    for i in range(0,60):
       # print(f"color = {color}") # Uncomment
        if color == 'red':
            if i_min == 0:
                if (reds[i]<R_avg):
                    bitmap[row,i] = 0x0000
                    sum_color_index += i*1
                    sum_color += 1
                else:
                    bitmap[row,i] = 0xFFFF

            else:
                if (reds[i]>R_avg):
                    bitmap[row,i] = 0x0000
                    sum_color_index += i*1
                    sum_color += 1
                else:
                    bitmap[row,i] = 0xFFFF
        elif color == 'green':
            if i_min == 0:
                if (greens[i]<G_avg):
                    bitmap[row,i] = 0x0000
                    sum_color_index += i*1
                    sum_color += 1
                else:
                    bitmap[row,i] = 0xFFFF

            else:
                if (greens[i]>G_avg):
                    bitmap[row,i] = 0x0000
                    sum_color_index += i*1
                    sum_color += 1
                else:
                    bitmap[row,i] = 0xFFFF
        elif color == 'blue':
            if i_min == 0:
                if (blues[i]<B_avg):
                    bitmap[row,i] = 0x0000
                    sum_color_index += i*1
                    sum_color += 1
                else:
                    bitmap[row,i] = 0xFFFF

            else:
                if (blues[i]>B_avg):
                    bitmap[row,i] = 0x0000
                    sum_color_index += i*1
                    sum_color += 1
                else:
                    bitmap[row,i] = 0xFFFF


    # Line in image
    if sum_color!=0 and sum_color_index!=0:
        COM = sum_color_index/sum_color
    else: # Line not in image
        COM = 30

    # # Will stop jumps from happening
    # if abs(COM - Prev_COM) > 20:
    #     COM = Prev_COM

    # Prev_COM = COM



    """ END OF RGB color choose """



    """ Get avg line between RGB """
    # R_High_px = 0
    # R_Low_px = 0
    # R_sum = 0
    # G_High_px = 0
    # G_Low_px = 0
    # G_sum = 0
    # B_High_px = 0
    # B_Low_px = 0
    # B_sum = 0
    # for i in range(0,60):
    #     R_sum += reds[i]
    #     G_sum += greens[i]
    #     B_sum += blues[i]
    # R_avg = R_sum/60
    # G_avg = G_sum/60
    # B_avg = B_sum/60

    # for i in range(0,60):
    #     if (reds[i]<R_avg):
    #         R_Low_px += 1
    #     elif (reds[i]>R_avg):
    #         R_High_px += 1

    #     if (greens[i]<G_avg):
    #         G_Low_px += 1
    #     elif (greens[i]>G_avg):
    #         G_High_px += 1

    #     if (blues[i]<B_avg):
    #         B_Low_px += 1
    #     elif (blues[i]>B_avg):
    #         B_High_px += 1

    # if R_Low_px < R_High_px:
    #     min_red = R_Low_px # Low value are line pixels
    #     i_min_red = 0
    # else:
    #     min_red = R_High_px # High values are line pixels
    #     i_min_red = 1

    # if G_Low_px < G_High_px:
    #     min_green = G_Low_px # Low value are line pixels
    #     i_min_green = 0
    # else:
    #     min_green = G_High_px # High values are line pixels
    #     i_min_green = 1
    # if B_Low_px < B_High_px:
    #     min_blue = B_Low_px # Low value are line pixels
    #     i_min_blue = 0
    # else:
    #     min_blue = B_High_px # High values are line pixels
    #     i_min_blue = 1

    # if min_red <= min_green:
    #     if min_red <= min_blue:
    #         min_color = min_red
    #         i_min = i_min_red
    #         color = 'red'
    # elif min_green <= min_blue:
    #     min_color = min_green
    #     i_min = i_min_green
    #     color = 'green'
    # else:
    #     min_color = min_blue
    #     i_min = i_min_blue
    #     color = 'blue'

    # COM = 0
    # COM_R = 30 # Center -> pixel 30
    # sum_color_index_R = 0
    # sum_color_R = 0
    # COM_G = 30 # Center -> pixel 30
    # sum_color_index_G = 0
    # sum_color_G = 0
    # COM_B = 30 # Center -> pixel 30
    # sum_color_index_B = 0
    # sum_color_B = 0

    # for i in range(0,60):
    #    # print(f"color = {color}") # Uncomment
    #         if i_min_red == 0:
    #             if (reds[i]<R_avg):
    #                 bitmap[row,i] = 0x0000
    #                 sum_color_index_R += i*1
    #                 sum_color_R += 1
    #             else:
    #                 bitmap[row,i] = 0xFFFF

    #         else:
    #             if (reds[i]>R_avg):
    #                 bitmap[row,i] = 0x0000
    #                 sum_color_index_R += i*1
    #                 sum_color_R += 1
    #             else:
    #                 bitmap[row,i] = 0xFFFF

    #         if i_min_green == 0:
    #             if (greens[i]<G_avg):
    #                 bitmap[row,i] = 0x0000
    #                 sum_color_index_G += i*1
    #                 sum_color_G += 1
    #             else:
    #                 bitmap[row,i] = 0xFFFF

    #         else:
    #             if (greens[i]>G_avg):
    #                 bitmap[row,i] = 0x0000
    #                 sum_color_index_G += i*1
    #                 sum_color_G += 1
    #             else:
    #                 bitmap[row,i] = 0xFFFF

    #         if i_min_blue == 0:
    #             if (blues[i]<B_avg):
    #                 bitmap[row,i] = 0x0000
    #                 sum_color_index_B += i*1
    #                 sum_color_B += 1
    #             else:
    #                 bitmap[row,i] = 0xFFFF

    #         else:
    #             if (blues[i]>B_avg):
    #                 bitmap[row,i] = 0x0000
    #                 sum_color_index_B += i*1
    #                 sum_color_B += 1
    #             else:
    #                 bitmap[row,i] = 0xFFFF

    # # Line in image
    # if sum_color_R!=0 and sum_color_index_R!=0:
    #     COM_R = sum_color_index_R/sum_color_R
    # else: # Line not in image
    #     COM_R = 30

    # # Line in image
    # if sum_color_G!=0 and sum_color_index_G!=0:
    #     COM_G = sum_color_index_G/sum_color_G
    # else: # Line not in image
    #     COM_G = 30

    # # Line in image
    # if sum_color_B!=0 and sum_color_index_B!=0:
    #     COM_B = sum_color_index_B/sum_color_B
    # else: # Line not in image
    #     COM_B = 30

    # COM = (COM_B + COM_G + COM_R)/3.0


    """ END OF RGB average line """


    # draw a red line for estimate line position
    for i in range(0,80):
        bitmap[i, int(COM)] = 0x3F<<5


    #print(f"COM = {COM}") # Uncomment

    # print the raw pixel value to the computer
    #for i in range(0,60):
    #    print(str(i)+" "+str(bitmap[row,i]))


    # Uncomment
    # After reading an image and finding the line send the center to PIC over UART
    value_str = str(COM) +'\n'
    uart.write(value_str.encode())

    bitmap.dirty() # updae the image on the screen
    display.refresh(minimum_frames_per_second=0)
    t1 = time.monotonic_ns()
    #print("fps", 1e9 / (t1 - t0))
    t0 = t1

