#!/usr/bin/env python
# coding=utf-8

from PIL import Image, ImageChops
import sys
import os
import numpy as np

def is_real(filename, quality=85):
    resaved = filename + '.resaved.jpg'
    im = Image.open(filename)
    im.save(resaved, 'JPEG', quality=quality)
    resaved_im = Image.open(resaved)
    ela_im = ImageChops.difference(im, resaved_im)
    extrema = ela_im.getextrema()
    os.remove(resaved)
    return np.std([ex[1] for ex in extrema]) < 2

if __name__ == "__main__":
    filename = sys.argv[1]
    if is_real(filename):
        print "%s é real!" % (filename)
    else:
        print "%s é falsa!" % (filename)