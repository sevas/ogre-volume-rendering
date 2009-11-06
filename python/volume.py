

import numpy as np
import PIL


if __name__ == "__main__":
    
    for i in range(1, 110):
        f = open("../volumes/MRbrain/MRbrain.%d" % i, 'rb')
        buf = f.read(256*256*2)
        
        
        img = np.frombuffer(buf, dtype="uint16")
        #img = img.reshape((256,256))
        #img = ((img.astype("float")/65536)**3)*256
        img = img.byteswap()
        img = (((img.astype("float")/65536)**0.5)*65536).astype('uint16')
        #plt.imshow(img, cmap=plt.cm.gray)
        #plt.show()
        out = open("../volumes/MRbrain2/MRbrain.%d" % i, "wb")
        out.write(img.data)
        f.close()
        out.close()
