/**
 ******************************************************************************
 *
 * @file       generator_common.cpp
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @brief      common functions for generating uavobjects code
 *
 * @see        The GNU Public License (GPL) Version 3
 *
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
#include "generator_common.h"

void replaceCommonTags(QString& out) 
{
    // Replace $(GENERATEDWARNING) tag
    out.replace(QString("$(GENERATEDWARNING)"), "This is a autogenerated file!! Do not modify and expect a result.");
}
/**
 * Replace all the common tags from the template file with actual object information.
 */
void replaceCommonTags(QString& out, ObjectInfo* info)
{

    QStringList updateModeStr,accessModeStr;
    updateModeStr << "UPDATEMODE_PERIODIC" << "UPDATEMODE_ONCHANGE"
        << "UPDATEMODE_MANUAL" << "UPDATEMODE_NEVER";

    accessModeStr << "ACCESS_READWRITE" << "ACCESS_READONLY";

    QString value;

    // replace the tags which don't need info 
    replaceCommonTags(out);

    // Replace $(XMLFILE) tag
    out.replace(QString("$(XMLFILE)"), info->filename);
    // Replace $(NAME) tag
    out.replace(QString("$(NAME)"), info->name);
    // Replace $(NAMELC) tag
    out.replace(QString("$(NAMELC)"), info->namelc);
    // Replace $(DESCRIPTION) tag
    out.replace(QString("$(DESCRIPTION)"), info->description);
    // Replace $(NAMEUC) tag
    out.replace(QString("$(NAMEUC)"), info->name.toUpper());
    // Replace $(OBJID) tag
    out.replace(QString("$(OBJID)"), QString().setNum(info->id));
    // Replace $(UOBJID) tag
    out.replace(QString("$(UOBJID)"), QString().setNum((qint32)info->id));
    // Replace $(OBJIDHEX) tag
    out.replace(QString("$(OBJIDHEX)"),QString("0x")+ QString().setNum(info->id,16).toUpper());
    // Replace $(ISSINGLEINST) tag
    out.replace(QString("$(ISSINGLEINST)"), boolTo01String( info->isSingleInst ));
    out.replace(QString("$(ISSINGLEINSTTF)"), boolToTRUEFALSEString( info->isSingleInst ));
    // Replace $(ISSETTINGS) tag
    out.replace(QString("$(ISSETTINGS)"), boolTo01String( info->isSettings ));
    out.replace(QString("$(ISSETTINGSTF)"), boolToTRUEFALSEString( info->isSettings ));
    // Replace $(GCSACCESS) tag
    value = accessModeStr[info->gcsAccess];
    out.replace(QString("$(GCSACCESS)"), value);
    // Replace $(FLIGHTACCESS) tag
    value = accessModeStr[info->flightAccess];
    out.replace(QString("$(FLIGHTACCESS)"), value);
    // Replace $(FLIGHTTELEM_ACKED) tag
    out.replace(QString("$(FLIGHTTELEM_ACKED)"), boolTo01String( info->flightTelemetryAcked ));
    out.replace(QString("$(FLIGHTTELEM_ACKEDTF)"), boolToTRUEFALSEString( info->flightTelemetryAcked ));
    // Replace $(FLIGHTTELEM_UPDATEMODE) tag
    value =updateModeStr[info->flightTelemetryUpdateMode];
    out.replace(QString("$(FLIGHTTELEM_UPDATEMODE)"), value);
    // Replace $(FLIGHTTELEM_UPDATEPERIOD) tag
    out.replace(QString("$(FLIGHTTELEM_UPDATEPERIOD)"), QString().setNum(info->flightTelemetryUpdatePeriod));
    // Replace $(GCSTELEM_ACKED) tag
    out.replace(QString("$(GCSTELEM_ACKED)"),  boolTo01String( info->gcsTelemetryAcked ));
    out.replace(QString("$(GCSTELEM_ACKEDTF)"),  boolToTRUEFALSEString( info->gcsTelemetryAcked ));
    // Replace $(GCSTELEM_UPDATEMODE) tag
    value = updateModeStr[info->gcsTelemetryUpdateMode];
    out.replace(QString("$(GCSTELEM_UPDATEMODE)"), value);
    // Replace $(GCSTELEM_UPDATEPERIOD) tag
    out.replace(QString("$(GCSTELEM_UPDATEPERIOD)"), QString().setNum(info->gcsTelemetryUpdatePeriod));
    // Replace $(LOGGING_UPDATEMODE) tag
    value = updateModeStr[info->loggingUpdateMode];
    out.replace(QString("$(LOGGING_UPDATEMODE)"), value);
    // Replace $(LOGGING_UPDATEPERIOD) tag
    out.replace(QString("$(LOGGING_UPDATEPERIOD)"), QString().setNum(info->loggingUpdatePeriod));


}

/**
 * Convert a boolean to string "0" or "1"
 */
QString boolTo01String(bool value)
{
    if ( value )
        return QString("1");

    return QString("0");
}

/**
 * Convert a boolean to string "TRUE" or "FALSE"
 */
QString boolToTRUEFALSEString(bool value)
{
    if ( value )
        return QString("TRUE");

    return QString("FALSE");
}

