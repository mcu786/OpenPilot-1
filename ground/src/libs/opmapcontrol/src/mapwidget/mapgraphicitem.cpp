#include "mapgraphicitem.h"
namespace mapcontrol
{
    MapGraphicItem::MapGraphicItem(internals::Core *core):core(core),MapRenderTransform(1), maxZoom(17),minZoom(2),zoomReal(0),isSelected(false),rotation(0)
    {
        EmptytileBrush = Qt::cyan;
        MissingDataFont =QFont ("Times",10,QFont::Bold);
        EmptyTileText = "We are sorry, but we don't\nhave imagery at this zoom\nlevel for this region.";
        EmptyTileBorders = QPen(Qt::white);
        ScalePen = QPen(Qt::blue);
        SelectionPen = QPen(Qt::blue);
        MapScaleInfoEnabled = true;
        showTileGridLines=true;
        DragButton = Qt::RightButton;
        isMouseOverMarker=false;
        maprect=QRectF(0,0,1022,680);
        core->SetCurrentRegion(internals::Rectangle(0, 0, maprect.width(), maprect.height()));
        core->SetMapType(MapType::GoogleHybrid);
        this->SetZoom(2);
        connect(core,SIGNAL(OnNeedInvalidation()),this,SLOT(Core_OnNeedInvalidation()));


        //resize();
    }
    void MapGraphicItem::start()
    {
        core->StartSystem();
    }

    void MapGraphicItem::resize(const QRectF &rect)
    {

        {
            maprect=boundingBox(scene()->sceneRect(),rotation);
            this->setTransform(QTransform().translate(-(maprect.width()-scene()->width())/2,-(maprect.height()-scene()->height())/2));
            this->setTransformOriginPoint(maprect.center().x(),maprect.center().y());
            this->setRotation(rotation);
        }

        core->OnMapSizeChanged(maprect.width(),maprect.height());
        core->SetCurrentRegion(internals::Rectangle(0, 0, maprect.width(), maprect.height()));
        if(isVisible())
        {
            core->GoToCurrentPosition();
        }
    }

    QRectF MapGraphicItem::boundingRect() const
    {
        const int Margin = 1;
        return maprect.adjusted(-Margin, -Margin, +Margin, +Margin);
    }
    void MapGraphicItem::Core_OnNeedInvalidation()
    {
        this->update();
    }
    void MapGraphicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
    {

        if(MapRenderTransform!=1)
        {
            QTransform transform;
            transform.scale(MapRenderTransform,MapRenderTransform);
            painter->setWorldTransform(transform);
            {
                DrawMap2D(painter);
            }
            painter->resetTransform();
        }
        else
        {
            DrawMap2D(painter);
        }
        painter->drawRect(maprect);
    }
    void MapGraphicItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {
        if(core->IsDragging())
        {
            core->mouseCurrent.SetX(event->pos().x());
            core->mouseCurrent.SetY(event->pos().y());

            {
                core->Drag(core->mouseCurrent);
            }

        }
    }
    void MapGraphicItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        if(!IsMouseOverMarker())
        {
            if(event->button() == DragButton && CanDragMap())
            {
                core->mouseDown.SetX(event->pos().x());
                core->mouseDown.SetY(event->pos().y());


                this->setCursor(Qt::SizeAllCursor);

                core->BeginDrag(core->mouseDown);
                this->update();

            }
            else if(!isSelected)
            {
                isSelected = true;
                SetSelectedArea (internals::RectLatLng::Empty);
                selectionEnd = internals::PointLatLng::Empty;
                selectionStart = FromLocalToLatLng(event->pos().x(), event->pos().y());
            }
        }
    }
    void MapGraphicItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        if(isSelected)
        {
            isSelected = false;
        }

        if(core->IsDragging())
        {
            core->EndDrag();

            this->setCursor(Qt::ArrowCursor);
            if(!BoundsOfMap.IsEmpty() && !BoundsOfMap.Contains(core->CurrentPosition()))
            {
                if(!core->LastLocationInBounds.IsEmpty())
                {
                    core->SetCurrentPosition(core->LastLocationInBounds);
                }
            }
        }
        else
        {
            if(!selectionEnd.IsEmpty() && !selectionStart.IsEmpty())
            {
                if(!selectedArea.IsEmpty() && event->modifiers() == Qt::ShiftModifier)
                {
                    //   SetZoomToFitRect(SelectedArea());TODO
                }
            }

        }
    }
    void MapGraphicItem::wheelEvent(QGraphicsSceneWheelEvent *event)
    {
        if(!IsMouseOverMarker() && !IsDragging())
        {
            if(core->GetmouseLastZoom().X() != event->pos().x() && core->mouseLastZoom.Y() != event->pos().y())
            {
                if(GetMouseWheelZoomType() == internals::MouseWheelZoomType::MousePositionAndCenter)
                {
                    core->SetCurrentPosition(FromLocalToLatLng(event->pos().x(), event->pos().y()));
                }
                else if(GetMouseWheelZoomType() == internals::MouseWheelZoomType::ViewCenter)
                {
                    core->SetCurrentPosition(FromLocalToLatLng((int) maprect.width()/2, (int) maprect.height()/2));
                }
                else if(GetMouseWheelZoomType() == internals::MouseWheelZoomType::MousePositionWithoutCenter)
                {
                    core->SetCurrentPosition(FromLocalToLatLng(event->pos().x(), event->pos().y()));

                }

                core->mouseLastZoom.SetX((event->pos().x()));
                core->mouseLastZoom.SetY((event->pos().y()));
            }

            // set mouse position to map center
            if(GetMouseWheelZoomType() != internals::MouseWheelZoomType::MousePositionWithoutCenter)
            {
                {
                    //                      System.Drawing.Point p = PointToScreen(new System.Drawing.Point(Width/2, Height/2));
                    //                      Stuff.SetCursorPos((int) p.X, (int) p.Y);
                }
            }

            core->MouseWheelZooming = true;

            if(event->delta() > 0)
            {
                SetZoom(Zoom()+1);
            }
            else if(event->delta() < 0)
            {
                SetZoom(Zoom()-1);
            }

            core->MouseWheelZooming = false;
        }
    }
    void MapGraphicItem::DrawMap2D(QPainter *painter)
    {
        // qDebug()<<core->Matrix.count();
        // painter.drawText(10,10,"TESTE");
        for(int i = -core->GetsizeOfMapArea().Width(); i <= core->GetsizeOfMapArea().Width(); i++)
        {
            for(int j = -core->GetsizeOfMapArea().Height(); j <= core->GetsizeOfMapArea().Height(); j++)
            {
                core->SettilePoint (core->GetcenterTileXYLocation());
                core->SettilePoint(Point(core->GettilePoint().X()+ i,core->GettilePoint().Y()+j));



                {
                    internals::Tile* t = core->Matrix.TileAt(core->GettilePoint());
                    //qDebug()<<"OPMapControl::DrawMap2D tile:"<<t->GetPos().ToString()<<" as "<<t->Overlays.count()<<" overlays";
                    //Tile t = core->Matrix[tileToDraw];
                    if(t!=0)
                    {
                        //qDebug()<< "opmapcontrol:draw2d TileHasValue:"<<t->GetPos().ToString();
                        core->tileRect.SetX(core->GettilePoint().X()*core->tileRect.Width());
                        core->tileRect.SetY(core->GettilePoint().Y()*core->tileRect.Height());
                        core->tileRect.Offset(core->GetrenderOffset());

                        if(core->GetCurrentRegion().IntersectsWith(core->tileRect))
                        {
                            bool found = false;

                            // render tile
                            //lock(t.Overlays)
                            {
                                foreach(QByteArray img,t->Overlays)
                                {
                                    if(img.count()!=0)
                                    {
                                        if(!found)
                                            found = true;
                                        {
                                            painter->drawPixmap(core->tileRect.X(),core->tileRect.Y(), core->tileRect.Width(), core->tileRect.Height(),PureImageProxy::FromStream(img));

                                        }
                                    }
                                }
                            }

                            if(showTileGridLines)
                            {
                                painter->setPen(EmptyTileBorders);
                                painter->drawRect(core->tileRect.X(), core->tileRect.Y(), core->tileRect.Width(), core->tileRect.Height());
                                {
                                    painter->setFont(MissingDataFont);
                                    painter->setPen(Qt::red);
                                    painter->drawText(QRectF(core->tileRect.X(), core->tileRect.Y(), core->tileRect.Width(), core->tileRect.Height()),Qt::AlignCenter,(core->GettilePoint() == core->GetcenterTileXYLocation()? "CENTER: " :"TILE: ")+core->GettilePoint().ToString());
                                    //qDebug()<<"ShowTileGridLine:"<<core->GettilePoint().ToString()<<"=="<<core->GetcenterTileXYLocation().ToString();
                                }
                            }

                            // add text if tile is missing
                            if(!found)
                            {

                                painter->fillRect(QRectF(core->tileRect.X(), core->tileRect.Y(), core->tileRect.Width(), core->tileRect.Height()),EmptytileBrush);
                                painter->setFont(MissingDataFont);
                                painter->drawText(QRectF(core->tileRect.X(), core->tileRect.Y(), core->tileRect.Width(), core->tileRect.Height()),EmptyTileText);



                                painter->setPen(EmptyTileBorders);
                                painter->drawRect(core->tileRect.X(), core->tileRect.Y(), core->tileRect.Width(), core->tileRect.Height());

                                // raise error

                            }
                        }
                    }
                }
            }
        }
    }
    internals::PointLatLng MapGraphicItem::FromLocalToLatLng(int x, int y)
    {
        if(MapRenderTransform!=-1)
        {
            x = (int) (x * MapRenderTransform);
            y = (int) (y * MapRenderTransform);
        }
        return core->FromLocalToLatLng(x, y);
    }
    double MapGraphicItem::Zoom()
    {
        return zoomReal;
    }
    void MapGraphicItem::SetZoom(double const& value)
    {
        if(zoomReal != value)
        {
            if(value > MaxZoom())
            {
                zoomReal = MaxZoom();
            }
            else
                if(value < MinZoom())
                {
                zoomReal = MinZoom();
            }
            else
            {
                zoomReal = value;
            }

            float remainder = (float)std::fmod((float) value, (float) 1);
            if(remainder != 0)
            {
                float scaleValue = remainder + 1;
                {
                    MapRenderTransform = scaleValue;
                }

                SetZoomStep((qint32)(value - remainder));

                this->update();

            }
            else
            {

                MapRenderTransform = 1;

                SetZoomStep ((qint32)(value));
                zoomReal = ZoomStep();
                this->update();
            }
        }
    }
    int MapGraphicItem::ZoomStep()const
    {
        return core->Zoom();
    }
    void MapGraphicItem::SetZoomStep(int const& value)
    {
        if(value > MaxZoom())
        {
            core->SetZoom(MaxZoom());
            emit zoomChanged(MaxZoom());
        }
        else if(value < MinZoom())
        {
            core->SetZoom(MinZoom());
            emit zoomChanged(MinZoom());
        }
        else
        {
            core->SetZoom(value);
            emit zoomChanged(value);
        }

    }

    void MapGraphicItem::Offset(int const& x, int const& y)
    {
        core->DragOffset(Point(x, y));
    }
    void MapGraphicItem::mapRotate(qreal angle)
    {
        rotation=angle;
        resize(scene()->sceneRect());
    }
    QRectF MapGraphicItem::boundingBox(const QRectF &rect, const qreal &angle)
    {
        QRectF ret(rect);
        float c=cos(angle*2*M_PI/360);
        float s=sin(angle*2*M_PI/360);
        ret.setHeight(rect.height()*fabs(c)+rect.width()*fabs(s));
        ret.setWidth(rect.width()*fabs(c)+rect.height()*fabs(s));
        return ret;
    }
    QSize MapGraphicItem::sizeHint()const
    {
        core::Size size=core->projection->GetTileMatrixMaxXY(MinZoom());
        core::Size tilesize=core->projection->TileSize();
        QSize rsize((size.Width()+1)*tilesize.Width(),(size.Height()+1)*tilesize.Height());
        return rsize;
    }
}