/**
 ******************************************************************************
 *
 * @file       nmeaparser.cpp
 * @author     Sami Korhonen Copyright (C) 2010.
 * @addtogroup GCSPlugins GCS Plugins
 * @{
 * @addtogroup GPSGadgetPlugin GPS Gadget Plugin
 * @{
 * @brief A gadget that displays GPS status and enables basic configuration
 *****************************************************************************/
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */


#include "nmeaparser.h"
#include <iostream>
#include <math.h>
#include <QDebug>
#include <QStringList>
#include <QtGui/QWidget>
#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>

// Message Codes
#define NMEA_NODATA		0	// No data. Packet not available, bad, or not decoded
#define NMEA_GPGGA		1	// Global Positioning System Fix Data
#define NMEA_GPVTG		2	// Course over ground and ground speed
#define NMEA_GPGLL		3	// Geographic position - latitude/longitude
#define NMEA_GPGSV		4	// GPS satellites in view
#define NMEA_GPGSA		5	// GPS DOP and active satellites
#define NMEA_GPRMC		6	// Recommended minimum specific GPS data
#define NMEA_UNKNOWN	0xFF// Packet received but not known

#define GPS_TIMEOUT_MS 500

// Debugging

//#define GPSDEBUG
#define NMEA_DEBUG_PKT	///< define to enable debug of all NMEA messages

#ifdef GPSDEBUG
        #define NMEA_DEBUG_PKT	///< define to enable debug of all NMEA messages
        #define NMEA_DEBUG_GGA	///< define to enable debug of GGA messages
        #define NMEA_DEBUG_VTG	///< define to enable debug of VTG messages
        #define NMEA_DEBUG_RMC	///< define to enable debug of RMC messages
        #define NMEA_DEBUG_GSA	///< define to enable debug of GSA messages
#endif

/**
 * Initialize the parser
 */
NMEAParser::NMEAParser(QObject *parent):QObject(parent)
{
    bufferInit(&gpsRxBuffer, (unsigned char *)gpsRxData, 512);
    gpsRxOverflow=0;
}

NMEAParser::~NMEAParser()
{

}

/**
 * Called each time there are data in the input buffer
 */
void NMEAParser::processInputStream(char c)
{
        if( !bufferAddToEnd(&gpsRxBuffer, c) )
        {
                // no space in buffer
                // count overflow
                gpsRxOverflow++;
                return;
        }
        nmeaProcess(&gpsRxBuffer);
}


/**
 * Prosesses NMEA sentence checksum
 * \param[in] Buffer for parsed nmea sentence
 * \return 0 checksum not valid
 * \return 1 checksum valid
 */
char NMEAParser::nmeaChecksum(char* gps_buffer)
{
        char checksum=0;
        char checksum_received=0;

        for(int x=0; x<NMEA_BUFFERSIZE; x++)
        {
                if(gps_buffer[x]=='*')
                {
                        //Parsing received checksum...
                        checksum_received = strtol(&gps_buffer[x+1],NULL,16);
                        break;
                }
                else
                {
                        //XOR the received data...
                        checksum^=gps_buffer[x];
                }
        }
        if(checksum == checksum_received)
        {
                ++numUpdates;
                return 1;
        }
        else
        {
                ++numErrors;
                return 0;
        }
}

/**
 * Prosesses NMEA sentences
 * \param[in] cBuffer for prosessed nmea sentences
 * \return Message code for found packet
 * \return 0xFF NO packet found
 */
uint8_t NMEAParser::nmeaProcess(cBuffer* rxBuffer)
{
        uint8_t foundpacket = NMEA_NODATA;
        uint8_t startFlag = FALSE;
        //u08 data;
        uint16_t i,j;

        // process the receive buffer
        // go through buffer looking for packets
        while(rxBuffer->datalength)
        {
                // look for a start of NMEA packet
                if(bufferGetAtIndex(rxBuffer,0) == '$')
                {
                        // found start
                        startFlag = TRUE;
                        // when start is found, we leave it intact in the receive buffer
                        // in case the full NMEA string is not completely received.  The
                        // start will be detected in the next nmeaProcess iteration.

                        // done looking for start
                        break;
                }
                else
                        bufferGetFromFront(rxBuffer);
        }

        // if we detected a start, look for end of packet
        if(startFlag)
        {
                for(i=1; i<(rxBuffer->datalength)-1; i++)
                {
                        // check for end of NMEA packet <CR><LF>
                        if((bufferGetAtIndex(rxBuffer,i) == '\r') && (bufferGetAtIndex(rxBuffer,i+1) == '\n'))
                        {
                                // have a packet end
                                // dump initial '$'
                                bufferGetFromFront(rxBuffer);
                                // copy packet to NmeaPacket
                                for(j=0; j<(i-1); j++)
                                {
                                        // although NMEA strings should be 80 characters or less,
                                        // receive buffer errors can generate erroneous packets.
                                        // Protect against packet buffer overflow
                                        if(j<(NMEA_BUFFERSIZE-1))
                                                NmeaPacket[j] = bufferGetFromFront(rxBuffer);
                                        else
                                                bufferGetFromFront(rxBuffer);
                                }
                                // null terminate it
                                if (j<(NMEA_BUFFERSIZE-1)) {
                                        NmeaPacket[j] = 0;
                                } else {
                                        NmeaPacket[NMEA_BUFFERSIZE-1] = 0;
                                }
                                // dump <CR><LF> from rxBuffer
                                bufferGetFromFront(rxBuffer);
                                bufferGetFromFront(rxBuffer);
                                //DEBUG
                                #ifdef NMEA_DEBUG_PKT
                                    qDebug() << NmeaPacket;
                                #endif
                                // found a packet
                                // done with this processing session
                                foundpacket = NMEA_UNKNOWN;
                                break;
                        }
                }
        }

        if(foundpacket)
        {
                // check message type and process appropriately
                if(!strncmp(NmeaPacket, "GPGGA", 5))
                {
                        // process packet of this type
                        nmeaProcessGPGGA(NmeaPacket);
                        // report packet type
                        foundpacket = NMEA_GPGGA;
                }
                else if(!strncmp(NmeaPacket, "GPVTG", 5))
                {
                        // process packet of this type
                        nmeaProcessGPVTG(NmeaPacket);
                        // report packet type
                        foundpacket = NMEA_GPVTG;
                }
                else if(!strncmp(NmeaPacket, "GPGSA", 5))
                {
                        // process packet of this type
                        nmeaProcessGPGSA(NmeaPacket);
                        // report packet type
                        foundpacket = NMEA_GPGSA;
                }
                else if(!strncmp(NmeaPacket, "GPRMC", 5))
                {
                        // process packet of this type
                        nmeaProcessGPRMC(NmeaPacket);
                        // report packet type
                        foundpacket = NMEA_GPRMC;
                }
        }
        else if(rxBuffer->datalength >= rxBuffer->size)
        {
                // if we found no packet, and the buffer is full
                // we're logjammed, flush entire buffer
                bufferFlush(rxBuffer);
        }
        return foundpacket;
}

/**
 * Prosesses NMEA GPGGA sentences
 * \param[in] Buffer for parsed nmea GPGGA sentence
 */
void NMEAParser::nmeaProcessGPGGA(char* packet)
{
        // start parsing just after "GPGGA,"
        // attempt to reject empty packets right away
        if(packet[6]==',' && packet[7]==',')
                return;

        if(!nmeaChecksum(packet))
        {
                // checksum not valid
                return;
        }

        QString* nmeaString = new QString( packet );
        QStringList tokenslist = nmeaString->split(",");
        GpsData.GPStime = tokenslist.at(1).toDouble();
        GpsData.Latitude = tokenslist.at(2).toDouble();
        int deg = (int)GpsData.Latitude/100;
        double min = ((GpsData.Latitude)-(deg*100))/60.0;
        GpsData.Latitude=deg+min;
        // next field: N/S indicator
        // correct latitute for N/S
        if(tokenslist.at(3).contains("S")) GpsData.Latitude = -GpsData.Latitude;

        GpsData.Longitude = tokenslist.at(4).toDouble();
        deg = (int)GpsData.Longitude/100;
        min = ((GpsData.Longitude)-(deg*100))/60.0;
        GpsData.Longitude=deg+min;
        // next field: E/W indicator
        // correct latitute for E/W
        if(tokenslist.at(5).contains("W")) GpsData.Longitude = -GpsData.Longitude;

        GpsData.SV = tokenslist.at(7).toInt();

        GpsData.Altitude = tokenslist.at(9).toDouble();
        GpsData.GeoidSeparation = tokenslist.at(11).toDouble();
        emit position(GpsData.Latitude,GpsData.Longitude,GpsData.Altitude);
        emit sv(GpsData.SV);
        emit datetime(GpsData.GPSdate,GpsData.GPStime);

}

/**
 * Prosesses NMEA GPRMC sentences
 * \param[in] Buffer for parsed nmea GPRMC sentence
 */
void NMEAParser::nmeaProcessGPRMC(char* packet)
{
        // start parsing just after "GPRMC,"
        // attempt to reject empty packets right away
        if(packet[6]==',' && packet[7]==',')
                return;

        if(!nmeaChecksum(packet))
        {
                // checksum not valid
                return;
        }

        QString* nmeaString = new QString( packet );
        QStringList tokenslist = nmeaString->split(",");
        GpsData.GPStime = tokenslist.at(1).toDouble();
        GpsData.Groundspeed = tokenslist.at(7).toDouble();
        GpsData.Groundspeed = GpsData.Groundspeed*0.51444;
        GpsData.Heading = tokenslist.at(8).toDouble();
        GpsData.GPSdate = tokenslist.at(9).toDouble();
        emit datetime(GpsData.GPSdate,GpsData.GPStime);
        emit speedheading(GpsData.Groundspeed,GpsData.Heading);
}


/**
 * Prosesses NMEA GPVTG sentences
 * \param[in] Buffer for parsed nmea GPVTG sentence
 */
void NMEAParser::nmeaProcessGPVTG(char* packet)
{
        // start parsing just after "GPVTG,"
        // attempt to reject empty packets right away
        if(packet[6]==',' && packet[7]==',')
                return;

        if(!nmeaChecksum(packet))
        {
                // checksum not valid
                return;
        }

        QString* nmeaString = new QString( packet );
        QStringList tokenslist = nmeaString->split(",");
}

/**
 * Prosesses NMEA GPGSA sentences
 * \param[in] Buffer for parsed nmea GPGSA sentence
 */
void NMEAParser::nmeaProcessGPGSA(char* packet)
{
        // start parsing just after "GPGSA,"
        // attempt to reject empty packets right away
        if(packet[6]==',' && packet[7]==',')
                return;

        if(!nmeaChecksum(packet))
        {
                // checksum not valid
                return;
        }

        QString* nmeaString = new QString( packet );
        QStringList tokenslist = nmeaString->split(",");
        // next field: Mode
        // Mode: 1=Fix not available, 2=2D, 3=3D
        int mode = tokenslist.at(2).toInt();
        /*if (mode == 1)
        {
                GpsData.Status = POSITIONACTUAL_STATUS_NOFIX;
        }
        else if (mode == 2)
        {
                GpsData.Status = POSITIONACTUAL_STATUS_FIX2D;
        }
        else if (mode == 3)
        {
                GpsData.Status = POSITIONACTUAL_STATUS_FIX3D;
        }*/
        GpsData.PDOP = tokenslist.at(15).toDouble();
        GpsData.HDOP = tokenslist.at(16).toDouble();
        GpsData.VDOP = tokenslist.at(17).toDouble();

}


