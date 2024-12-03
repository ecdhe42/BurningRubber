import math
from PIL import Image, ImageDraw
from PIL import ImageGrab
import turtle

#fillcolor('black')
#begin_fill()
#clear()
#end_fill()
turtle.color('blue')
turtle.hideturtle()
turtle.pensize(5)

# Create a new image
#im = Image.new('RGB', (200, 200), 'black')
#draw = ImageDraw.Draw(im)

# Define the bounding box for the arc
x1, y1 = 50, 50
x2, y2 = 150, 150
bbox = (x1, y1, x2, y2)

x = 10
y = 100
angle_degrees = 0
turn = 0

positions = []

track = [40, #First segment
         64+1,64+30, 128+1,128+1, #First turn
         40, #Second segment
         64+1,64+20, 128+1,128+1, #Second turn
         40, #Third segment
         64+1,64+1,64+40, 128+1,128+1,128+1, #Third turn
         63, #Fourth segment
         128+1,128+1,128+30, 64+1,64+1,64+1, #Fourth turn
         63, #Fith segment
         64+1,64+1,64+1,64+40, 128+1,128+1,128+1,128+1,
         30,
         128+1,128+30,64+1,64+1,
         16,
         64+1,64+1,64+37,128+1,128+1,128+1,
         60]

pos = [(1.00,0.00), (2.00,0.00), (3.00,0.00), (4.00,0.00), (5.00,0.00), (6.00,0.00), (7.00,0.00), (8.00,0.00), (9.00,0.00), (10.00,0.00), (11.00,0.00), (12.00,0.00), (13.00,0.00), (14.00,0.00), (15.00,0.00), (16.00,0.00), (17.00,0.00), (18.00,0.00), (19.00,0.00), (20.00,0.00), (21.00,0.00), (22.00,0.00), (23.00,0.00), (24.00,0.00), (25.00,0.00), (26.00,0.00), (27.00,0.00), (28.00,0.00), (29.00,0.00), (30.00,0.00), (31.00,0.00), (32.00,0.00), (33.00,0.00), (34.00,0.00), (35.00,0.00), (36.00,0.00), (37.00,0.00), (38.00,0.00), (39.00,0.00), (40.00,0.00), (41.00,-0.02), (42.00,-0.07), (42.99,-0.16), (43.99,-0.28), (44.97,-0.44), (45.96,-0.63), (46.93,-0.85), (47.90,-1.11), (48.85,-1.40), (49.80,-1.73), (50.73,-2.09), (51.65,-2.48), (52.56,-2.90), (53.45,-3.35), (54.32,-3.84), (55.18,-4.35), (56.02,-4.90), (56.84,-5.47), (57.64,-6.07), (58.42,-6.70), (59.17,-7.36), (59.90,-8.04), (60.61,-8.75), (61.29,-9.48), (61.95,-10.23), (62.58,-11.01), (63.18,-11.81), (63.75,-12.63), (64.30,-13.47), (64.81,-14.32), (65.30,-15.20), (65.77,-16.08), (66.23,-16.97), (66.70,-17.85), (67.17,-18.73), (67.64,-19.61), (68.11,-20.50), (68.58,-21.38), (69.05,-22.26), (69.52,-23.15), (69.99,-24.03), (70.46,-24.91), (70.93,-25.79), (71.40,-26.68), (71.87,-27.56), (72.34,-28.44), (72.81,-29.33), (73.28,-30.21), (73.75,-31.09), (74.22,-31.98), (74.69,-32.86), (75.15,-33.74), (75.62,-34.62), (76.09,-35.51), (76.56,-36.39), (77.03,-37.27), (77.50,-38.16), (77.97,-39.04), (78.44,-39.92), (78.91,-40.80), (79.38,-41.69), (79.85,-42.57), (80.32,-43.45), (80.79,-44.34), (81.26,-45.22), (81.73,-46.10), (82.20,-46.99), (82.67,-47.87), (83.14,-48.75), (83.61,-49.63), (84.07,-50.52), (84.54,-51.40), (85.01,-52.28), (85.47,-53.17), (85.89,-54.08), (86.28,-55.00), (86.64,-55.93), (86.96,-56.88), (87.26,-57.84), (87.52,-58.80), (87.74,-59.78), (87.93,-60.76), (88.09,-61.75), (88.21,-62.74), (88.30,-63.73), (88.35,-64.73), (88.37,-65.73), (88.35,-66.73), (88.30,-67.73), (88.21,-68.73), (88.09,-69.72), (87.93,-70.71), (87.74,-71.69), (87.52,-72.66), (87.27,-73.63), (87.03,-74.60), (86.79,-75.57), (86.55,-76.55), (86.31,-77.52), (86.06,-78.49), (85.82,-79.46), (85.58,-80.43), (85.34,-81.40), (85.10,-82.37), (84.86,-83.34), (84.61,-84.31), (84.37,-85.28), (84.13,-86.25), (83.89,-87.22), (83.65,-88.19), (83.40,-89.16), (83.16,-90.13), (82.92,-91.10), (82.68,-92.07), (82.44,-93.04), (82.19,-94.01), (81.95,-94.98), (81.71,-95.95), (81.47,-96.92), (81.23,-97.89), (80.98,-98.86), (80.74,-99.83), (80.50,-100.80), (80.26,-101.77), (80.02,-102.74), (79.77,-103.71), (79.53,-104.68), (79.29,-105.65), (79.05,-106.62), (78.81,-107.59), (78.57,-108.56), (78.32,-109.54), (78.08,-110.51), (77.84,-111.48), (77.60,-112.45), (77.36,-113.42), (77.10,-114.38), (76.80,-115.34), (76.46,-116.28), (76.07,-117.20), (75.63,-118.10), (75.15,-118.97), (74.62,-119.82), (74.04,-120.64), (73.43,-121.43), (72.77,-122.18), (72.08,-122.90), (71.35,-123.58), (70.58,-124.23), (69.78,-124.83), (68.95,-125.39), (68.10,-125.90), (67.21,-126.37), (66.31,-126.79), (65.38,-127.17), (64.43,-127.49), (63.47,-127.77), (62.50,-128.00), (61.51,-128.17), (60.52,-128.29), (59.52,-128.36), (58.52,-128.38), (57.52,-128.34), (56.53,-128.26), (55.54,-128.12), (54.56,-127.93), (53.59,-127.68), (52.63,-127.39), (51.69,-127.05), (50.77,-126.66), (49.87,-126.22), (49.00,-125.74), (48.15,-125.21), (47.33,-124.63), (46.54,-124.02), (45.79,-123.36), (45.07,-122.67), (44.38,-121.93), (43.73,-121.18), (43.09,-120.41), (42.44,-119.65), (41.80,-118.88), (41.16,-118.12), (40.51,-117.35), (39.87,-116.58), (39.23,-115.82), (38.59,-115.05), (37.94,-114.29), (37.30,-113.52), (36.66,-112.75), (36.02,-111.99), (35.37,-111.22), (34.73,-110.46), (34.09,-109.69), (33.44,-108.92), (32.80,-108.16), (32.16,-107.39), (31.52,-106.62), (30.87,-105.86), (30.23,-105.09), (29.59,-104.33), (28.94,-103.56), (28.30,-102.79), (27.66,-102.03), (27.02,-101.26), (26.37,-100.50), (25.73,-99.73), (25.09,-98.96), (24.45,-98.20), (23.80,-97.43), (23.16,-96.67), (22.52,-95.90), (21.87,-95.13), (21.23,-94.37), (20.59,-93.60), (19.95,-92.84), (19.30,-92.07), (18.66,-91.30), (18.02,-90.54), (17.37,-89.77), (16.73,-89.01), (16.09,-88.24), (15.45,-87.47), (14.80,-86.71), (14.16,-85.94), (13.52,-85.18), (12.88,-84.41), (12.23,-83.64), (11.59,-82.88), (10.95,-82.11), (10.30,-81.35), (9.66,-80.58), (9.02,-79.81), (8.38,-79.05), (7.73,-78.28), (7.09,-77.52), (6.45,-76.75), (5.80,-75.98), (5.16,-75.22), (4.52,-74.45), (3.88,-73.69), (3.23,-72.92), (2.59,-72.15), (1.95,-71.39), (1.29,-70.63), (0.61,-69.90), (-0.11,-69.21), (-0.86,-68.55), (-1.65,-67.93), (-2.47,-67.36), (-3.32,-66.83), (-4.19,-66.35), (-5.09,-65.91), (-6.01,-65.52), (-6.95,-65.18), (-7.91,-64.88), (-8.88,-64.64), (-9.86,-64.45), (-10.85,-64.31), (-11.85,-64.22), (-12.85,-64.19), (-13.85,-64.21), (-14.84,-64.28), (-15.84,-64.40), (-16.82,-64.57), (-17.80,-64.80), (-18.76,-65.07), (-19.70,-65.40), (-20.63,-65.77), (-21.54,-66.19), (-22.42,-66.66), (-23.28,-67.18), (-24.11,-67.74), (-24.90,-68.34), (-25.67,-68.98), (-26.40,-69.67), (-27.11,-70.37), (-27.80,-71.09), (-28.50,-71.81), (-29.19,-72.53), (-29.89,-73.25), (-30.58,-73.97), (-31.28,-74.69), (-31.97,-75.41), (-32.67,-76.13), (-33.36,-76.85), (-34.06,-77.57), (-34.75,-78.28), (-35.44,-79.00), (-36.14,-79.72), (-36.83,-80.44), (-37.53,-81.16), (-38.22,-81.88), (-38.92,-82.60), (-39.61,-83.32), (-40.31,-84.04), (-41.00,-84.76), (-41.70,-85.48), (-42.39,-86.20), (-43.09,-86.92), (-43.78,-87.64), (-44.48,-88.36), (-45.17,-89.08), (-45.86,-89.79), (-46.56,-90.51), (-47.25,-91.23), (-47.95,-91.95), (-48.64,-92.67), (-49.34,-93.39), (-50.03,-94.11), (-50.73,-94.83), (-51.42,-95.55), (-52.12,-96.27), (-52.81,-96.99), (-53.51,-97.71), (-54.20,-98.43), (-54.90,-99.15), (-55.59,-99.87), (-56.28,-100.58), (-56.98,-101.30), (-57.67,-102.02), (-58.37,-102.74), (-59.06,-103.46), (-59.76,-104.18), (-60.45,-104.90), (-61.15,-105.62), (-61.84,-106.34), (-62.54,-107.06), (-63.23,-107.78), (-63.93,-108.50), (-64.62,-109.22), (-65.32,-109.94), (-66.01,-110.66), (-66.70,-111.37), (-67.40,-112.09), (-68.09,-112.81), (-68.79,-113.53), (-69.48,-114.25), (-70.18,-114.97), (-70.87,-115.69), (-71.57,-116.41), (-72.26,-117.13), (-72.97,-117.84), (-73.70,-118.52), (-74.47,-119.16), (-75.28,-119.75), (-76.12,-120.28), (-77.01,-120.75), (-77.92,-121.16), (-78.86,-121.50), (-79.82,-121.77), (-80.80,-121.98), (-81.79,-122.12), (-82.79,-122.19), (-83.79,-122.19), (-84.78,-122.12), (-85.77,-121.98), (-86.75,-121.77), (-87.71,-121.50), (-88.65,-121.16), (-89.57,-120.75), (-90.45,-120.28), (-91.30,-119.75), (-92.11,-119.16), (-92.87,-118.52), (-93.59,-117.82), (-94.26,-117.08), (-94.88,-116.29), (-95.44,-115.46), (-95.94,-114.60), (-96.38,-113.70), (-96.75,-112.77), (-97.06,-111.82), (-97.30,-110.85), (-97.47,-109.87), (-97.58,-108.87), (-97.61,-107.87), (-97.58,-106.87), (-97.47,-105.88), (-97.30,-104.89), (-97.06,-103.92), (-96.75,-102.97), (-96.38,-102.04), (-95.94,-101.15), (-95.44,-100.28), (-94.89,-99.44), (-94.32,-98.62), (-93.73,-97.81), (-93.14,-97.00), (-92.56,-96.19), (-91.97,-95.39), (-91.38,-94.58), (-90.79,-93.77), (-90.20,-92.96), (-89.62,-92.15), (-89.03,-91.34), (-88.44,-90.53), (-87.85,-89.72), (-87.27,-88.91), (-86.68,-88.10), (-86.09,-87.30), (-85.50,-86.49), (-84.91,-85.68), (-84.33,-84.87), (-83.74,-84.06), (-83.15,-83.25), (-82.56,-82.44), (-81.98,-81.63), (-81.39,-80.82), (-80.80,-80.01), (-80.21,-79.21), (-79.62,-78.40), (-79.04,-77.59), (-78.45,-76.78), (-77.86,-75.97), (-77.27,-75.16), (-76.68,-74.35), (-76.10,-73.54), (-75.51,-72.73), (-74.94,-71.91), (-74.39,-71.08), (-73.88,-70.22), (-73.39,-69.34), (-72.94,-68.45), (-72.51,-67.55), (-72.12,-66.63), (-71.77,-65.69), (-71.44,-64.75), (-71.15,-63.79), (-70.89,-62.82), (-70.66,-61.85), (-70.47,-60.87), (-70.32,-59.88), (-70.19,-58.89), (-70.11,-57.89), (-70.06,-56.89), (-70.04,-55.89), (-70.06,-54.89), (-70.11,-53.89), (-70.19,-52.90), (-70.32,-51.91), (-70.47,-50.92), (-70.66,-49.94), (-70.89,-48.96), (-71.15,-48.00), (-71.44,-47.04), (-71.77,-46.09), (-72.12,-45.16), (-72.51,-44.24), (-72.94,-43.33), (-73.38,-42.44), (-73.81,-41.54), (-74.25,-40.64), (-74.69,-39.74), (-75.13,-38.84), (-75.57,-37.94), (-76.01,-37.04), (-76.44,-36.14), (-76.88,-35.25), (-77.32,-34.35), (-77.76,-33.45), (-78.20,-32.55), (-78.64,-31.65), (-79.07,-30.75), (-79.51,-29.85), (-79.95,-28.95), (-80.39,-28.05), (-80.83,-27.16), (-81.25,-26.25), (-81.64,-25.33), (-81.98,-24.39), (-82.28,-23.43), (-82.52,-22.46), (-82.71,-21.48), (-82.85,-20.49), (-82.93,-19.49), (-82.97,-18.50), (-82.95,-17.50), (-82.88,-16.50), (-82.76,-15.51), (-82.59,-14.52), (-82.36,-13.55), (-82.09,-12.59), (-81.76,-11.64), (-81.39,-10.71), (-80.96,-9.81), (-80.49,-8.92), (-79.98,-8.07), (-79.42,-7.24), (-78.82,-6.44), (-78.18,-5.67), (-77.49,-4.94), (-76.77,-4.25), (-76.02,-3.59), (-75.23,-2.97), (-74.41,-2.40), (-73.56,-1.87), (-72.69,-1.39), (-71.79,-0.95), (-70.87,-0.56), (-69.93,-0.22), (-68.97,0.08), (-68.00,0.32), (-67.02,0.51), (-66.03,0.65), (-65.04,0.74), (-64.04,0.77), (-63.04,0.77), (-62.04,0.75), (-61.04,0.74), (-60.04,0.72), (-59.04,0.70), (-58.04,0.68), (-57.04,0.67), (-56.04,0.65), (-55.04,0.63), (-54.04,0.61), (-53.04,0.60), (-52.04,0.58), (-51.04,0.56), (-50.04,0.54), (-49.04,0.53), (-48.04,0.51), (-47.04,0.49), (-46.04,0.47), (-45.04,0.46), (-44.04,0.44), (-43.04,0.42), (-42.04,0.40), (-41.04,0.39), (-40.04,0.37), (-39.04,0.35), (-38.04,0.33), (-37.04,0.32), (-36.04,0.30), (-35.04,0.28), (-34.04,0.27), (-33.04,0.25), (-32.04,0.23), (-31.04,0.21), (-30.04,0.20), (-29.04,0.18), (-28.04,0.16), (-27.04,0.14), (-26.04,0.13), (-25.04,0.11), (-24.04,0.09), (-23.04,0.07), (-22.04,0.06), (-21.04,0.04), (-20.04,0.02), (-19.04,0.00), (-18.04,-0.01), (-17.04,-0.03), (-16.04,-0.05), (-15.04,-0.07), (-14.04,-0.08), (-13.04,-0.10), (-12.04,-0.12), (-11.04,-0.14), (-10.04,-0.15), (-9.04,-0.17), (-8.04,-0.19), (-7.04,-0.21), (-6.05,-0.22), (-5.05,-0.24), (-4.05,-0.26), (-3.05,-0.28), (-2.05,-0.29), (-1.05,-0.31)]

px = []
py = []
for i in range(98):
    p = pos[round(len(pos)*i/98)]
    px.append(round((p[0]+100)/4.6)-1)
    py.append(124 - round((p[1]+130)/4.6))

screen = turtle.Screen()
t = turtle.Turtle()
t.color('blue')
t.hideturtle()
t.pensize(5)

for segment in track:
    seg_len = segment & 63
    if segment & 128:
        turn -= 1
    elif segment & 64:
        turn += 1

    print(turn)

    if turn:
        angle_degrees += seg_len * turn
        for i in range(seg_len):
            t.right(turn)
            t.forward(1)
            positions.append(t.pos())
    else:
        for _ in range(seg_len):
            t.forward(1)
            positions.append(t.pos())

canvas = screen.getcanvas()
image = ImageGrab.grab()
#img = Image.frombytes("RGB", (200, 200), canvas.postscript().encode("utf-8"))

# Save the image
#img.save("track.png")
#turtle.done()

import msvcrt as m
def wait():
    m.getch()

#wait()
