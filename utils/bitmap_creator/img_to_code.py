from string import Template
from PIL import Image
import argparse
import os
import sys

temp = ""
template_src_path = "bitmap.template"
template_header_path = "bitmap_header.template"
template_path = template_src_path

parser = argparse.ArgumentParser(
    description='Create makise bitmap code from image.')
parser.add_argument('-b', '--binary', action='store_true',
                    help='Make output table binary. Default is hex')
parser.add_argument('-c', '--comment', type=str, help='Add comment')
parser.add_argument('-o', '--output', action='store_true',
                    help='Print acii image')
parser.add_argument('-d', '--header', action='store_true',
                    help='Output text to be added in header file.')
parser.add_argument('-l', '--max-line-len', type=int, default=50,
                    help='Sets max line lendth. Default is 50')
parser.add_argument('image',
                    help='Required image/')
parser.add_argument('name',
                    help='Name of bitmap/')


# parser.add_argument('-o', '--out', default='out.c',
#                    help='Specifiy output file. Default is out.c')
# parser.add_argument('-s', '--space', default=0,
#                    help='Space between characters in pixels. Default is 0')

args = parser.parse_args()

max_ll = args.max_line_len
output_hex = not args.binary
comment = ""
if args.comment is not None:
    comment = args.comment
    if comment != "":
        comment = "// " + comment + "\n"

s_pth = os.path.dirname(sys.argv[0])
if args.header:
    template_path = os.path.join(s_pth, template_header_path)
else:
    template_path = os.path.join(s_pth, template_src_path)
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
    global max_ll
    s = ""
    i = 0
    sg = ""
    ou = ""
    bits = list()
    for i in range(0, im.width * im.height):
        if i % im.width == 0:
            ou += ("\n")
        if i % 8 == 0 and i != 0:
            bits.append(sg)
            sg = ""
        rgb = im.getpixel((i % im.width, i/im.width))
        ou += "#" if rgb < 100 else " "
        sg += "1" if rgb < 100 else "0"

    if i % 8 != 0:
        while i % 8 != 0:
            i += 1
            sg += "0"
        bits.append(sg)

    bits.append("000000000")

    if args.output:
        print(ou)

    i = 0
    for b in bits:
        if i > max_ll:
            s += "\n    "
            i = 0
        ss = ""
        if(output_hex):
            ss = ("0x%02x" % int(b[::-1], 2)) + ", "
        else:
            ss = ("0b" + b[::-1]) + ", "
        s += ss
        i += len(ss)
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
