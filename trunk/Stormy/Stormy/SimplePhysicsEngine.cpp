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
				if(normalCos <= 0.0f) continue;

				QTransform tr;
				qreal ang;
				if(contactPoint == NULL)	ang = qAtan2(point2.y() - point1.y(), point2.x() - point1.x());
				else						ang = -qAtan2(contactPoint->x() - pi->pos.x(), contactPoint->y() - pi->pos.y());
				tr.rotateRadians(-ang);

				QPointF newSpeed = tr.map(pi->speed);
				newSpeed.ry() *= -1*m_restitution;

				newSpeed = tr.inverted().map(newSpeed);

				pi->speed = newSpeed;
				continue;
			}
		}
		//2. Collision with other particles
		for(Glass::TParticlesMap::Iterator pi2 = glass->particles.begin(); pi2 != glass->particles.end(); ++pi2)
		{
			if(pi == pi2) continue;
			if(pi2->mark) continue;
			if(QLineF(pi->pos, pi2->pos).length() < pi->radius+pi2->radius)
			{
				QPointF normal = QPointF(pi2->pos.x()-pi->pos.x(), pi2->pos.y()-pi->pos.y());
				QTransform tr;
				qreal ang = -qAtan2(pi2->pos.x() - pi->pos.x(), pi2->pos.y() - pi->pos.y());
				tr.rotateRadians(-ang);

				//
				QVector2D v1(pi->speed), v2(pi2->speed);
				QVector2D p1(pi->pos), p2(pi2->pos);

				QVector2D dV = v1 - v2;
				QVector2D dP = p1 - p2;

				if(QVector2D::dotProduct(dV, dP) > 0) continue;

				v1 = QVector2D(tr.map(pi->speed));
				v2 = QVector2D(tr.map(pi2->speed));

				qreal m1 = pi->mass, m2 = pi2->mass;
				//QVector2D u1 = (m1*v1 - m2*m_restitution*(v1 - v2) + m2*v2)/(m1 + m2);
				//QVector2D u2 = u1 + m_restitution*(v1- v2);

				qreal y1 = (m1*v1.y() - m2*m_restitution*(v1.y() - v2.y()) + m2*v2.y())/(m1 + m2);
				qreal y2 = y1 + m_restitution*(v1.y() - v2.y());

				v1.setY(y1);
				v2.setY(y2);

				pi->speed = tr.inverted().map(v1.toPointF());
				pi2->speed = tr.inverted().map(v2.toPointF());

				//pi->speed = u1.toPointF();
				//pi2->speed = u2.toPointF();

				pi->mark = true;
				pi2->mark = true;
			}
		}
	}

	//3. Update locations
	double totalEnergy = 0.0f;
	for(Glass::TParticlesMap::Iterator pi = glass->particles.begin(); pi != glass->particles.end(); ++pi)
	{
		//qreal frictionAcclerationX = pi->speed.x()*m_friction/pi->mass;
		//qreal frictionAcclerationY = pi->speed.y()*m_friction/pi->mass;

		qreal frictionAcceleration = m_friction * c_gravityOfEarth / (c_pi * pi->radius*pi->radius);

		pi->speed.rx() -= sign(pi->speed.rx())*frictionAcceleration*timePassed_s;
		pi->speed.ry() -= sign(pi->speed.ry())*frictionAcceleration*timePassed_s;

		pi->pos.setX(pi->pos.x() + pi->speed.rx()*timePassed_s);
		pi->pos.setY(pi->pos.y() + pi->speed.ry()*timePassed_s);

		pi->mark = false;
	}

	glass->totalEnegry = totalEnergy;
}
