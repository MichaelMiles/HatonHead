from PIL import Image

img = Image.open('UWHat_no1.jpeg')
img = img.convert("RGBA")
pixels = img.getdata()

newData = []
for item in pixels:
    if item[0] >= 200 and item[1] >= 200 and item[2] >= 200:
        # make it transparent by setting alpha to 1
        newData.append((0, 0, 0, 0))
    else:
        newData.append(item)

img.putdata(newData)
img.save("img2.png", "PNG")
