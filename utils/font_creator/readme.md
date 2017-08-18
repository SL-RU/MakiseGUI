This utility is required to generate fonts for MakiseGUI.

# Requirements:

- python3.4 or newer
- pip3
- [PILLOW package](http://python-pillow.org/):
```Bash
pip3 install Pillow
```

# HOW TO USE

1) Select required font. Supported formats: TTF, OTF, PCF, BDF.

1.1) If font is PCF or BDF then use [pilfont](https://pillow.readthedocs.org/en/3.0.0/reference/ImageFont.html"""): 
```Bash
pilfont required_font.pcf
```
It'll create two files. "required_font.pil" is your font now.

1.2) If font is PCF or BDF determine it's encoding. Supported encodings are: "KOIR-8", (create issue to add new encodings). It is not required for TTF.

2) Generate font's bitmap using `gen_bitmap.py`:
```Bash
#for PCF or BDF
python3 gen_bitmap.py -c 32-126,1040-1103,169 -e koi8 myfont.pil MyFont
#for TTF
python3 gen_bitmap.py -c 32-126,1040-1103,169 -s 24 myfont.ttf MyFont
```
Arguments: 
* `-c` is required character's decimal codes. You can get it [from unicode table](https://unicode-table.com). For example: "32-126" is region of ordinary english alphabet, nums and few symbols, "1040-1103" is russian alphabet and 169 is copyright sign. You can mention whatever you want.
* `-e` is encoding. It is required only for PCF or BDF fonts.
* `-s` font size. Doesn't do nothing for BDF and PCF
* `myfont.xxx` - font file
* `MyFont` - font name which will be used in generated font
* `-j` Optional. Output file for json. Default is `img.json`
* `-i` Optional. Output file for bitmap. Default is `img.png`


More arguments you can see here:
```Bash
python3 gen_bitmap.py -h
```

3) You can edit generated .png bitmap as you want. Green lines is just borders - doesn't do nothing.

4) Generate font:
```Bash
python3 gen_font.py 
```

You can see arguments here:
```Bash
python3 gen_font.py -h 
```

5) Done! Font is in `out.c`. You can now copy out.c in your project, write that: `extern const MakiseFont F_MyFont;` in the header file and use that font where you want!
