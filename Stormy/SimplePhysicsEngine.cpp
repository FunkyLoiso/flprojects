#include "SimplePhysicsEngine.h"

#include <QLineF>
#include <QVector2D>
#include <QTransform>
#include <qmath.h>
#include "Glass.h"


void SimplePhysicsEngine::update(Glass* glass, qreal timePassed_s)
{
	//timePassed_s = 0.1;
	//1. Calculate position for the end of the frame

	for(Glass::TParticlesMap::Iterator pi = glass->particles.begin(); pi != glass->particles.end(); ++pi)
	{
		//1. Collision with the border
		for(QPolygonF::ConstIterator bi = glass->border.begin(); bi != glass->border.end(); ++bi)
		{
			//for each point calculate position at the end of this frame
			QVector2D a(m_gravityX, m_gravityY);
			QVector2D dv = a*timePassed_s;
			QVector2D dp = (pi->speed + dv/2)*timePassed_s;

			pi->tmpSpeed = pi->speed + dv;
			pi->tmpPos = pi->pos + dp;

			//now we determine whether the final position corresponds to a contact with border
			QVector2D vert1 = QVector2D(*bi);
			QVector2D vert2;
			QVector2D* contactVert = NULL; //Will hold pointer to point1 or point2 if we have a possible contact with a point, not an edge
			if(bi != glass->border.end()-1)	vert2 = QVector2D(*(bi+1));
			else							vert2 = QVector2D(*(glass->border.begin()));
			qreal dstVert1 = (pi->tmpPos - vert1).length();
			qreal dstVert2 = (pi->tmpPos - vert2).length();
			qreal dstV1V2 = (vert1 - vert2).length();

			qreal halfPerimeter;
			qreal dst;
			
			if(dstVert1 > QVector2D(dstVert2, dstV1V2).length())
			{//possible contact with vertex2
				contactVert = &vert2;
				dst = dstVert2;
			}
			else if(dstVert2 > QVector2D(dstVert1, dstV1V2).length())
			{//possible contact with vertex1
				contactVert = &vert1;
				dst = dstVert1;
			}
			else
			{//possible contact with the edge
				halfPerimeter = (dstVert1 + dstVert2 + dstV1V2) / 2;
				dst = 2/dstV1V2*qSqrt(halfPerimeter *	(halfPerimeter - dstV1V2)	* 
														(halfPerimeter - dstVert1)	* 
														(halfPerimeter - dstVert2)		);
			}

			if(dst < pi->radius)
			{//contact

				//2. Determine real contact point
				QVector2D normal;
				QVector2D contactPoint;

				if(contactVert == NULL)
				{//contact with an edge
					QVector2D edge(vert2 - vert1);
					qreal angSpeedToEdge = qAcos( qAbs(	QVector2D::dotProduct(pi->tmpSpeed, edge) /
															(pi->tmpSpeed.length() * edge.length())		) );

					//determine whether pos and tmpPos are on the same side of the edge
					qreal dl = 0;
					//if(qFuzzyCompare(angSpeedWithEdge, c_pi/2))
					//{//if speed is perpendicular to the edge
					//	if((pi->pos - pi->tmpPos).length() > dst)//tmpPos is beyond the edge
					//	{
					//		dl = pi->radius + dst;
					//	}
					//	else
					//	{
					//		dl = pi->radius - dst;
					//	}

					//}
					//else
					{
						QVector2D posToVrt2(vert2 - pi->pos);
						QVector2D tmpPosToVrt2(vert2 - pi->tmpPos);
						qreal angPosToEdge = qAcos( QVector2D::dotProduct(posToVrt2, edge) / (posToVrt2.length() * edge.length()) );
						qreal angTmpPosToPos = qAcos( QVector2D::dotProduct(tmpPosToVrt2, posToVrt2) / (tmpPosToVrt2.length() * posToVrt2.length()) );
						int sgn = -1;
						if(angTmpPosToPos > angPosToEdge) sgn = 1;
	
						dl = (pi->radius + sgn*dst) / qSin(angSpeedToEdge);
					}

					//calculate contactPoint
					//move tmpPos on dl in the direction opposite to speed vector
					contactPoint = pi->tmpPos - pi->tmpSpeed.normalized() * dl;

					normal = vert2 - vert1;
					QTransform rotateCoordinates90Clockwise;
					rotateCoordinates90Clockwise.rotate(-90.0);
					normal = QVector2D(rotateCoordinates90Clockwise.map(normal.toPointF()));
				}
				else
				{//contact with a point
 					QVector2D tmpPosToContactVertex(*contactVert - pi->tmpPos);
					qreal angleOppositeToR = qAcos(	QVector2D::dotProduct(-pi->tmpSpeed, tmpPosToContactVertex) / 
														(pi->tmpSpeed.length() * tmpPosToContactVertex.length())		);
					qreal angleOppositeToDst = qAsin(dst/pi->radius * qSin(angleOppositeToR));
					qreal angleOppositeTo_dl = c_pi - angleOppositeToDst - angleOppositeToR;
					qreal dl = pi->radius * qSin(angleOppositeTo_dl) / qSin(angleOppositeToR);

					normal = *contactVert - pi->pos;
				}
				//is the particle moving outward or inward?
				if(QVector2D::dotProduct(normal, pi->tmpSpeed) <= 0.0f) continue;

				//3. Determine speed at contactPoint and time left to move
				QVector2D posToContactPoint = contactPoint - pi->pos;
				int speedSign = sign(QVector2D::dotProduct(pi->speed, posToContactPoint));
				qreal speedVal = speedSign * pi->speed.length();
				int accelSign = sign(QVector2D::dotProduct(a, posToContactPoint));
				qreal accelVal = a.length();

				qreal timeBeforeContact;

				if(qFuzzyIsNull(accelVal))
				{
					timeBeforeContact = posToContactPoint.length() / speedVal;
				}
				else
				{
					qreal tmp = qSqrt(speedVal * speedVal + 2 * accelVal * posToContactPoint.length());
					qreal t1 = (tmp - speedVal)/accelVal;
					qreal t2 = (-tmp - speedVal)/accelVal;
					
					if(t1 < 0.0f)		timeBeforeContact = t2;
					else if(t2 < 0.0f)	timeBeforeContact = t1;
					else				timeBeforeContact = qMin(t1, t2);	
				}
		

				QVector2D speedAtContact = pi->speed + a*timeBeforeContact;
				qreal timeLeft = timePassed_s - timeBeforeContact;

				QTransform tr;
				qreal ang;
				if(contactVert == NULL)	ang = qAtan2(vert2.y() - vert1.y(), vert2.x() - vert1.x());
				else					ang = -qAtan2(contactVert->x() - pi->pos.x(), contactVert->y() - pi->pos.y());
				tr.rotateRadians(-ang);

				QPointF newSpeedAtContact = tr.map(speedAtContact.toPointF());
				newSpeedAtContact.ry() *= -m_restitution;
				newSpeedAtContact = tr.inverted().map(newSpeedAtContact);

				QVector2D dvLeft = a * timeLeft;
				QVector2D dpLeft = (QVector2D(newSpeedAtContact) + dvLeft/2) * timeLeft;

				pi->speed = QVector2D(newSpeedAtContact) + dvLeft;
				pi->pos = contactPoint + dpLeft;
				continue;
			}
		}
		//2. Collision with other particles
		for(Glass::TParticlesMap::Iterator pi2 = glass->particles.begin(); pi2 != glass->particles.end(); ++pi2)
		{
			if(pi == pi2) continue;
			if(pi2->mark) continue;
			if((pi->pos - pi2->pos).length() < pi->radius+pi2->radius)
			{
				QPointF normal = QPointF(pi2->pos.x()-pi->pos.x(), pi2->pos.y()-pi->pos.y());
				QTransform tr;
				qreal ang = -qAtan2(pi2->pos.x() - pi->pos.x(), pi2->pos.y() - pi->pos.y());
				tr.rotateRadians(-ang);

				//
				QVector2D v1(pi->speed), v2(pi2->speed);
				QVector2D p1(pi->pos), p2(pi2->pos);

				const QVector2D dV = v1 - v2;
				const QVector2D dP = p1 - p2;

				if(QVector2D::dotProduct(dV, dP) > 0) continue;

				v1 = QVector2D(tr.map(pi->speed.toPointF()));
				v2 = QVector2D(tr.map(pi2->speed.toPointF()));

				qreal m1 = pi->mass, m2 = pi2->mass;
				qreal y1 = (m1*v1.y() - m2*m_restitution*(v1.y() - v2.y()) + m2*v2.y())/(m1 + m2);
				qreal y2 = y1 + m_restitution*(v1.y() - v2.y());

				v1.setY(y1);
				v2.setY(y2);

				pi->speed = QVector2D( tr.inverted().map(v1.toPointF()) );
				pi2->speed = QVector2D( tr.inverted().map(v2.toPointF()) );

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

		//qreal frictionAcceleration = m_friction * c_gravityOfEarth / (c_pi * pi->radius*pi->radius);

		//pi->speed.setX( pi->speed.x() + (-sign(pi->speed.x())*frictionAcceleration + m_gravityX)*timePassed_s );
		//pi->speed.setY( pi->speed.y() + (-sign(pi->speed.y())*frictionAcceleration + m_gravityY)*timePassed_s );

		QVector2D a(m_gravityX, m_gravityY);
		QVector2D dv = a*timePassed_s;
		QVector2D dp = (pi->speed + dv/2)*timePassed_s;

		pi->speed += dv;
		pi->pos += dp;

		//pi->speed.setX( pi->speed.x() + m_gravityX*timePassed_s );
		//pi->speed.setY( pi->speed.y() + m_gravityY*timePassed_s );

		//pi->pos.setX(pi->pos.x() + pi->speed.x()*timePassed_s);
		//pi->pos.setY(pi->pos.y() + pi->speed.y()*timePassed_s);

		pi->mark = false;

		totalEnergy += pi->speed.lengthSquared() * pi->mass;
	}

	glass->totalEnegry = totalEnergy;
}
