# Makefile for Ex3, TDT4258
#
#
#
all: compile target flash
	

flash:
	ptxdist image root.romfs
	ptxdist test flash-rootfs

compile:
	ptxdist clean driver-gamepad
	ptxdist compile driver-gamepad

target:
	ptxdist targetinstall driver-gamepad

