import PIL
from PIL import Image, ImageDraw

img1 = Image.new("RGB", (128, 128))
img2 = Image.new("RGB", (128, 128))

def add():
    global t
    global fill
    thickness = max(1, round((64-len(t))/6.4))
    t.extend([fill]*thickness)
    fill = '.' if fill == 'X' else 'X'

tracks = []

for i in range(10):
    t = ['X']*(10-i)

    fill = '.'

    while len(t) < 64:
        add()

    arr = []

    for i in range(64):
        arr.append(i if t[63-i] == 'X' else i+64)

    tracks.append(arr)
    print('{' + ','.join([' '*(3-len(str(val)))+str(val) for val in arr]) + '},')

for i in range(10):
    t = ['.']*(10-i)

    fill = 'X'

    while len(t) < 64:
        add()

    arr = []

    for i in range(64):
        arr.append(i if t[63-i] == 'X' else i+64)

    tracks.append(arr)
    print('{' + ','.join([' '*(3-len(str(val)))+str(val) for val in arr]) + '},')

# 0-4: grass
# 5-15: border
# 16-27
# 28-39
# 40-51
# 52-63
# 64-75
# 76-87
# 88-99
# 100-112
# 113-121: border
def draw():
    col0 = PIL.ImageColor.getrgb("#000000")
    col1 = PIL.ImageColor.getrgb("#FF0000")
    col2 = PIL.ImageColor.getrgb("#00FF00")
    col3 = PIL.ImageColor.getrgb("#FFFFFF")

    draw = ImageDraw.Draw(img1)
    draw.rectangle([(0,0),(127, 127)], col2)
    draw.polygon([(64,0), (5,63), (121,63), (64,0)], col1)
    draw.polygon([(64,0), (16,63), (112,63), (64,0)], col0)
    draw.polygon([(64,0), (62,63), (66,63), (64,0)], col3)

    draw.polygon([(64,64), (5,127), (121,127), (64,64)], col0)
    draw.polygon([(64,64), (5,127), (15,127), (64,64)], col3)
    draw.polygon([(64,64), (113,127), (121,127), (64,64)], col3)
    img1.show()

    draw = ImageDraw.Draw(img2)
    draw.rectangle([(0,0),(127, 127)], col2)

    draw.polygon([(64,0), (16,63), (112,63), (64,0)], col0)
    draw.polygon([(64,0), (16,63), (27,63), (64,0)], col3)
    draw.polygon([(64,0), (40,63), (51,63), (64,0)], col3)
    draw.polygon([(64,0), (64,63), (75,63), (64,0)], col3)
    draw.polygon([(64,0), (88,63), (99,63), (64,0)], col3)
    draw.polygon([(64,0), (5,63), (15,63), (64,0)], col1)
    draw.polygon([(64,0), (113,63), (121,63), (64,0)], col1)

    draw.polygon([(64,64), (16,127), (112,127), (64,64)], col0)
    draw.polygon([(64,64), (28,127), (39,127), (64,64)], col3)
    draw.polygon([(64,64), (52,127), (63,127), (64,64)], col3)
    draw.polygon([(64,64), (76,127), (87,127), (64,64)], col3)
    draw.polygon([(64,64), (100,127), (112,127), (64,64)], col3)
    draw.polygon([(64,64), (5,127), (15,127), (64,64)], col3)
    draw.polygon([(64,64), (113,127), (121,127), (64,64)], col3)
    img2.show()

#draw()
