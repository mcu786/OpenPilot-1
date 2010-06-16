/**
******************************************************************************
*
* @file       tilecachequeue.cpp
* @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
*             Parts by Nokia Corporation (qt-info@nokia.com) Copyright (C) 2009.
* @brief      
* @see        The GNU Public License (GPL) Version 3
* @defgroup   OPMapWidget
* @{
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
#include "tilecachequeue.h"



 
namespace core {
TileCacheQueue::TileCacheQueue()
{

}

void TileCacheQueue::EnqueueCacheTask(CacheItemQueue &task)
{
#ifdef DEBUG_TILECACHEQUEUE
    qDebug()<<"DB Do I EnqueueCacheTask"<<task.GetPosition().X()<<","<<task.GetPosition().Y();
#endif //DEBUG_TILECACHEQUEUE
    if(!tileCacheQueue.contains(task))
    {
#ifdef DEBUG_TILECACHEQUEUE
        qDebug()<<"EnqueueCacheTask"<<task.GetPosition().X()<<","<<task.GetPosition().Y();
#endif //DEBUG_TILECACHEQUEUE
        mutex.lock();
        tileCacheQueue.enqueue(task);
        mutex.unlock();
        if(this->isRunning())
        {
#ifdef DEBUG_TILECACHEQUEUE
            qDebug()<<"Wake Thread";
#endif //DEBUG_TILECACHEQUEUE
            wait.wakeOne();
        }
        else
        {
#ifdef DEBUG_TILECACHEQUEUE
            qDebug()<<"Start Thread";
#endif //DEBUG_TILECACHEQUEUE
            this->start(QThread::LowestPriority);
        }
    }

}
void TileCacheQueue::run()
{
#ifdef DEBUG_TILECACHEQUEUE
    qDebug()<<"Cache Engine Start";
#endif //DEBUG_TILECACHEQUEUE
    while(true)
    {
        CacheItemQueue task;
#ifdef DEBUG_TILECACHEQUEUE
        qDebug()<<"Cache";
#endif //DEBUG_TILECACHEQUEUE
        if(tileCacheQueue.count()>0)
        {
            mutex.lock();
            task=tileCacheQueue.dequeue();
            mutex.unlock();
#ifdef DEBUG_TILECACHEQUEUE
            qDebug()<<"Cache engine Put:"<<task.GetPosition().X()<<","<<task.GetPosition().Y();
#endif //DEBUG_TILECACHEQUEUE
            Cache::Instance()->ImageCache.PutImageToCache(task.GetImg(),task.GetMapType(),task.GetPosition(),task.GetZoom());
            QThread::usleep(44);            
        }

        else
        {
            waitmutex.lock();
            if(!wait.wait(&waitmutex,4444))

            {
#ifdef DEBUG_TILECACHEQUEUE
                qDebug()<<"Cache Engine TimeOut";
#endif //DEBUG_TILECACHEQUEUE
                if(tileCacheQueue.count()==0) break;
            }
            waitmutex.unlock();
        }
    }
#ifdef DEBUG_TILECACHEQUEUE
    qDebug()<<"Cache Engine Stopped";
#endif //DEBUG_TILECACHEQUEUE
}


}