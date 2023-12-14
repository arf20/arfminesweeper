obj-m := arfminesweeper.o
arfminesweeper-y := module.o ../common/game.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean