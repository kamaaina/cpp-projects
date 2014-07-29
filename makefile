PROJECT=cpp-projects

ALL:
PROJECT=cpp-projects

ALL:
	cd i2c; make
	cd bmp180; make
	cd oled; make

clean:
	cd i2c; make clean
	cd bmp180; make clean
	cd oled; make clean
