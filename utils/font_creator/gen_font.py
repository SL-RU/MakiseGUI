from string import Template
import json
from PIL import Image
import argparse
import os
import sys

temp = ""
output_hex = True
template_path = "font.template"

parser = argparse.ArgumentParser(
    description='Create makise code from generated image and data.')
parser.add_argument('json', nargs="?", default="img.json",
                    help='Specifiy location of font data. Default: img.json')
parser.add_argument('-b', '--binary', action='store_true',
                    help='Make output table binary. Default is hex')
parser.add_argument('-o', '--out', default='out.c',
                    help='Specifiy output file. Default is out.c')
parser.add_argument('-s', '--space', default=0,
                    help='Space between characters in pixels. Default is 0')


args = parser.parse_args()


output_hex = not args.binary

s_pth = os.path.dirname(sys.argv[0])
template_path = os.path.join(s_pth, template_path)
if not os.path.isfile(template_path):
    print("Font template not found. Check script directory.")
    parser.print_help()
    sys.exit()
with open(template_path, "r") as f:
    temp = Template(f.read())

data = {}
if not os.path.isfile(args.json):
    print("Data json file %s not found" % args.json)
    parser.print_help()
    sys.exit()
with open(args.json, "r") as f:
    data = json.load(f)

if not os.path.isfile(data['img']):
    print("Bitmap file %s not found" % data['img'])
    parser.print_help()
    sys.exit()
im = Image.open(data['img'])

table_i = data['table_i']

# first block without gasps
# (usually codes from 32 to 127). Optimization for perfomance.
first_offset = table_i[0]
first_count = 1
last = table_i[0]
for i in table_i[1:]:
    if i != last + 1:
        break
    last = i
    first_count += 1

table_i = table_i[first_count:]


# Read bitmap and convert it in the bytes
def print_char(x, y, w, h):
    global im, output_hex
    i = 0
    sg = ""
    bits = list()
    for i in range((h + 1) * w):
        if(i % 8 == 0 and i != 0):
            bits.append(sg)
            sg = ""
        r, g, b, a = im.getpixel((x + i % w, y + i/w))
        sg += '1' if (r == g == b == 255) else '0'
    # print(i)
    #if i % 8 != 0 and i % 8 != 7:
    while i % 8 != 0:
        i += 1
        sg += "0"
        bits.append(sg)
    out = ""
    bytesinchar = 0
    for z in bits:
        if(output_hex):
            out += ("0x%02x" % int(z[::-1], 2)) + ", "
        else:
            out += "0b" + z[::-1] + ", "
        bytesinchar += 1
    return out


table = ""
table_o = []
table_w = data['table_w']
x = 1
o = 0
i = 0
for w in data['table_w']:
    s = print_char(x, 1, w, data["height"])
    if not chr(data['table_i'][i]) == '\\':
        table += s + '//' + chr(data['table_i'][i])
    else:
        table += s
    table += '\n'
    table_o.append(o)
    x += w + 1
    o += s.count(',')
    i += 1

table_w = str(table_w)[1:-1]
table_o = str(table_o)[1:-1]
table_i = str(table_i)[1:-1]

r = temp.substitute(
    name=data['name'],
    size=data['size'],
    table=table,
    width=table_w,  # if data['const_w'] == -1 else 0,
    offset=table_o,
    unicode=table_i,
    const_width=data['const_w'] if data['const_w'] != -1 else 0,
    height=data['height'],
    first_offset=first_offset,
    first_count=first_count,
    total_count=len(data["table_i"]),
    space=args.space
)
with open(args.out, "w") as f:
    f.write(r)

print("Font file %s generated" % args.out)
