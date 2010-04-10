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

#ifndef QYMODEMSEND_H
#define QYMODEMSEND_H
#include "qymodem_tx.h"
#include <QString>
#include <QFile>

#ifdef Q_OS_WIN
#define _DEVICE_SET_ QIODevice::ReadWrite|QIODevice::Unbuffered
#else
#define _DEVICE_SET_ QIODevice::ReadWrite
#endif

class QymodemSend:public QymodemTx
{

public:
    QymodemSend(QextSerialPort& port);
    int SendFile(QString filename);
    int SendFileT(QString filename);

private:
    void run();
    const char* FileName;
    void Send();
    class InFile;
    QString FileNameT;

};
#endif // QYmodemSend_H
