import base64 
import StringIO 
import webapp2 

from PIL import Image as PILImage 
from PIL import ImageChops 
from PIL import ImageEnhance 

filename = sys.argv[1]
image = PILImage.open(filename)
source = image
if image.format is not 'JPEG':
  print 'not a jpeg'
  source_io = StringIO.StringIO()
  image.save(source_io, format='JPEG', quality=100)
  source = PILImage.open(source_io)

# Resave the source image to NN% quality
resave_io = StringIO.StringIO()
source.save(resave_io, format='JPEG', quality=75)
resaved_image = PILImage.open(StringIO.StringIO(resave_io.getvalue()))
resave_io.close()

# Take the difference between the original and the resaved image
diff_image = ImageChops.difference(source, resaved_image)
diff_image = ImageEnhance.Brightness(diff_image).enhance(20.0)

# Convert the source image to a base64 string 
temp = StringIO.StringIO()
source.save(temp, format='JPEG', quality=100)
source_b64 = base64.b64encode(temp.getvalue())

# Convert the source image to a base64 string
temp = StringIO.StringIO()
diff_image.save(temp, format='JPEG', quality=100)
diff_b64 = base64.b64encode(temp.getvalue())
template_values = {
  'source_base64': 'data:image/jpeg;base64,' + source_b64,
  'diff_base64': 'data:image/jpeg;base64,' + diff_b64
}