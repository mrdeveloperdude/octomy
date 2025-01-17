/*
*
* This file is part of QMapControl,
* an open-source cross-platform map widget
*
* Copyright (C) 2007 - 2008 Kai Winter
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with QMapControl. If not, see <http://www.gnu.org/licenses/>.
*
* Contact e-mail: kaiwinter@gmx.de
* Program URL   : http://qmapcontrol.sourceforge.net/
*
*/

#include "Layer.hpp"

#include "uptime/MethodGate.hpp"

namespace qmapcontrol
{

Layer::Layer()
	:   visible(true)
	, mylayertype(MapLayer)
	, mapAdapter(nullptr)
	, takeevents(true)
	, myoffscreenViewport(QRect(0,0,0,0))
	, m_ImageManager(nullptr)
{
	OC_METHODGATE();
}

Layer::Layer(QString layername, QSharedPointer<MapAdapter> mapadapter, enum LayerType layertype, bool takeevents)
	:   visible(true)
	, mylayername(layername)
	, mylayertype(layertype)
	, mapAdapter(mapadapter)
	, takeevents(takeevents)
	, myoffscreenViewport(QRect(0,0,0,0))
	, m_ImageManager(nullptr)
{
	OC_METHODGATE();
}

Layer::~Layer()
{
	OC_METHODGATE();
}

void Layer::setSize(QSize size)
{
	OC_METHODGATE();
	this->size = size;
	screenmiddle = QPoint(size.width()/2, size.height()/2);
	emit(updateRequest());
}

QString Layer::layername() const
{
	OC_METHODGATE();
	return mylayername;
}

QSharedPointer<MapAdapter> Layer::mapadapter()
{
	OC_METHODGATE();
	return mapAdapter;
}

void Layer::setVisible(bool visible)
{
	OC_METHODGATE();
	this->visible = visible;
	emit(updateRequest());
}

QList<Geometry*>& Layer::getGeometries()
{
	OC_METHODGATE();
	return geometries;
}

bool Layer::containsGeometry( Geometry* geometry )
{
	OC_METHODGATE();
	return geometry && geometries.contains( geometry );
}

void Layer::sendGeometryToFront(Geometry *geometry)
{
	OC_METHODGATE();
	if ( !geometry || !geometries.contains( geometry ) ) {
		return;
	}
	geometries.removeAll(geometry);
	geometries.prepend(geometry);
	emit(updateRequest());
}

void Layer::sendGeometryToBack(Geometry *geometry)
{
	OC_METHODGATE();
	if ( !geometry || !geometries.contains( geometry ) ) {
		return;
	}
	geometries.removeAll(geometry);
	geometries.append(geometry);
	emit(updateRequest());
}

void Layer::addGeometry(Geometry* geom)
{
	OC_METHODGATE();
	if ( !geom || containsGeometry( geom ) ) {
		return;
	}

	geometries.append(geom);
	emit(updateRequest(geom->boundingBox()));
	//a geometry can request a redraw, e.g. when its position has been changed
	connect(geom, SIGNAL(updateRequest(QRectF)),
			this, SIGNAL(updateRequest(QRectF)));
}

void Layer::removeGeometry(Geometry* geometry, bool qDeleteObject)
{
	OC_METHODGATE();
	if ( !geometry ) {
		return;
	}

	QRectF boundingBox = geometry->boundingBox();

	foreach( Geometry* geo, geometries ) {
		if ( geo && geo == geometry ) {
			disconnect(geometry);
			geometries.removeAll( geometry );
			if (qDeleteObject) {
				delete geo;
				geo = nullptr;
			}
		}
	}
	emit(updateRequest(boundingBox));
}

void Layer::clearGeometries( bool qDeleteObject )
{
	OC_METHODGATE();
	foreach(Geometry *geometry, geometries) {
		disconnect(geometry);
		if ( qDeleteObject ) {
			delete geometry;
			geometry = nullptr;
		}
	}
	geometries.clear();
}

bool Layer::isVisible() const
{
	OC_METHODGATE();
	return visible;
}
void Layer::zoomIn() const
{
	OC_METHODGATE();
	if(!mapAdapter.isNull()) {
		mapAdapter->zoom_in();
	}
}

void Layer::zoomOut() const
{
	OC_METHODGATE();
	if(!mapAdapter.isNull()) {
		mapAdapter->zoom_out();
	}
}

void Layer::mouseEvent(const QMouseEvent* evnt, const QPoint mapmiddle_px)
{
	OC_METHODGATE();
	if (takesMouseEvents()) {
		if ( geometries.size() > 0 &&
				evnt->button() == Qt::LeftButton &&
				evnt->type() == QEvent::MouseButtonPress) {
			// check for collision
			QPointF c;
			auto pos=evnt->pos();
			if(!mapAdapter.isNull()) {
				c= mapAdapter->displayToCoordinate(QPoint(pos.x()-screenmiddle.x()+mapmiddle_px.x(),
												   pos.y()-screenmiddle.y()+mapmiddle_px.y()));
			}
			Point tmppoint(c.x(), c.y());
			for(QList<Geometry*>::const_iterator iter = geometries.begin(); iter != geometries.end(); ++iter) {
				Geometry *geo = *iter;
				if (geo && geo->isVisible() && geo->Touches(&tmppoint, mapAdapter)) {
					emit(geometryClicked(geo, pos));
				}
			}
		}
	}
}

bool Layer::takesMouseEvents() const
{
	OC_METHODGATE();
	return takeevents;
}

void Layer::drawYourImage(QPainter* painter, const QPoint mapmiddle_px) const
{
	OC_METHODGATE();
	if (mylayertype == MapLayer) {
		_draw(painter, mapmiddle_px);
	}

	drawYourGeometries(painter, QPoint(mapmiddle_px.x()-screenmiddle.x(), mapmiddle_px.y()-screenmiddle.y()), myoffscreenViewport);
}

void Layer::drawYourGeometries(QPainter* painter, const QPoint mapmiddle_px, QRect viewport) const
{
	OC_METHODGATE();
	QPoint offset;
	if (mylayertype == MapLayer) {
		offset = mapmiddle_px;
	} else {
		offset = mapmiddle_px-screenmiddle;
	}

	painter->translate(-mapmiddle_px+screenmiddle);

	for(QList<Geometry*>::const_iterator iter = geometries.begin(); iter != geometries.end(); ++iter) {
		Geometry *geo = *iter;
		geo->draw(painter, mapAdapter, viewport, offset);
	}
	painter->translate(mapmiddle_px-screenmiddle);

}

void Layer::_draw(QPainter* painter, const QPoint mapmiddle_px) const
{
	OC_METHODGATE();
	if ( nullptr == m_ImageManager) {
		return;
	}

	if(mapAdapter.isNull()) {
		return;
	}

	// screen middle...
	int tilesize = mapAdapter->tilesize();
	int cross_x = int(mapmiddle_px.x())%tilesize; // position on middle tile
	int cross_y = int(mapmiddle_px.y())%tilesize;

	// calculate how many surrounding tiles have to be drawn to fill the display
	int space_left = screenmiddle.x() - cross_x;
	int tiles_left = space_left/tilesize;
	if (space_left>0) {
		tiles_left+=1;
	}

	int space_above = screenmiddle.y() - cross_y;
	int tiles_above = space_above/tilesize;
	if (space_above>0) {
		tiles_above+=1;
	}

	int space_right = screenmiddle.x() - (tilesize-cross_x);
	int tiles_right = space_right/tilesize;
	if (space_right>0) {
		tiles_right+=1;
	}

	int space_bottom = screenmiddle.y() - (tilesize-cross_y);
	int tiles_bottom = space_bottom/tilesize;
	if (space_bottom>0) {
		tiles_bottom+=1;
	}

	//int tiles_displayed = 0;
	int mapmiddle_tile_x = mapmiddle_px.x()/tilesize;
	int mapmiddle_tile_y = mapmiddle_px.y()/tilesize;

	const QPoint from = QPoint((-tiles_left+mapmiddle_tile_x)*tilesize, (-tiles_above+mapmiddle_tile_y)*tilesize);
	const QPoint to = QPoint((tiles_right+mapmiddle_tile_x+1)*tilesize, (tiles_bottom+mapmiddle_tile_y+1)*tilesize);

	myoffscreenViewport = QRect(from, to);

	// for the EmptyMapAdapter no tiles should be loaded and painted.
	if (mapAdapter->host().isEmpty()) {
		return;
	}

	//grab the middle tile (under the pointer) first
	if (mapAdapter->isTileValid(mapmiddle_tile_x, mapmiddle_tile_y, mapAdapter->currentZoom())) {
		painter->drawPixmap(-cross_x+size.width(),
							-cross_y+size.height(),
							m_ImageManager->getImage(mapAdapter->host(), mapAdapter->query(mapmiddle_tile_x, mapmiddle_tile_y, mapAdapter->currentZoom())) );
	}

	for (int i=-tiles_left+mapmiddle_tile_x; i<=tiles_right+mapmiddle_tile_x; ++i) {
		for (int j=-tiles_above+mapmiddle_tile_y; j<=tiles_bottom+mapmiddle_tile_y; ++j) {
			// check if image is valid
			if (!(i==mapmiddle_tile_x && j==mapmiddle_tile_y)) {
				if (mapAdapter->isTileValid(i, j, mapAdapter->currentZoom())) {
					painter->drawPixmap(((i-mapmiddle_tile_x)*tilesize)-cross_x+size.width(),
										((j-mapmiddle_tile_y)*tilesize)-cross_y+size.height(),
										m_ImageManager->getImage(mapAdapter->host(), mapAdapter->query(i, j, mapAdapter->currentZoom())));
				}
			}
		}
	}

	bool enabledPrefetch = false;
	if ( enabledPrefetch ) {
		// Prefetch the next set of rows/column tiles (ready for when the user starts panning).
		const int prefetch_tile_left = tiles_left - 1;
		const int prefetch_tile_top = tiles_above - 1;
		const int prefetch_tile_right = tiles_right + 1;
		const int prefetch_tile_bottom = tiles_bottom + 1;

		// Fetch the top/bottom rows
		for (int i=prefetch_tile_left; i<=prefetch_tile_right; ++i) {
			if (mapAdapter->isTileValid(i, prefetch_tile_top, mapAdapter->currentZoom())) {
				m_ImageManager->prefetchImage(mapAdapter->host(), mapAdapter->query(i, prefetch_tile_top, mapAdapter->currentZoom()));
			}
			if (mapAdapter->isTileValid(i, prefetch_tile_bottom, mapAdapter->currentZoom())) {
				m_ImageManager->prefetchImage(mapAdapter->host(), mapAdapter->query(i, prefetch_tile_bottom, mapAdapter->currentZoom()));
			}
		}

		for (int i=prefetch_tile_top; i<=prefetch_tile_bottom; ++i) {
			if (mapAdapter->isTileValid(prefetch_tile_left, i, mapAdapter->currentZoom())) {
				m_ImageManager->prefetchImage(mapAdapter->host(), mapAdapter->query(prefetch_tile_left, i, mapAdapter->currentZoom()));
			}
			if (mapAdapter->isTileValid(prefetch_tile_right, i, mapAdapter->currentZoom())) {
				m_ImageManager->prefetchImage(mapAdapter->host(), mapAdapter->query(prefetch_tile_right, i, mapAdapter->currentZoom()));
			}
		}
	}
}

QRect Layer::offscreenViewport() const
{
	OC_METHODGATE();
	return myoffscreenViewport;
}

void Layer::moveWidgets(const QPoint mapmiddle_px) const
{
	OC_METHODGATE();
	for( Geometry* geometry: geometries ) {
		if (geometry->GeometryType == "Point") {
			Point* point = static_cast<Point*>(geometry);
			if (nullptr!=point) {
				QPoint topleft_relative = QPoint(mapmiddle_px-screenmiddle);
				point->drawWidget(mapAdapter, topleft_relative);
			}
		}
	}
}

Layer::LayerType Layer::layertype() const
{
	OC_METHODGATE();
	return mylayertype;
}

void Layer::setMapAdapter(QSharedPointer<MapAdapter> mapadapter)
{
	OC_METHODGATE();
	mapAdapter = mapadapter;
	emit(updateRequest());
}

void Layer::setImageManager(ImageManager *qImageManager)
{
	OC_METHODGATE();
	m_ImageManager = qImageManager;
}

}
