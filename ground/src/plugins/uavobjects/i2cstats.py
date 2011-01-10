##
##############################################################################
#
# @file       i2cstats.py
# @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
# @brief      Implementation of the I2CStats object. This file has been 
#             automatically generated by the UAVObjectGenerator.
# 
# @note       Object definition file: i2cstats.xml. 
#             This is an automatically generated file.
#             DO NOT modify manually.
#
# @see        The GNU Public License (GPL) Version 3
#
#############################################################################/
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
#


import uavobject

import struct
from collections import namedtuple

# This is a list of instances of the data fields contained in this object
_fields = [ \
	uavobject.UAVObjectField(
		'event_errors',
		'H',
		1,
		[
			'0',
		],
		{
		}
	),
	uavobject.UAVObjectField(
		'fsm_errors',
		'H',
		1,
		[
			'0',
		],
		{
		}
	),
	uavobject.UAVObjectField(
		'irq_errors',
		'H',
		1,
		[
			'0',
		],
		{
		}
	),
	uavobject.UAVObjectField(
		'last_error_type',
		'b',
		1,
		[
			'0',
		],
		{
			'0' : 'EVENT',
			'1' : 'FSM',
			'2' : 'INTERRUPT',
		}
	),
	uavobject.UAVObjectField(
		'evirq_log',
		'I',
		5,
		[
			'0',
			'1',
			'2',
			'3',
			'4',
		],
		{
		}
	),
	uavobject.UAVObjectField(
		'erirq_log',
		'I',
		5,
		[
			'0',
			'1',
			'2',
			'3',
			'4',
		],
		{
		}
	),
	uavobject.UAVObjectField(
		'event_log',
		'b',
		5,
		[
			'0',
			'1',
			'2',
			'3',
			'4',
		],
		{
			'0' : 'I2C_EVENT_BUS_ERROR',
			'1' : 'I2C_EVENT_START',
			'2' : 'I2C_EVENT_STARTED_MORE_TXN_READ',
			'3' : 'I2C_EVENT_STARTED_MORE_TXN_WRITE',
			'4' : 'I2C_EVENT_STARTED_LAST_TXN_READ',
			'5' : 'I2C_EVENT_STARTED_LAST_TXN_WRITE',
			'6' : 'I2C_EVENT_ADDR_SENT_LEN_EQ_0',
			'7' : 'I2C_EVENT_ADDR_SENT_LEN_EQ_1',
			'8' : 'I2C_EVENT_ADDR_SENT_LEN_EQ_2',
			'9' : 'I2C_EVENT_ADDR_SENT_LEN_GT_2',
			'10' : 'I2C_EVENT_TRANSFER_DONE_LEN_EQ_0',
			'11' : 'I2C_EVENT_TRANSFER_DONE_LEN_EQ_1',
			'12' : 'I2C_EVENT_TRANSFER_DONE_LEN_EQ_2',
			'13' : 'I2C_EVENT_TRANSFER_DONE_LEN_GT_2',
			'14' : 'I2C_EVENT_NACK',
			'15' : 'I2C_EVENT_STOPPED',
			'16' : 'I2C_EVENT_AUTO',
		}
	),
	uavobject.UAVObjectField(
		'state_log',
		'b',
		5,
		[
			'0',
			'1',
			'2',
			'3',
			'4',
		],
		{
			'0' : 'I2C_STATE_FSM_FAULT',
			'1' : 'I2C_STATE_BUS_ERROR',
			'2' : 'I2C_STATE_STOPPED',
			'3' : 'I2C_STATE_STOPPING',
			'4' : 'I2C_STATE_STARTING',
			'5' : 'I2C_STATE_R_MORE_TXN_ADDR',
			'6' : 'I2C_STATE_R_MORE_TXN_PRE_ONE',
			'7' : 'I2C_STATE_R_MORE_TXN_PRE_FIRST',
			'8' : 'I2C_STATE_R_MORE_TXN_PRE_MIDDLE',
			'9' : 'I2C_STATE_R_MORE_TXN_LAST',
			'10' : 'I2C_STATE_R_MORE_TXN_POST_LAST',
			'11' : 'R_LAST_TXN_ADDR',
			'12' : 'I2C_STATE_R_LAST_TXN_PRE_ONE',
			'13' : 'I2C_STATE_R_LAST_TXN_PRE_FIRST',
			'14' : 'I2C_STATE_R_LAST_TXN_PRE_MIDDLE',
			'15' : 'I2C_STATE_R_LAST_TXN_PRE_LAST',
			'16' : 'I2C_STATE_R_LAST_TXN_POST_LAST',
			'17' : 'I2C_STATE_W_MORE_TXN_ADDR',
			'18' : 'I2C_STATE_W_MORE_TXN_MIDDLE',
			'19' : 'I2C_STATE_W_MORE_TXN_LAST',
			'20' : 'I2C_STATE_W_LAST_TXN_ADDR',
			'21' : 'I2C_STATE_W_LAST_TXN_MIDDLE',
			'22' : 'I2C_STATE_W_LAST_TXN_LAST',
			'23' : 'I2C_STATE_NACK',
		}
	),
]


class I2CStats(uavobject.UAVObject):
    ## Object constants
    OBJID        = 122889918
    NAME         = "I2CStats"
    METANAME     = "I2CStatsMeta"
    ISSINGLEINST = 1
    ISSETTINGS   = 0

    def __init__(self):
        uavobject.UAVObject.__init__(self,
                                     self.OBJID, 
                                     self.NAME,
                                     self.METANAME,
                                     0,
                                     self.ISSINGLEINST)

        for f in _fields:
            self.add_field(f)
        
    def __str__(self):
        s  = ("0x%08X (%10u)  %-30s  %3u bytes  format '%s'\n"
                 % (self.OBJID, self.OBJID, self.NAME, self.get_struct().size, self.get_struct().format))
        for f in self.get_tuple()._fields:
            s += ("\t%s\n" % f)
        return (s)

def main():
    # Instantiate the object and dump out some interesting info
    x = I2CStats()
    print (x)

if __name__ == "__main__":
    #import pdb ; pdb.run('main()')
    main()
