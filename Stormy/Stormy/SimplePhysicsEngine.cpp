#include "SimplePhysicsEngine.h"

#include <QLineF>
#include <QVector2D>
#include <QTransform>
#include <qmath.h>
#include "Glass.h"


void SimplePhysicsEngine::update(Glass* glass, qreal timePassed_s)
{
	//timePassed_s = 0.01;
	for(Glass::TParticlesMap::Iterator pi = glass->particles.begin(); pi != glass->particles.end(); ++pi)
	{
		//1. Collision with the border
		for(QPolygonF::ConstIterator bi = glass->border.begin(); bi != glass->border.end(); ++bi)
		{
			QPointF point1 = *bi;
			QPointF point2;
			if(bi != glass->border.end()-1)	point2 = *(bi+1);
			else							point2 = *(glass->border.begin());
			qreal dstPoint1 = QLineF(pi->pos, point1).length();
			qreal dstPoint2 = QLineF(pi->pos, point2).length();
			qreal dstP1P2 = QLineF(point1, point2).length();
			qreal halfPerimeter = (dstPoint1 + dstPoint2 + dstP1P2) / 2;

			qreal dst = 2/dstP1P2*qSqrt(halfPerimeter *	(halfPerimeter - dstP1P2)	* 
														(halfPerimeter - dstPoint1)	* 
														(halfPerimeter - dstPoint2)		);


			if(dst < pi->radius)
			{//contact with the wall!
				//is the particle moving outward or inward?
				QPointF normal(point2.x()-point1.x(), point2.y()-point1.y());
				QTransform rotateCoordinates90Clockwise;
				rotateCoordinates90Clockwise.rotate(-90.0);
				normal = rotateCoordinates90Clockwise.map(normal);

				QVector2D speed(pi->speed);
				QVector2D norm(normal);
				qreal normalCos = QVector2D::dotProduct(norm, speed)/(speed.length()*norm.length());
				if(normalCos < 0.0f) break;

				QTransform tr;
				qreal ang = qAtan2(point2.y() - point1.y(), point2.x() - point1.x());
				tr.rotateRadians(-ang);

				QPointF newSpeed = tr.map(pi->speed);
				newSpeed.ry() *= -1;

				newSpeed = tr.inverted().map(newSpeed);

				pi->speed = newSpeed;
				break;
			}
		}
		//2. Collision with other paticles
	}

	//3. Update locations
	for(Glass::TParticlesMap::Iterator pi = glass->particles.begin(); pi != glass->particles.end(); ++pi)
	{
		pi->pos.setX(pi->pos.x() + pi->speed.rx()*timePassed_s);
		pi->pos.setY(pi->pos.y() + pi->speed.ry()*timePassed_s);
	}
}
