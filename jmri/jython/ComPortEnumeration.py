# This is a simple script to enumerate the communication ports
# available on the computer.
#
# Author: Matthew Harris, copyright (C) 2010.
# Part of the JMRI distribution
#
# The next line is maintained by CVS, please don't change it
# $Revision: 27263 $

import gnu.io
#import ch.ntb.usb

def portType(type):
    if type == gnu.io.CommPortIdentifier.PORT_PARALLEL:
        return "Parallel"
    elif type == gnu.io.CommPortIdentifier.PORT_SERIAL:
        return "Serial"
    elif type == gnu.io.CommPortIdentifier.PORT_I2C:
        return "I2C"
    elif type == gnu.io.CommPortIdentifier.PORT_RS485:
        return "RS485"
    elif type == gnu.io.CommPortIdentifier.PORT_RAW:
        return "Raw"
    else:
        return "Unknown type"

print "---------------------------------"
print "Enumerating available com ports"
print "---------------------------------"

portnames = gnu.io.CommPortIdentifier.getPortIdentifiers()

for portname in portnames:
    print "Port: ", portname.name, " type: ", portType(portname.getPortType())

print "---------------------------------"
print "Enumerating available USB devices"
print "(via JInput)"
print "---------------------------------"

usbmodel = jmri.jmrix.jinput.TreeModel.instance()
usbdevices = usbmodel.controllers()

for usbdevice in usbdevices:
    print "Device: ", usbdevice.toString()

#print "---------------------------------"
#print "Enumerating available USB devices"
#print "(via Libusb)"
#print "---------------------------------"
#
#ch.ntb.usb.LibusbJava.usb_init()
#ch.ntb.usb.LibusbJava.usb_find_busses()
#ch.ntb.usb.LibusbJava.usb_find_devices()
#
#usbbus = ch.ntb.usb.LibusbJava.usb_get_busses()
#
#ch.ntb.usb.Utils.logBus(usbbus)

print "---------------------------------"
print "Done"