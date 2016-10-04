# espwebframework
Ubuntu-16.04 Build espwebframework
Note:change Makefile Defaule remove esp-open-sdk;
So you need to download and compile it first or use precompiled esp-open-sdk then open shell in the dictory run the command below.

	echo export PATH=$PWD/xtensa-lx106-elf/bin:$PWD/bin:\$PATH" >> ~/.bashrc
---------

    git clone https://github.com/23171580/espwebframework.git
    cd espwebframework
    make
    sudo make flash
    reset esp8266 board.
