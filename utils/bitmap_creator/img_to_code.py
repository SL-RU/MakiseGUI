from string import Template
from PIL import Image
import argparse
import os
import sys

temp = ""
output_hex = True
template_path = "bitmap.template"

parser = argparse.ArgumentParser(
    description='Create makise bitmap code from image.')
parser.add_argument('-b', '--binary', action='store_true',
                    help='Make output table binary. Default is hex')
parser.add_argument('-c', '--comment', type=str, help='Add comment')
parser.add_argument('image',
                    help='Required image/')
parser.add_argument('name',
                    help='Name of bitmap/')


# parser.add_argument('-o', '--out', default='out.c',
#                    help='Specifiy output file. Default is out.c')
# parser.add_argument('-s', '--space', default=0,
#                    help='Space between characters in pixels. Default is 0')

args = parser.parse_args()

output_hex = not args.binary
comment = ""
if args.comment is not None:
    comment = args.comment
    if comment != "":
        comment = "\n// " + comment

s_pth = os.path.dirname(sys.argv[0])
template_path = os.path.join(s_pth, template_path)
if not os.path.isfile(template_path):
    print("Font template not found. Check script directory.")
    parser.print_help()
    sys.exit()
with open(template_path, "r") as f:
    temp = Template(f.read())

if not os.path.isfile(args.image):
    print("Image file %s not found" % args.image)
    parser.print_help()
    sys.exit()
im = Image.open(args.image)
im = im.convert("L")


def img_to_code(im):
    s = ""
    i = 0
    sg = ""
    ou = ""
    bits = list()
    for i in range(im.width*im.height):
        if(i % im.width == 0):
            ou += ("\n")
        if i % 8 == 0 and i != 0:
            bits.append(sg)
            sg = ""
        rgb = im.getpixel((i % im.width, i/im.width))
        if rgb > 100:
            ou += ("*")
        else:
            ou += (" ")
        sg += "1" if rgb > 100 else "0"

    if i % 8 != 0 and i % 8 != 7:
        while i % 8 != 0:
            i += 1
            sg += "0"
        bits.append(sg)

    i = 0
    for b in bits:
        # if i % max_ll == 0:
        #     s+="\n"
        i += 1
        s += ("0x%02x" % int(b[::-1], 2)) + ", "
    return s


data = img_to_code(im)

r = temp.substitute(
    name=args.name,
    width=im.width,
    height=im.height,
    data=data,
    comment=comment
)

print(r)
