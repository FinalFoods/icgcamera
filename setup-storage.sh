#!/bin/bash

if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root"
    exit 1
fi

# Make filesystem and setup storage DCF layout
#
whiptail --yesno "Making filesystem on ${1}. Continue?" 10 40
ret=$?
if [ "$ret" == 0 ]; then
    echo "Making filesystem on $1"
fi

mkfs.ext4 "$1" -L ICGCAMERA
mount "$1" /mnt

# make the necessary directories for a DCF layout
mkdir -p /mnt/DCIM

# on the RPI add the fstab entry and mount the disk
#echo "LABEL=ICGCAMERA	/media/icgcamera	ext4	defaults	0	0" >> /etc/fstab
#mount -a

