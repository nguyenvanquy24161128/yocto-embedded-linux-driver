SUMMARY = "Custom Character Device Driver for Yocto QEMU"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://COPYING;md5=a14fe441b6663e8969e960a52006e395"

SRC_URI = "file://my_driver.c \
           file://Makefile \
           file://COPYING \
          "

S = "${WORKDIR}"

inherit module

KERNEL_MODULE_AUTOLOAD += "my_driver"

