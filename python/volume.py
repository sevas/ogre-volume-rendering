
import numpy as np
import matplotlib.pyplot as plt


def byteswap(in_dir, out_dir, basename, n_slices):
    for i in range(1, n_slices+1):
        f = open("%s/%s.%d" % (in_dir, basename, i), "rb")
        buf = f.read(256*256*2)
        f.close()
        img = np.frombuffer(buf, dtype="uint16")
        #img = img.reshape((256,256))
        #img = ((img.astype("float")/65536)**3)*256
        img = img.byteswap()
        img = (((img.astype("float")/65536)**0.5)*65536).astype('uint16')
        #plt.imshow(img, cmap=plt.cm.gray)
        #plt.show()
        out = open("%s/%s.%d" % (out_dir, basename, i), "wb")
        out.write(img.data)
        out.close()


def show_slice(in_dir, basename, slice_num):
    f = open("%s/%s.%d" % (in_dir, basename, slice_num), "rb")
    buf = f.read(256*256*2)
    f.close()
    img = np.frombuffer(buf, dtype="uint16")
    img = img.reshape((256,256))
    #img = img.byteswap()
    #img = ((img.astype("float")/65536)**3)*256
    #img = (((img.astype("float")/65536)**0.5)*65536).astype('uint16')
    plt.imshow(img)#, cmap=plt.cm.gray)
    plt.show()


if __name__ == "__main__":
    #byteswap("../volumes/MRbrain", 
    #         "../volumes/MRbrain2",
    #         "MRbrain",
    #         109)
    
    show_slice("../volumes/MRbrain2", "MRbrain", 50)
    
    
