
#include "schematic_scene.h"
#include "schematic_doc.h"
#include "qt_compat.h"

#include <QFileInfo>
#include <QGraphicsSceneDragDropEvent>

// ---------------------------------------------------
// forward to graphicscene, once it is there.
// BUG: what if there are multiple items?
ElementGraphics* SchematicDoc::itemAt(float x, float y)
{
	QPointF p(x, y);
	QGraphicsItem* I=scene()->itemAt(p, QTransform());
	if(ElementGraphics* G=dynamic_cast<ElementGraphics*>(I)){ untested();
		qDebug() << "got something" << element(G)->name();
		return G;
	}else{ untested();
		qDebug() << "miss";
		return nullptr;
	}
}

QPoint SchematicScene::gridSize() const
{
	assert(doc());
	return doc()->gridSize();
}

#ifndef USE_SCROLLVIEW
// scene()->selectedItems gives QGraphicsItems
Element* element(QGraphicsItem* g)
{
	auto e=dynamic_cast<ElementGraphics*>(g);
	if(!e) return nullptr;
	return e->operator->();
}
Component* component(QGraphicsItem* g)
{
	auto e=dynamic_cast<ElementGraphics*>(g);
	if(!e) return nullptr;
	return component(e->operator->());
}
Wire* wire(QGraphicsItem* g)
{
	auto e=dynamic_cast<ElementGraphics*>(g);
	if(!e) return nullptr;
	return wire(e->operator->());
}
WireLabel* wireLabel(QGraphicsItem* g)
{
	auto e=dynamic_cast<ElementGraphics*>(g);
	if(!e) return nullptr;
	return wireLabel(e->operator->());
}
Diagram* diagram(QGraphicsItem* g)
{
	auto e=dynamic_cast<ElementGraphics*>(g);
	if(!e) return nullptr;
	return diagram(e->operator->());
}
Painting* painting(QGraphicsItem* g)
{
	auto e=dynamic_cast<ElementGraphics*>(g);
	if(!e) return nullptr;
	return painting(e->operator->());
}
Marker* marker(QGraphicsItem* g)
{
	auto e=dynamic_cast<ElementGraphics*>(g);
	if(!e) return nullptr;
	return marker(e->operator->());
}
Node* node(QGraphicsItem* g)
{
	auto e=dynamic_cast<ElementGraphics*>(g);
	if(!e) return nullptr;
	return node(e->operator->());
}
Graph* graph(QGraphicsItem* g)
{
	auto e=dynamic_cast<ElementGraphics*>(g);
	if(!e) return nullptr;
	return graph(e->operator->());
}

#endif

SchematicScene::SchematicScene(QObject *parent)
  : QGraphicsScene(parent)
{
}

SchematicDoc* SchematicScene::doc()
{
	assert(parent());
	return dynamic_cast<SchematicDoc*>(parent());
}
SchematicDoc const* SchematicScene::doc() const
{
	assert(parent());
	return dynamic_cast<SchematicDoc const*>(parent());
}

SchematicScene::~SchematicScene()
{
}

QPoint SchematicScene::snapToGrid(QPointF const& p) const
{
	assert(doc());
	return doc()->setOnGrid(getX(p), getY(p));
}

void SchematicScene::drawBackground(QPainter *painter, const QRectF &r)
{ untested(); // for now.
	//	QGraphicsScene::drawBackground(painter, r);

	double gridSize = 20; // TODO

	QPen pen;
	painter->setPen(pen);

	// not sure if this is good.
	double left = r.left() - fmod(r.left(), gridSize);
	double top = r.top() - fmod(r.top(), gridSize);
	for (double x=left; x<r.right(); x+=gridSize){
		for (double y=top; y<r.bottom(); y+=gridSize){
			painter->drawPoint(QPointF(x,y));
		}
	}
#if 0
	/// \todo getter and setter
	int GridX = 10;
	int GridY = 10;

	// Get associated view, assume single view
	// SchematicDoc *v = static_cast<SchematicDoc *>(views().at(0));

	// When scaling, adjust visible grid spacing
	float scale = 1;// v->Scale;
	if(scale < 1) {
		if(scale > 0.5) {
			GridX *= 4;
			GridY *= 4;
		} else {
			GridX *= 16;
			GridY *= 16;
		}
	}
#endif
}

#ifndef USE_SCROLLVIEW
// void ElementGraphics::paintScheme(SchematicDoc *p)
// { untested();
//   	assert(_e);
// 	_e->paintScheme(p);
// }

// scene::display(SchematicModel&)?
// 'l' is a bit of a hack. let's see
void SchematicModel::toScene(QGraphicsScene& s, QList<ElementGraphics*>* l) const
{
	incomplete(); // this is too strange.
  for(auto i : components()){ untested();
    auto x=new ElementGraphics(i);
	 if(l){
		 l->append(x);
	 }
    s.addItem(x);
  }
  for(auto i : wires()){ untested();
    auto x=new ElementGraphics(i);
	 if(l){
		 l->append(x);
	 }
    s.addItem(x);
  }

  incomplete(); // do the others...
  qDebug() << "wires" << s.items().size();
  s.update();
}

#if 0
void SchematicScene::removeItem(Element const* xx)
{ unreachable();
}
#endif

// FIXME: is the weird order really necessary?
void SchematicScene::selectedItemsAndBoundingBox(QList<ElementGraphics*>& ElementCache, QRectF& BB)
{
	for(auto elt : selectedItems()){
		if(BB.isEmpty()){
			// BUG
			BB = elt->boundingRect();
		}else{
			BB = BB.united(elt->boundingRect());
		}
		ElementGraphics* eg=prechecked_cast<ElementGraphics*>(elt);
		qDebug() << "selected" << element(eg)->name() << element(eg)->boundingRect();
		qDebug() << "unite" << BB;
		assert(eg);
		if(wireLabel(elt)){
			ElementCache.append(eg);
		}else{
		}
	}
	for(auto elt : selectedItems()){
		ElementGraphics* eg=prechecked_cast<ElementGraphics*>(elt);
		assert(eg);
		if(component(elt)){
			ElementCache.append(eg);
		}else{
		}
	}
	for(auto elt : selectedItems()){
		ElementGraphics* eg=prechecked_cast<ElementGraphics*>(elt);
		assert(eg);
		if(wire(elt)){
			ElementCache.append(eg);
		}else{
		}
	}
	for(auto elt : selectedItems()){
		ElementGraphics* eg=prechecked_cast<ElementGraphics*>(elt);
		assert(eg);
		if(painting(elt)){
			ElementCache.append(eg);
		}else{
		}
	}
}
#endif

void SchematicScene::dropEvent(QGraphicsSceneDragDropEvent*)
{
	// getting here when dropping stuff from the component menu
	incomplete();
}

bool SchematicScene::itemEvent(QEvent* e)
{ itested();
	if(!e){ untested();
		unreachable();
		return false;
	}else{
		trace1("scene::itemEvent", e->type());
		return doc()->handleMouseActions(e);
	}
}

//
#include "component_widget.h"
#include <QMimeData>
// https://stackoverflow.com/questions/14631098/qt-properly-integrating-undo-framework-with-qgraphicsscene

// should return true if the event e was "recognized and processed."
// (whatever that means)
//
// this is called before SchematicDoc::event
// (bound to some condition??)
bool SchematicScene::event(QEvent* e)
{ itested();
	// 155 for move.
	trace2("SchematicScene::event", e->isAccepted(), e->type());

	if(dynamic_cast<QGraphicsSceneEvent*>(e)){ itested();
	}else{ itested();
	}

	doc()->handleMouseActions(e);
	bool r = false;
	if(e->isAccepted()){
	}else{itested();
		trace0("fwd");
		r = QGraphicsScene::event(e);
	}

	trace3("SchematicScene::event post", e->isAccepted(), e->type(), r);

	if(e->isAccepted()){ itested();
		if(r){ itested();
			// move objects is here.
		}else{untested();
		}

		// doc()->handleMouseActions(e);
//		e->ignore(); // pass on to view??
	}else{itested();
		if(r){itested();
			// "new ground".
			// "rectangle draw"
		}else{itested();
		}
		// recrangle release also here.
		// done in the "View".
		//
//		doc()->handleMouseActions(e);
	}
	assert(doc());

	// TODO: this is just a stub, untangle!
	if(dynamic_cast<QDragEnterEvent*>(e)){ untested();
		trace1("scene leave", e->isAccepted());
	}else if(dynamic_cast<QMouseEvent*>(e)){ untested();
		trace1("MOUSE", e->isAccepted());
	}else if(dynamic_cast<QDragLeaveEvent*>(e)){ untested();
		trace1("scene enter", e->isAccepted());
	}else if(auto de=dynamic_cast<QGraphicsSceneDragDropEvent*>(e)){
		// 164 == QEvent::GraphicsSceneDragEnter
		// 165 == QEvent::GraphicsSceneDragMove
		// 166 == QEvent::GraphicsSceneDragLeave
		trace2("scene dragdrop", e->type(), e->isAccepted());
		auto a = ComponentWidget::itemMimeType();

		if(e->type()==QEvent::GraphicsSceneDragEnter){
			selectAll(false);
			// create element and switch to "move" mode?
			 if (de->mimeData()->hasFormat(a)){ untested();
				trace2("got payload", e->type(), e->isAccepted());

				QByteArray eltInfo = de->mimeData()->data(a);
				QDataStream dataStream(&eltInfo, QIODevice::ReadOnly);
				ComponentListWidgetItem a;
				dataStream >> a;

				Element* elt = a.cloneElement();
				assert(elt);

				trace1("setting pos", de->scenePos());
				auto pos = de->scenePos();
				elt->setCenter(pos.x(), pos.y());
				doc()->takeOwnership(elt); // BUG
				assert(elt->scope());
				auto gfx = new ElementGraphics(elt);
				{ untested();
					addItem(gfx);
				}

				gfx->setSelected(true);

				if(0){
					QGraphicsSceneMouseEvent fake(QEvent::GraphicsSceneMousePress);
					gfx->sceneEvent(&fake);
				}
			 }else{
			 }
		}else if(e->type()==165){
			e->accept(); // this sets the plus
			             // only do if mime type match...
		}else{
		}
	}else if(dynamic_cast<QGraphicsSceneMoveEvent*>(e)){
		trace1("GSM", e->type());
	}else if(dynamic_cast<QGraphicsSceneEvent*>(e)){
		trace1("GSE", e->type());
		if(e->type()==155){ itested();
			// getting here when moving around elements.
		}else{
		}
	}else{
		// 11 = leave?
		trace1("scene unknown?", e->type());
	}

	return r;
}

void SchematicScene::selectAll(bool v)
{
	for(auto i : items()){ untested();
		i->setSelected(v);
	}
}