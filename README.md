# debfetch
Screenshot information tool for Debian GNU/Linux, written in C, it is fork of archfetch.
![debfetch](https://user-images.githubusercontent.com/18743742/96940467-560e0300-14bf-11eb-822d-5225a7e3244f.png)
It contains ASCII characrers, so no Unicode required.
To dowload archfetch type in your terminal:
# git clone -b master --depth 1 https://github.com/DmitryHetman/debfetch
GPU detection still doesn't work, you can just edit source code to 
substitute Intel HD with your GPU model:
# $ sandy gentoofetch.c
To run archfetch you must compile and install it:
# $ make
# (root) # make install
