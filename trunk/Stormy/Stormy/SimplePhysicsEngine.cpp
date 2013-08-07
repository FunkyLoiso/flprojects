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
			QPointF* contactPoint = NULL; //Will hold pointer to point1 or point if we have a possible contact with a point, not an edge
			if(bi != glass->border.end()-1)	point2 = *(bi+1);
			else							point2 = *(glass->border.begin());
			qreal dstPoint1 = QLineF(pi->pos, point1).length();
			qreal dstPoint2 = QLineF(pi->pos, point2).length();
			qreal dstP1P2 = QLineF(point1, point2).length();
			qreal dst;
			
			if(dstPoint1 > QLineF(dstPoint2, dstP1P2, 0, 0).length())
			{//possible contact with point2
				contactPoint = &point2;
				dst = dstPoint2;
			}
			else if(dstPoint2 > QLineF(dstPoint1, dstP1P2, 0, 0).length())
			{//possible contact with point1
				contactPoint = &point1;
				dst = dstPoint1;
			}
			else
			{//possible contact with an edge
				qreal halfPerimeter = (dstPoint1 + dstPoint2 + dstP1P2) / 2;
				dst = 2/dstP1P2*qSqrt(halfPerimeter *	(halfPerimeter - dstP1P2)	* 
															(halfPerimeter - dstPoint1)	* 
															(halfPerimeter - dstPoint2)		);
			}

			if(dst < pi->radius)
			{//contact
				QPointF normal;


				if(contactPoint == NULL)
				{//contact with an edge
					normal = QPointF(point2.x()-point1.x(), point2.y()-point1.y());
					QTransform rotateCoordinates90Clockwise;
					rotateCoordinates90Clockwise.rotate(-90.0);
					normal = rotateCoordinates90Clockwise.map(normal);
				}
				else
				{//contact with a point
					normal = QPointF(contactPoint->x()-pi->pos.x(), contactPoint->y()-pi->pos.y());
				}
				//is the particle moving outward or inward?
				QVector2D speed(pi->speed);
				QVector2D norm(normal);
				qreal normalCos = QVector2D::dotProduct(norm, speed)/(speed.length()*norm.length());
				if(normalCos < 0.0f) continue;

				QTransform tr;
				qreal ang;
				if(contactPoint == NULL)	ang = qAtan2(point2.y() - point1.y(), point2.x() - point1.x());
				else						ang = -qAtan2(contactPoint->x() - pi->pos.x() ,contactPoint->y() - pi->pos.y());
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
