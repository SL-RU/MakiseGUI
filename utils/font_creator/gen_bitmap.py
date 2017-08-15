#!/bin/python3

# import PIL
from PIL import ImageFont
from PIL import Image
from PIL import ImageDraw
import koi8
import argparse
import sys
import json
import os

font_h = 0
req_chars = []
available_encodings = ["koi8"]

parser = argparse.ArgumentParser(description='Create bitmap from font.')
parser.add_argument('-s', '--size', type=int, default=12,
                    help='Size of the font in pixels. Need only for TTF')
parser.add_argument('-e', '--encoding', default="",
                    help='Encoding: "koi8". Only for bitmap font')
parser.add_argument('-c', '--characters', default="31-126",
                    help='Required character\'s unicode codes. ' +
                    'Regions like: 31-127,1040-1103,232,129')
parser.add_argument('-i', '--image', help='Out bitmap path. Default: img.png',
                    default="img.png")
parser.add_argument('-j', '--json', help='Out json path. Default: img.json',
                    default="img.json")
parser.add_argument('font', help='Font path')
parser.add_argument('name', help='Font\'s name')

args = parser.parse_args()

font_path = args.font
font_size = args.size
font_enc = args.encoding
out_image = args.image
out_json = args.json
name = args.name

if args.font is None:
    print("Missing font path(-f)")
    parser.print_help()
    sys.exit()

if font_enc is not "" and font_enc not in available_encodings:
    print("Wrong encoding! Available encodings:")
    for i in available_encodings:
        print(i)
    print("Choose one of them!")
    parser.print_help()
    sys.exit()


for i in args.characters.split(','):
    if "-" in i:
        a = i.split('-')
        if len(a) == 2 and a[0].isdigit() and a[1].isdigit():
            req_chars.append([int(a[0]), int(a[1]) + 1])
        else:
            print("Wrong argument -c %s" % i)
            parser.print_help()
            sys.exit()
    else:
        if i.isdigit():
            req_chars.append([int(i), int(i) + 1])
        else:
            print("Wrong argument -c %s" % i)
            parser.print_help()
            sys.exit()


table = []
table_w = []
const_w = -1
table_i = []

if font_path.endswith(".pcf") or font_path.endswith(".bdf"):
    print("Convert fonts to .pil.")
    print("""Use pilfont utility:
https://pillow.readthedocs.org/en/3.0.0/reference/ImageFont.html""")
    print("example: pilfont %s" % font_path)
    parser.print_help()
    sys.exit()

if not os.path.isfile(font_path):
    print("Font %s not found." % font_path)
    parser.print_help()
    sys.exit()

# return char index
def c_enc(c):
    if not font_path.endswith(".pil"):
        return chr(c)

    if(font_enc == "koi8"):
        return chr(koi8.translate(k))


if font_path.endswith(".pil"):
    font = ImageFont.load(font_path)
    font_h = font.getsize("a")[1]
    const_w = font.getsize("a")[0]
    font_size = font_h
    if(font_enc == ""):
        print("Encoding is not specified! Please mention it(--encoding)")
        parser.print_help()
        sys.exit()
    print("Font height: %s" % font_h)
else:
    font = ImageFont.truetype(font_path, font_size)
    font_h = font_size


width = 0
height = 0
last_x = 0
# create table of characters and calculate total width
for i in req_chars:
    for k in range(i[0], i[1]):
        s = font.getsize(c_enc(k))
        if s is not None and s[0] != 0 and k not in table_i:
            width += s[0] + 1
            table_i.append(k)
            height = max(height, s[1])

table_i = list(set(table_i))
table_i.sort()

img = Image.new("RGBA", (width, height + 2), (0, 0, 0, 255))
draw = ImageDraw.Draw(img)
draw.fontmode = "1"

draw.line([(0, 0), (width - 1, 0), (width - 1, height + 1),
           (0, height + 1)],
          fill=(0, 255, 0))

for k in table_i:
    s = font.getsize(c_enc(k))
    if s is not None and s[0] != 0:
        table_w.append(s[0])
        draw.line([(last_x, 0), (last_x, height + 1)], fill=(0, 255, 0))
        draw.text((last_x + 1, 1), c_enc(k), (255, 255, 255), font=font)
        last_x += s[0] + 1


draw = ImageDraw.Draw(img)
img.save(out_image)
print("Font's bitmap generated: %s" % out_image)

data = {
    "name": name,
    "font": font_path,
    "img": out_image,
    "size": font_size,
    "height":  height,
    "width":  width,
    "table_w": table_w,
    "const_w": const_w,
    "table_i": table_i
}

with open(out_json, 'w') as outfile:
    json.dump(data, outfile)
print("Font's data generated: %s" % out_json)
