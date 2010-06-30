/**
 ******************************************************************************
 *
 * @file       opmapgadgetwidget.cpp
 * @author     The OpenPilot Team, http://www.openpilot.org Copyright (C) 2010.
 * @brief
 * @see        The GNU Public License (GPL) Version 3
 * @defgroup   opmap
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

#include "opmapgadgetwidget.h"
#include "ui_opmap_widget.h"

#include <QStringList>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QDir>

#include "extensionsystem/pluginmanager.h"

// *************************************************************************************
// constructor

OPMapGadgetWidget::OPMapGadgetWidget(QWidget *parent) : QWidget(parent)
{
    // **************

    m_widget = NULL;
    m_map = NULL;
    wayPoint_treeView_model = NULL;
    findPlaceCompleter = NULL;
    m_map_graphics_scene = NULL;
    m_map_scene_proxy = NULL;
    m_map_overlay_widget = NULL;

    m_mouse_waypoint = NULL;

    prev_tile_number = 0;

    // **************
    // create the widget that holds the user controls and the map

    m_widget = new Ui::OPMap_Widget();
    m_widget->setupUi(this);

    // **************
    // create the map widget

    m_map = new mapcontrol::OPMapWidget();

    if (m_map)
    {
	m_map->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
	m_map->setMinimumSize(64, 64);

	m_map->setFrameStyle(QFrame::NoFrame);

	m_map->configuration->DragButton = Qt::LeftButton;  // use the left mouse button for map dragging
    }

    // **************

    setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_map);
//    layout->addChildWidget();
    m_widget->mapWidget->setLayout(layout);

    // **************
    // create the user controls overlayed onto the map
/*
    m_map_overlay_widget = new OPMap_MapOverlayWidget();

    m_map_graphics_scene = m_map->scene();
    m_map_scene_proxy = m_map_graphics_scene->addWidget(m_map_overlay_widget);

    m_map_overlay_widget->setGeometry(m_widget->mapWidget->geometry());
*/
    // **************
    // Get required UAVObjects

    ExtensionSystem::PluginManager *pm = ExtensionSystem::PluginManager::instance();
    UAVObjectManager *objManager = pm->getObject<UAVObjectManager>();
    m_positionActual = PositionActual::GetInstance(objManager);

    // **************
    // create various context (mouse right click) menu actions

    createActions();

    // **************
    // set the user control options

    m_widget->labelUAVPos->setText("---");
    m_widget->labelMapPos->setText("---");
    m_widget->labelMousePos->setText("---");

    m_widget->progressBarMap->setMaximum(1);

    m_widget->frameFlightControls->setVisible(false);
    m_widget->toolButtonFlightControlsShowHide->setIcon(QIcon(QString::fromUtf8(":/core/images/prev.png")));

    m_widget->treeViewWaypoints->setVisible(false);
    m_widget->toolButtonWaypointsTreeViewShowHide->setIcon(QIcon(QString::fromUtf8(":/core/images/next.png")));


    // **************
    // add an auto-completer to the find-place line edit box
/*
    findPlaceWordList << "england" << "london" << "birmingham" << "shropshire";
    QCompleter *findPlaceCompleter = new QCompleter(findPlaceWordList, this);
    findPlaceCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    findPlaceCompleter->setCompletionMode(QCompleter::PopupCompletion);
    findPlaceCompleter->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    m_widget->comboBoxFindPlace->setCompleter(findPlaceCompleter);
*/
    m_widget->comboBoxFindPlace->setAutoCompletion(true);

    connect( m_widget->comboBoxFindPlace->lineEdit(), SIGNAL(returnPressed()), this, SLOT(on_comboBoxFindPlace_returnPressed()));

    // **************
    // map stuff

    // get current UAV data
    PositionActual::DataFields data = m_positionActual->getData();

    if (m_map)
    {
	connect(m_map, SIGNAL(zoomChanged(double)), this, SLOT(zoomChanged(double)));					// map zoom change signals
	connect(m_map, SIGNAL(OnCurrentPositionChanged(internals::PointLatLng)), this, SLOT(OnCurrentPositionChanged(internals::PointLatLng)));    // map poisition change signals
	connect(m_map, SIGNAL(OnTileLoadComplete()), this, SLOT(OnTileLoadComplete()));					// tile loading stop signals
	connect(m_map, SIGNAL(OnTileLoadStart()), this, SLOT(OnTileLoadStart()));					// tile loading start signals
	connect(m_map, SIGNAL(OnMapDrag()), this, SLOT(OnMapDrag()));							// map drag signals
	connect(m_map, SIGNAL(OnMapZoomChanged()), this, SLOT(OnMapZoomChanged()));					// map zoom changed
	connect(m_map, SIGNAL(OnMapTypeChanged(MapType::Types)), this, SLOT(OnMapTypeChanged(MapType::Types)));		// map type changed
	connect(m_map, SIGNAL(OnEmptyTileError(int, core::Point)), this, SLOT(OnEmptyTileError(int, core::Point)));	// tile error
	connect(m_map, SIGNAL(OnTilesStillToLoad(int)), this, SLOT(OnTilesStillToLoad(int)));				// tile loading signals
	connect(m_map, SIGNAL(WPNumberChanged(int const&,int const&,WayPointItem*)), this, SLOT(WPNumberChanged(int const&,int const&,WayPointItem*)));
    	connect(m_map, SIGNAL(WPValuesChanged(WayPointItem*)), this, SLOT(WPValuesChanged(WayPointItem*)));
    	connect(m_map, SIGNAL(WPInserted(int const&, WayPointItem*)), this, SLOT(WPInserted(int const&, WayPointItem*)));
    	connect(m_map, SIGNAL(WPDeleted(int const&)), this, SLOT(WPDeleted(int const&)));

	m_map->SetMaxZoom(19);									    // increase the maximum zoom level
	m_map->SetMouseWheelZoomType(internals::MouseWheelZoomType::MousePositionWithoutCenter);    // set how the mouse wheel zoom functions
	m_map->SetFollowMouse(true);								    // we want a contiuous mouse position reading
	m_map->SetCurrentPosition(internals::PointLatLng(data.Latitude, data.Longitude));	    // set the default map position
    }

    // **************
    // init the waypoint tree

    wayPoint_treeView_model = new QStandardItemModel();
    m_widget->treeViewWaypoints->setModel(wayPoint_treeView_model);


/*
    // test
//    wayPoint_treeView_model = new QStandardItemModel(5, 2);
    for (int r = 0; r < 5; r++)
    {
	for (int c = 0; c < 2; c++)
	{
	    QStandardItem *item = new QStandardItem(QString("Row:%0, Column:%1").arg(r).arg(c));

	    if (c == 0)
    	    {
		for (int i = 0; i < 3; i++)
		{
		    QStandardItem *child = new QStandardItem(QString("Item %0").arg(i));
		    child->setEditable(false);
		    item->appendRow(child);
		}
	    }

	    wayPoint_treeView_model->setItem(r, c, item);
	}
    }
    wayPoint_treeView_model->setHorizontalHeaderItem(0, new QStandardItem("Foo"));
    wayPoint_treeView_model->setHorizontalHeaderItem(1, new QStandardItem("Bar-Baz"));

//    m_widget->treeViewWaypoints->setModel(wayPoint_treeView_model);
*/





    // test only

    // create a waypoint group
    QStandardItem *item = new QStandardItem(tr("Camera shoot at the town hall"));
    for (int i = 1; i < 5; i++)
    {   // add some way points
	QStandardItem *child = new QStandardItem(QIcon(QString::fromUtf8(":/opmap/images/waypoint.png")), QString("Waypoint %0").arg(i));
	child->setEditable(true);
	item->appendRow(child);
    }
    wayPoint_treeView_model->appendRow(item);



    // create another waypoint group
    item = new QStandardItem(tr("Flight path 62"));
    for (int i = 1; i < 8; i++)
    {   // add some way points
	QStandardItem *child = new QStandardItem(QIcon(QString::fromUtf8(":/opmap/images/waypoint.png")), QString("Waypoint %0").arg(i));
	child->setEditable(true);
	item->appendRow(child);
    }
    wayPoint_treeView_model->appendRow(item);










    // **************
    // create the desired timers

    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(200);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(updatePosition()));
    m_updateTimer->start();

    m_statusUpdateTimer = new QTimer(this);
    m_statusUpdateTimer->setInterval(200);
    connect(m_statusUpdateTimer, SIGNAL(timeout()), this, SLOT(updateMousePos()));
    m_statusUpdateTimer->start();

    // **************
}

// *************************************************************************************
// destructor

OPMapGadgetWidget::~OPMapGadgetWidget()
{
    on_clearWayPointsAct_triggered();

    if (wayPoint_treeView_model) delete wayPoint_treeView_model;
    if (m_map_overlay_widget) delete m_map_overlay_widget;
    if (m_map) delete m_map;
    if (m_widget) delete m_widget;
}

// *************************************************************************************
// widget signals

void OPMapGadgetWidget::resizeEvent(QResizeEvent *event)
{
//    update();
    QWidget::resizeEvent(event);
}

void OPMapGadgetWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_map)
    {
//	mouse_lat_lon = m_map->currentMousePosition();			    // fetch the current mouse lat/longitude position
//	if (mouse_lat_lon != lat_lon)
//	{	// the mouse has moved
//	    mouse_lat_lon = lat_lon;
//
//	    QString coord_str = " " + QString::number(mouse_lat_lon.Lat(), 'f', 7) + "   " + QString::number(mouse_lat_lon.Lng(), 'f', 7) + " ";
//
//	    statusLabel.setText(coord_str);
//	    widget->labelStatus->setText(coord_str);
//	}
    }

    if (event->buttons() & Qt::LeftButton)
    {
	 QPoint pos = event->pos();
    }
}

void OPMapGadgetWidget::contextMenuEvent(QContextMenuEvent *event)
{
    if (!m_map)
	return;	// we don't appear to have a map to work with!

    if (event->reason() != QContextMenuEvent::Mouse)
	return;	// not a mouse click event

    QPoint p = m_map->mapFromGlobal(QCursor::pos());

    if (!m_map->contentsRect().contains(p))
	return;	// the mouse click was not on the map

    // find out if we have a waypoint under the mouse cursor
    QGraphicsItem *item = m_map->itemAt(p);
    m_mouse_waypoint = qgraphicsitem_cast<mapcontrol::WayPointItem *>(item);

    // ****************
    // create the popup menu

    QMenu menu(this);

    QMenu zoomMenu(tr("&Zoom ") + "(" + QString::number(m_map->Zoom()) + ")", this);
    for (int i = 0; i < zoomAct.count(); i++)
	zoomMenu.addAction(zoomAct.at(i));

    menu.addAction(closeAct);

    menu.addSeparator();

    menu.addAction(reloadAct);

    menu.addSeparator();

    menu.addAction(findPlaceAct);

    menu.addSeparator();

    menu.addAction(showCompassAct);

    menu.addSeparator()->setText(tr("Zoom"));

    menu.addAction(zoomInAct);
    menu.addAction(zoomOutAct);
    menu.addMenu(&zoomMenu);

    menu.addSeparator()->setText(tr("Position"));

    menu.addAction(goMouseClickAct);
    menu.addAction(goHomeAct);
    menu.addAction(goUAVAct);

    menu.addSeparator()->setText(tr("Follow"));

    menu.addAction(followUAVpositionAct);
    menu.addAction(followUAVheadingAct);

    menu.addSeparator()->setText(tr("Waypoints"));

    menu.addAction(wayPointEditorAct);
    menu.addAction(addWayPointAct);
    if (m_mouse_waypoint)
    {	// we have a waypoint under the mouse
	menu.addAction(editWayPointAct);

	bool locked = (m_mouse_waypoint->flags() & QGraphicsItem::ItemIsMovable) == 0;

	lockWayPointAct->setChecked(locked);
	menu.addAction(lockWayPointAct);

	if (!locked)
	    menu.addAction(deleteWayPointAct);
    }

    m_waypoint_list_mutex.lock();
	if (m_waypoint_list.count() > 0)
	    menu.addAction(clearWayPointsAct);	// we have waypoints
    m_waypoint_list_mutex.unlock();

//    menu.addSeparator();

    menu.exec(event->globalPos());  // popup the menu

    // ****************
}

void OPMapGadgetWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape) // ESC
    {
    }
    else
    if (event->key() == Qt::Key_F1) // F1
    {
    }
    else
    if (event->key() == Qt::Key_F2) // F2
    {
    }
    else
    if (event->key() == Qt::Key_Up)
    {
    }
    else
    if (event->key() == Qt::Key_Down)
    {
    }
    else
    if (event->key() == Qt::Key_Left)
    {
    }
    else
    if (event->key() == Qt::Key_Right)
    {
    }
    else
    if (event->key() == Qt::Key_PageUp)
    {
    }
    else
    if (event->key() == Qt::Key_PageDown)
    {
    }
    else
    {
	qDebug() << event->key() << endl;
    }
}

// *************************************************************************************
// timer signals

void OPMapGadgetWidget::updatePosition()
{
    PositionActual::DataFields data = m_positionActual->getData();				// get current UAV data
    internals::PointLatLng uav_pos = internals::PointLatLng(data.Latitude, data.Longitude);	// current UAV position
    double uav_heading = data.Heading;								// current UAV heading
    double uav_height_meters = data.Altitude;							// current UAV height
    double uav_ground_speed_meters_per_second = data.Groundspeed;				// current UAV ground speed

    // display the UAV lat/lon position
    QString str =   " lat: " + QString::number(uav_pos.Lat(), 'f', 7) +
		    "   lon: " + QString::number(uav_pos.Lng(), 'f', 7) +
		    "   " + QString::number(uav_heading, 'f', 1) + "deg" +
		    "   " + QString::number(uav_height_meters, 'f', 1) + "m" +
		    "   " + QString::number(uav_ground_speed_meters_per_second, 'f', 1) + "m/s";
    if (m_widget) m_widget->labelUAVPos->setText(str);

    if (m_map && followUAVpositionAct && followUAVheadingAct)
    {
	if (followUAVpositionAct->isChecked())
	{
	    internals::PointLatLng map_pos = m_map->CurrentPosition();					// current MAP position
	    double map_heading = m_map->Rotate();

	    if (map_pos != uav_pos || map_heading != uav_heading)
	    {
		m_map->SetCurrentPosition(uav_pos);							// keep the map centered on the UAV

		if (followUAVheadingAct->isChecked())
		    m_map->SetRotate(-uav_heading);							// rotate the map to match the uav heading
		else
		if (m_map->Rotate() != 0)
		    m_map->SetRotate(0);								// reset the rotation to '0'
	    }
	}
    }
}

void OPMapGadgetWidget::updateMousePos()
{
    internals::PointLatLng lat_lon = m_map->currentMousePosition();				// fetch the current lat/lon mouse position

    if (mouse_lat_lon != lat_lon)
    {	// the mouse has moved
	mouse_lat_lon = lat_lon;

	QString str = " " + QString::number(mouse_lat_lon.Lat(), 'f', 7) + "   " + QString::number(mouse_lat_lon.Lng(), 'f', 7);
	if (m_widget) m_widget->labelMousePos->setText(str);
    }
}

// *************************************************************************************
// map signals

void OPMapGadgetWidget::zoomChanged(double zoom)
{
    int i_zoom = (int)(zoom + 0.5);
    if (i_zoom < 2 || i_zoom > 19) return;

    if (m_widget)
    {
//	m_widget->labelZoom->setText(" " + QString::number(zoom));
	if (m_widget->horizontalSliderZoom->value() != i_zoom)
	    m_widget->horizontalSliderZoom->setValue(i_zoom);
    }

    if (i_zoom - 2 < zoomAct.count())
	zoomAct.at(i_zoom - 2)->setChecked(true);
}

void OPMapGadgetWidget::OnMapDrag()
{
}

void OPMapGadgetWidget::OnCurrentPositionChanged(internals::PointLatLng point)
{
    if (m_widget)
    {
	QString coord_str = " " + QString::number(point.Lat(), 'f', 7) + "   " + QString::number(point.Lng(), 'f', 7) + " ";
	m_widget->labelMapPos->setText(coord_str);
    }
}

void OPMapGadgetWidget::OnTilesStillToLoad(int number)
{
    if (m_widget)
    {
//	if (prev_tile_number < number || m_widget->progressBarMap->maximum() < number)
//	    m_widget->progressBarMap->setMaximum(number);

	if (m_widget->progressBarMap->maximum() < number)
	    m_widget->progressBarMap->setMaximum(number);

	m_widget->progressBarMap->setValue(m_widget->progressBarMap->maximum() - number);	// update the progress bar

//	m_widget->labelNumTilesToLoad->setText(" " + QString::number(number));

	prev_tile_number = number;
    }
}

void OPMapGadgetWidget::OnTileLoadStart()
{
    if (m_widget)
	m_widget->progressBarMap->setVisible(true);
}

void OPMapGadgetWidget::OnTileLoadComplete()
{
    if (m_widget)
	m_widget->progressBarMap->setVisible(false);
}

void OPMapGadgetWidget::OnMapZoomChanged()
{
    // to do
}

void OPMapGadgetWidget::OnMapTypeChanged(MapType::Types type)
{
    // to do
}

void OPMapGadgetWidget::OnEmptyTileError(int zoom, core::Point pos)
{
    // to do
}

void OPMapGadgetWidget::WPNumberChanged(int const &oldnumber, int const &newnumber, WayPointItem *waypoint)
{
    // to do
}

void OPMapGadgetWidget::WPValuesChanged(WayPointItem *waypoint)
{
    // to do
}

void OPMapGadgetWidget::WPInserted(int const &number, WayPointItem *waypoint)
{
    // to do
}

void OPMapGadgetWidget::WPDeleted(int const &number)
{
    // to do
}

// *************************************************************************************
// user control signals

void OPMapGadgetWidget::on_comboBoxFindPlace_returnPressed()
{
    QString place = m_widget->comboBoxFindPlace->currentText().simplified();
    if (place.isNull() || place.isEmpty()) return;

    if (!findPlaceWordList.contains(place, Qt::CaseInsensitive))
    {
	findPlaceWordList << place;	// add the new word into the history list
/*
	m_widget->comboBoxFindPlace->setCompleter(NULL);
	delete findPlaceCompleter;
	findPlaceCompleter = new QCompleter(findPlaceWordList, this);
	findPlaceCompleter->setCaseSensitivity(Qt::CaseInsensitive);
	findPlaceCompleter->setCompletionMode(QCompleter::PopupCompletion);
	findPlaceCompleter->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
	m_widget->comboBoxFindPlace->setCompleter(findPlaceCompleter);
*/
    }

    if (!m_map) return;

    core::GeoCoderStatusCode::Types x = m_map->SetCurrentPositionByKeywords(place);
    QString returned_text = mapcontrol::Helper::StrFromGeoCoderStatusCode(x);

    QMessageBox::information(this, tr("OpenPilot GCS"), returned_text, QMessageBox::Ok);
}

void OPMapGadgetWidget::on_toolButtonFindPlace_clicked()
{
    m_widget->comboBoxFindPlace->setFocus();
    on_comboBoxFindPlace_returnPressed();
}

void OPMapGadgetWidget::on_toolButtonZoomP_clicked()
{
    zoomIn();
}

void OPMapGadgetWidget::on_toolButtonZoomM_clicked()
{
    zoomOut();
}

void OPMapGadgetWidget::on_toolButtonWaypointsTreeViewShowHide_clicked()
{
    if (m_widget)
    {
	m_widget->treeViewWaypoints->setVisible(!m_widget->treeViewWaypoints->isVisible());

	if (m_widget->treeViewWaypoints->isVisible())
	    m_widget->toolButtonWaypointsTreeViewShowHide->setIcon(QIcon(QString::fromUtf8(":/core/images/prev.png")));
	else
	    m_widget->toolButtonWaypointsTreeViewShowHide->setIcon(QIcon(QString::fromUtf8(":/core/images/next.png")));
    }
}

void OPMapGadgetWidget::on_toolButtonFlightControlsShowHide_clicked()
{
    if (m_widget)
    {
	m_widget->frameFlightControls->setVisible(!m_widget->frameFlightControls->isVisible());

	if (m_widget->frameFlightControls->isVisible())
	    m_widget->toolButtonFlightControlsShowHide->setIcon(QIcon(QString::fromUtf8(":/core/images/next.png")));
	else
	    m_widget->toolButtonFlightControlsShowHide->setIcon(QIcon(QString::fromUtf8(":/core/images/prev.png")));
    }
}

void OPMapGadgetWidget::on_toolButtonMapHome_clicked()
{
    followUAVpositionAct->setChecked(false);
}

void OPMapGadgetWidget::on_toolButtonMapUAV_clicked()
{
    followUAVpositionAct->toggle();
}

void OPMapGadgetWidget::on_horizontalSliderZoom_sliderMoved(int position)
{
    setZoom(position);
}

void OPMapGadgetWidget::on_toolButtonHome_clicked()
{
    // to do
}


void OPMapGadgetWidget::on_toolButtonPrevWaypoint_clicked()
{
    // to do
}

void OPMapGadgetWidget::on_toolButtonNextWaypoint_clicked()
{
    // to do
}

void OPMapGadgetWidget::on_toolButtonHoldPosition_clicked()
{
    // to do
}

void OPMapGadgetWidget::on_toolButtonGo_clicked()
{
    // to do
}

void OPMapGadgetWidget::on_toolButtonAddWaypoint_clicked()
{
    addWayPointAct->trigger();
}

void OPMapGadgetWidget::on_toolButtonWaypointEditor_clicked()
{
    wayPointEditorAct->trigger();
}

void OPMapGadgetWidget::on_treeViewWaypoints_clicked(QModelIndex index)
{
    if (!wayPoint_treeView_model) return;

    QStandardItem *item = wayPoint_treeView_model->itemFromIndex(index);
    if (!item) return;

    // to do
}

// *************************************************************************************
// public functions

void OPMapGadgetWidget::zoomIn()
{
    if (m_map)
	m_map->SetZoom(m_map->Zoom() + 1);
}

void OPMapGadgetWidget::zoomOut()
{
    if (m_map)
	m_map->SetZoom(m_map->Zoom() - 1);
}

void OPMapGadgetWidget::setZoom(int value)
{
    if (m_map)
    {
	internals::MouseWheelZoomType::Types zoom_type = m_map->GetMouseWheelZoomType();
	m_map->SetMouseWheelZoomType(internals::MouseWheelZoomType::ViewCenter);

	m_map->SetZoom(value);

	m_map->SetMouseWheelZoomType(zoom_type);
    }
}

void OPMapGadgetWidget::setPosition(QPointF pos)
{
    if (m_map)
	m_map->SetCurrentPosition(internals::PointLatLng(pos.y(), pos.x()));
}

void OPMapGadgetWidget::setMapProvider(QString provider)
{
    if (m_map)
	m_map->SetMapType(mapcontrol::Helper::MapTypeFromString(provider));
}

void OPMapGadgetWidget::setAccessMode(QString accessMode)
{
    if (m_map)
	m_map->configuration->SetAccessMode(mapcontrol::Helper::AccessModeFromString(accessMode));
}

void OPMapGadgetWidget::setUseOpenGL(bool useOpenGL)
{
    if (m_map)
	m_map->SetUseOpenGL(useOpenGL);
}

void OPMapGadgetWidget::setShowTileGridLines(bool showTileGridLines)
{
    if (m_map)
	m_map->SetShowTileGridLines(showTileGridLines);
}

void OPMapGadgetWidget::setUseMemoryCache(bool useMemoryCache)
{
    if (m_map)
	m_map->configuration->SetUseMemoryCache(useMemoryCache);
}

void OPMapGadgetWidget::setCacheLocation(QString cacheLocation)
{
    cacheLocation = cacheLocation.simplified();	// remove any surrounding spaces

    if (cacheLocation.isEmpty()) return;

    #if defined(Q_WS_WIN)
	if (!cacheLocation.endsWith('/')) cacheLocation += '/';
    #elif defined(Q_WS_X11)
	if (!cacheLocation.endsWith(QDir::separator())) cacheLocation += QDir::separator();
    #elif defined(Q_WS_MAC)
	if (!cacheLocation.endsWith(QDir::separator())) cacheLocation += QDir::separator();
    #endif

    QDir dir;
    if (!dir.exists(cacheLocation))
	if (!dir.mkpath(cacheLocation))
	    return;

    qDebug() << "map cache dir: " << cacheLocation;

    if (m_map)
	m_map->configuration->SetCacheLocation(cacheLocation);
}

// *************************************************************************************
// Context menu stuff

void OPMapGadgetWidget::createActions()
{
    // ***********************
    // create menu actions

    closeAct = new QAction(tr("&Close menu"), this);
//    closeAct->setShortcuts(QKeySequence::New);
    closeAct->setStatusTip(tr("Close the context menu"));

    reloadAct = new QAction(tr("&Reload map"), this);
    reloadAct->setShortcut(tr("F5"));
    reloadAct->setStatusTip(tr("Reload the map tiles"));
    connect(reloadAct, SIGNAL(triggered()), this, SLOT(on_reloadAct_triggered()));

    findPlaceAct = new QAction(tr("&Find place"), this);
    findPlaceAct->setShortcut(tr("Ctrl+F"));
    findPlaceAct->setStatusTip(tr("Find a location"));
    connect(findPlaceAct, SIGNAL(triggered()), this, SLOT(on_findPlaceAct_triggered()));

    showCompassAct = new QAction(tr("Show compass"), this);
//    showCompassAct->setShortcut(tr("Ctrl+M"));
    showCompassAct->setStatusTip(tr("Show/Hide the map compass"));
    showCompassAct->setCheckable(true);
    showCompassAct->setChecked(true);
    connect(showCompassAct, SIGNAL(toggled(bool)), this, SLOT(on_showCompassAct_toggled(bool)));

    zoomInAct = new QAction(tr("Zoom &In"), this);
    zoomInAct->setShortcut(Qt::Key_PageUp);
    zoomInAct->setStatusTip(tr("Zoom the map in"));
    connect(zoomInAct, SIGNAL(triggered()), this, SLOT(on_goZoomInAct_triggered()));

    zoomOutAct = new QAction(tr("Zoom &Out"), this);
    zoomOutAct->setShortcut(Qt::Key_PageDown);
    zoomOutAct->setStatusTip(tr("Zoom the map out"));
    connect(zoomOutAct, SIGNAL(triggered()), this, SLOT(on_goZoomOutAct_triggered()));

    goMouseClickAct = new QAction(tr("Go to where you right clicked the mouse"), this);
    goMouseClickAct->setStatusTip(tr("Center the map onto where you right clicked the mouse"));
    connect(goMouseClickAct, SIGNAL(triggered()), this, SLOT(on_goMouseClickAct_triggered()));

    goHomeAct = new QAction(tr("Go to &Home location"), this);
    goHomeAct->setShortcut(tr("Ctrl+H"));
    goHomeAct->setStatusTip(tr("Center the map onto the home location"));
    connect(goHomeAct, SIGNAL(triggered()), this, SLOT(on_goHomeAct_triggered()));

    goUAVAct = new QAction(tr("Go to &UAV location"), this);
    goUAVAct->setShortcut(tr("Ctrl+U"));
    goUAVAct->setStatusTip(tr("Center the map onto the UAV location"));
    connect(goUAVAct, SIGNAL(triggered()), this, SLOT(on_goUAVAct_triggered()));

    followUAVpositionAct = new QAction(tr("Follow UAV position"), this);
    followUAVpositionAct->setShortcut(tr("Ctrl+F"));
    followUAVpositionAct->setStatusTip(tr("Keep the map centered onto the UAV"));
    followUAVpositionAct->setCheckable(true);
    followUAVpositionAct->setChecked(false);
    connect(followUAVpositionAct, SIGNAL(toggled(bool)), this, SLOT(on_followUAVpositionAct_toggled(bool)));

    followUAVheadingAct = new QAction(tr("Follow UAV heading"), this);
    followUAVheadingAct->setShortcut(tr("Ctrl+F"));
    followUAVheadingAct->setStatusTip(tr("Keep the map rotation to the UAV heading"));
    followUAVheadingAct->setCheckable(true);
    followUAVheadingAct->setChecked(true);
    connect(followUAVheadingAct, SIGNAL(toggled(bool)), this, SLOT(on_followUAVheadingAct_toggled(bool)));

    wayPointEditorAct = new QAction(tr("&Way point editor"), this);
    wayPointEditorAct->setShortcut(tr("Ctrl+W"));
    wayPointEditorAct->setStatusTip(tr("Open the way-point editor"));
    connect(wayPointEditorAct, SIGNAL(triggered()), this, SLOT(on_openWayPointEditorAct_triggered()));

    addWayPointAct = new QAction(tr("&Add waypoint"), this);
    addWayPointAct->setShortcut(tr("Ctrl+A"));
    addWayPointAct->setStatusTip(tr("Add waypoint"));
    connect(addWayPointAct, SIGNAL(triggered()), this, SLOT(on_addWayPointAct_triggered()));

    editWayPointAct = new QAction(tr("&Edit waypoint"), this);
    editWayPointAct->setShortcut(tr("Ctrl+E"));
    editWayPointAct->setStatusTip(tr("Edit waypoint"));
    connect(editWayPointAct, SIGNAL(triggered()), this, SLOT(on_editWayPointAct_triggered()));

    lockWayPointAct = new QAction(tr("&Lock waypoint"), this);
    lockWayPointAct->setStatusTip(tr("Lock/Unlock a waypoint"));
    lockWayPointAct->setCheckable(true);
    lockWayPointAct->setChecked(false);
    connect(lockWayPointAct, SIGNAL(triggered()), this, SLOT(on_lockWayPointAct_triggered()));

    deleteWayPointAct = new QAction(tr("&Delete waypoint"), this);
    deleteWayPointAct->setShortcut(tr("Ctrl+D"));
    deleteWayPointAct->setStatusTip(tr("Delete waypoint"));
    connect(deleteWayPointAct, SIGNAL(triggered()), this, SLOT(on_deleteWayPointAct_triggered()));

    clearWayPointsAct = new QAction(tr("&Clear waypoints"), this);
    clearWayPointsAct->setShortcut(tr("Ctrl+C"));
    clearWayPointsAct->setStatusTip(tr("Clear waypoints"));
    connect(clearWayPointsAct, SIGNAL(triggered()), this, SLOT(on_clearWayPointsAct_triggered()));

    zoomActGroup = new QActionGroup(this);
    connect(zoomActGroup, SIGNAL(triggered(QAction *)), this, SLOT(on_zoomActGroup_triggered(QAction *)));
    zoomAct.clear();
    for (int i = 2; i <= 19; i++)
    {
	QAction *zoom_act = new QAction(QString::number(i), zoomActGroup);
	zoom_act->setCheckable(true);
	zoom_act->setData(i);
	zoomAct.append(zoom_act);
    }

    // ***********************
}

void OPMapGadgetWidget::on_reloadAct_triggered()
{
    if (m_map)
	m_map->ReloadMap();
}

void OPMapGadgetWidget::on_findPlaceAct_triggered()
{
    m_widget->comboBoxFindPlace->setFocus();	// move focus to the 'find place' text box

/*
    bool ok;
    QString place = QInputDialog::getText(this, tr("OpenPilot GCS"), tr("Find place"), QLineEdit::Normal, QString::null, &ok);
    place = place.simplified();
    if (!ok || place.isNull() || place.isEmpty()) return;

    if (!findPlaceWordList.contains(place, Qt::CaseInsensitive))
    {
	findPlaceWordList += place;	// add the new word into the history list
    }

    if (!m_map) return;

    core::GeoCoderStatusCode::Types x = m_map->SetCurrentPositionByKeywords(place);
    QString returned_text = mapcontrol::Helper::StrFromGeoCoderStatusCode(x);

    QMessageBox::information(this, tr("OpenPilot GCS"), returned_text, QMessageBox::Ok);
*/
}

void OPMapGadgetWidget::on_showCompassAct_toggled(bool show_compass)
{
    if (m_map)
	m_map->SetShowCompass(show_compass);
}

void OPMapGadgetWidget::on_goZoomInAct_triggered()
{
    if (m_map)
	setZoom(m_map->Zoom() + 1);
}

void OPMapGadgetWidget::on_goZoomOutAct_triggered()
{
    if (m_map)
	setZoom(m_map->Zoom() - 1);
}

void OPMapGadgetWidget::on_zoomActGroup_triggered(QAction *action)
{
    if (!action) return;

    int zoom = action->data().toInt();
    if (zoom < 2 || zoom > 19) return;

    setZoom(zoom);
}

void OPMapGadgetWidget::on_goMouseClickAct_triggered()
{
    if (m_map)
	m_map->SetCurrentPosition(m_map->currentMousePosition());   // center the map onto the mouse position
}

void OPMapGadgetWidget::on_goHomeAct_triggered()
{
    followUAVpositionAct->setChecked(false);
}

void OPMapGadgetWidget::on_goUAVAct_triggered()
{
    PositionActual::DataFields data = m_positionActual->getData();				// get current UAV data

    if (m_map)
    {
	internals::PointLatLng uav_pos = internals::PointLatLng(data.Latitude, data.Longitude);	// current UAV position
	internals::PointLatLng map_pos = m_map->CurrentPosition();				// current MAP position
	if (map_pos != uav_pos) m_map->SetCurrentPosition(uav_pos);				// center the map onto the UAV
    }
}

void OPMapGadgetWidget::on_followUAVpositionAct_toggled(bool checked)
{
    if (m_widget)
    {
	if (m_widget->toolButtonMapUAV->isChecked() != followUAVpositionAct->isChecked())
	    m_widget->toolButtonMapUAV->setChecked(followUAVpositionAct->isChecked());

	if (m_map)
	    m_map->SetRotate(0);				    				// reset the rotation to '0'
    }
}

void OPMapGadgetWidget::on_followUAVheadingAct_toggled(bool checked)
{
    if (!checked && m_map)
	m_map->SetRotate(0);									// reset the rotation to '0'
}

void OPMapGadgetWidget::on_openWayPointEditorAct_triggered()
{
    waypoint_editor_dialog.show();
}

void OPMapGadgetWidget::on_addWayPointAct_triggered()
{
    if (m_map)
    {
	m_waypoint_list_mutex.lock();

	    // create a waypoint
	    t_waypoint waypoint;
	    waypoint.item = m_map->WPCreate();
	    waypoint.time_seconds = 0;
	    waypoint.hold_time_seconds = 0;

	    // and remember it
	    m_waypoint_list.append(waypoint);

	m_waypoint_list_mutex.unlock();
    }

    // to do
}

void OPMapGadgetWidget::on_editWayPointAct_triggered()
{
    if (!m_mouse_waypoint) return;

    // to do

    waypoint_edit_dialog.show();

    m_mouse_waypoint = NULL;
}

void OPMapGadgetWidget::on_lockWayPointAct_triggered()
{
    if (!m_mouse_waypoint) return;

    bool locked = (m_mouse_waypoint->flags() & QGraphicsItem::ItemIsMovable) == 0;
    m_mouse_waypoint->setFlag(QGraphicsItem::ItemIsMovable, locked);

    m_mouse_waypoint = NULL;
}

void OPMapGadgetWidget::on_deleteWayPointAct_triggered()
{
    if (!m_mouse_waypoint) return;

    bool locked = (m_mouse_waypoint->flags() & QGraphicsItem::ItemIsMovable) == 0;

    if (locked) return;	// waypoint is locked

    m_waypoint_list_mutex.lock();

	for (int i = 0; i < m_waypoint_list.count(); i++)
	{
	    t_waypoint waypoint = m_waypoint_list.at(i);
	    if (waypoint.item != m_mouse_waypoint) continue;

	    // delete the waypoint from the map
	    if (m_map) m_map->WPDelete(waypoint.item);

	    // delete the waypoint from our local waypoint list
	    m_waypoint_list.removeAt(i);

	    break;
	}

    m_waypoint_list_mutex.unlock();

    m_mouse_waypoint = NULL;
}

void OPMapGadgetWidget::on_clearWayPointsAct_triggered()
{
    m_waypoint_list_mutex.lock();
	if (m_map) m_map->WPDeleteAll();
	m_waypoint_list.clear();
    m_waypoint_list_mutex.unlock();
}

// *************************************************************************************
