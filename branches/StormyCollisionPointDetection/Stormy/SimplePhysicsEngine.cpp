#include "SimplePhysicsEngine.h"

#include <QLineF>
#include <QVector2D>
#include <QTransform>
#include <qmath.h>

#include "quartic/quartic.hpp"

#include "Glass.h"


void SimplePhysicsEngine::update(Glass* glass, qreal timePassed_s)
{
	//timePassed_s = 0.1;
	m_glass = glass;
	m_time_s = timePassed_s;
	for(Glass::TParticlesMap::Iterator pi = m_glass->particles.begin(); pi != m_glass->particles.end(); ++pi)
	{// 1. Write projected speeds and positions for all particles 
		writeProjectedSpeedAndPosition(*pi);
		pi->passive = false;// mark that this particle can initiate collisions
	}
	for(Glass::TParticlesMap::Iterator pi = m_glass->particles.begin(); pi != m_glass->particles.end(); ++pi)
	{// 2. Do collision detection and corresponding projected speed and location modifications for each particle
		if(!pi->passive) doCollisions(*pi);
	}
	for(Glass::TParticlesMap::Iterator pi = m_glass->particles.begin(); pi != m_glass->particles.end(); ++pi)
	{// 3. Write projected to actual after all collisions were detected
		//writeActualSpeedAndPosition(*pi);
		pi->speed = pi->projectedSpeed;
		pi->pos = pi->projectedPos;
	}
}
	
//	//1. Calculate position for the end of the frame
//	for(m_glass::TParticlesMap::Iterator pi = m_glass->particles.begin(); pi != m_glass->particles.end(); ++pi)
//	{
//		//1. Collision with the border
//		for(QPolygonF::ConstIterator bi = m_glass->border.begin(); bi != m_glass->border.end(); ++bi)
//		{
//			//for each point calculate position at the end of this frame
//
//
//			//now we determine whether the final position corresponds to a contact with border
//			QVector2D vert1 = QVector2D(*bi);
//			QVector2D vert2;
//			QVector2D* contactVert = NULL; //Will hold pointer to point1 or point2 if we have a possible contact with a point, not an edge
//			if(bi != m_glass->border.end()-1)	vert2 = QVector2D(*(bi+1));
//			else							vert2 = QVector2D(*(m_glass->border.begin()));
//			qreal dstVert1 = (pi->projectedPos - vert1).length();
//			qreal dstVert2 = (pi->projectedPos - vert2).length();
//			qreal dstV1V2 = (vert1 - vert2).length();
//
//			qreal halfPerimeter;
//			qreal dst;
//			
//			if(dstVert1 > QVector2D(dstVert2, dstV1V2).length())
//			{//possible contact with vertex2
//				contactVert = &vert2;
//				dst = dstVert2;
//			}
//			else if(dstVert2 > QVector2D(dstVert1, dstV1V2).length())
//			{//possible contact with vertex1
//				contactVert = &vert1;
//				dst = dstVert1;
//			}
//			else
//			{//possible contact with the edge
//				halfPerimeter = (dstVert1 + dstVert2 + dstV1V2) / 2;
//				dst = 2/dstV1V2*qSqrt(halfPerimeter *	(halfPerimeter - dstV1V2)	* 
//														(halfPerimeter - dstVert1)	* 
//														(halfPerimeter - dstVert2)		);
//			}
//
//			if(dst < pi->radius)
//			{//contact
//
//				//2. Determine real contact point
//				QVector2D normal;
//
//				if(contactVert == NULL)
//				{//contact with an edge
//					QVector2D edge(vert2 - vert1);
//					qreal angSpeedToEdge = qAcos( qAbs(	QVector2D::dotProduct(pi->projectedSpeed, edge) /
//															(pi->projectedSpeed.length() * edge.length())		) );
//
//					//determine whether pos and tmpPos are on the same side of the edge
//					qreal dl = 0;
//					//if(qFuzzyCompare(angSpeedWithEdge, c_pi/2))
//					//{//if speed is perpendicular to the edge
//					//	if((pi->pos - pi->tmpPos).length() > dst)//tmpPos is beyond the edge
//					//	{
//					//		dl = pi->radius + dst;
//					//	}
//					//	else
//					//	{
//					//		dl = pi->radius - dst;
//					//	}
//
//					//}
//					//else
//					{
//						QVector2D posToVrt2(vert2 - pi->pos);
//						QVector2D tmpPosToVrt2(vert2 - pi->projectedPos);
//						qreal angPosToEdge = qAcos( QVector2D::dotProduct(posToVrt2, edge) / (posToVrt2.length() * edge.length()) );
//						qreal angTmpPosToPos = qAcos( QVector2D::dotProduct(tmpPosToVrt2, posToVrt2) / (tmpPosToVrt2.length() * posToVrt2.length()) );
//						int sgn = -1;
//						if(angTmpPosToPos > angPosToEdge) sgn = 1;
//	
//						dl = (pi->radius + sgn*dst) / qSin(angSpeedToEdge);
//					}
//
//					normal = vert2 - vert1;
//					QTransform rotateCoordinates90Clockwise;
//					rotateCoordinates90Clockwise.rotate(-90.0);
//					normal = QVector2D(rotateCoordinates90Clockwise.map(normal.toPointF()));
//				}
//				else
//				{//contact with a point
// 					QVector2D tmpPosToContactVertex(*contactVert - pi->projectedPos);
//					qreal angleOppositeToR = qAcos(	QVector2D::dotProduct(-pi->projectedSpeed, tmpPosToContactVertex) / 
//														(pi->projectedSpeed.length() * tmpPosToContactVertex.length())		);
//					qreal angleOppositeToDst = qAsin(dst/pi->radius * qSin(angleOppositeToR));
//					qreal angleOppositeTo_dl = c_pi - angleOppositeToDst - angleOppositeToR;
//					qreal dl = pi->radius * qSin(angleOppositeTo_dl) / qSin(angleOppositeToR);
//
//					normal = *contactVert - pi->pos;
//				}
//				//calculate contactPoint
//				//move tmpPos on dl in the direction opposite to speed vector
//				QVector2D contactPoint = pi->projectedPos - pi->projectedSpeed.normalized() * dl;
//
//				//is the particle moving outward or inward?
//				if(QVector2D::dotProduct(normal, pi->projectedSpeed) <= 0.0f) continue;
//
//				//3. Determine speed at contactPoint and time left to move
//				QVector2D posToContactPoint = contactPoint - pi->pos;
//				int speedSign = sign(QVector2D::dotProduct(pi->speed, posToContactPoint));
//				qreal speedVal = speedSign * pi->speed.length();
//				int accelSign = sign(QVector2D::dotProduct(m_gravity, posToContactPoint));
//				qreal accelVal = m_gravity.length();
//
//				qreal timeBeforeContact;
//
//				if(qFuzzyIsNull(accelVal))
//				{
//					timeBeforeContact = posToContactPoint.length() / speedVal;
//				}
//				else
//				{
//					qreal tmp = qSqrt(speedVal * speedVal + 2 * accelVal * posToContactPoint.length());
//					qreal t1 = (tmp - speedVal)/accelVal;
//					qreal t2 = (-tmp - speedVal)/accelVal;
//					
//					if(t1 < 0.0f)		timeBeforeContact = t2;
//					else if(t2 < 0.0f)	timeBeforeContact = t1;
//					else				timeBeforeContact = qMin(t1, t2);	
//				}
//		
//
//				QVector2D speedAtContact = pi->speed + m_gravity*timeBeforeContact;
//				qreal timeLeft = m_time_s - timeBeforeContact;
//
//				QTransform tr;
//				qreal ang;
//				if(contactVert == NULL)	ang = qAtan2(vert2.y() - vert1.y(), vert2.x() - vert1.x());
//				else					ang = -qAtan2(contactVert->x() - pi->pos.x(), contactVert->y() - pi->pos.y());
//				tr.rotateRadians(-ang);
//
//				QPointF newSpeedAtContact = tr.map(speedAtContact.toPointF());
//				newSpeedAtContact.ry() *= -m_restitution;
//				newSpeedAtContact = tr.inverted().map(newSpeedAtContact);
//
//				QVector2D dvLeft = m_gravity * timeLeft;
//				QVector2D dpLeft = (QVector2D(newSpeedAtContact) + dvLeft/2) * timeLeft;
//
//				pi->speed = QVector2D(newSpeedAtContact) + dvLeft;
//				pi->pos = contactPoint + dpLeft;
//				continue;
//			}
//		}
//		//2. Collision with other particles
//		for(m_glass::TParticlesMap::Iterator pi2 = m_glass->particles.begin(); pi2 != m_glass->particles.end(); ++pi2)
//		{
//			if(pi == pi2) continue;
//			if(pi2->passive) continue;
//			if((pi->pos - pi2->pos).length() < pi->radius+pi2->radius)
//			{
//				QPointF normal = QPointF(pi2->pos.x()-pi->pos.x(), pi2->pos.y()-pi->pos.y());
//				QTransform tr;
//				qreal ang = -qAtan2(pi2->pos.x() - pi->pos.x(), pi2->pos.y() - pi->pos.y());
//				tr.rotateRadians(-ang);
//
//				//
//				QVector2D v1(pi->speed), v2(pi2->speed);
//				QVector2D p1(pi->pos), p2(pi2->pos);
//
//				const QVector2D dV = v1 - v2;
//				const QVector2D dP = p1 - p2;
//
//				if(QVector2D::dotProduct(dV, dP) > 0) continue;
//
//				v1 = QVector2D(tr.map(pi->speed.toPointF()));
//				v2 = QVector2D(tr.map(pi2->speed.toPointF()));
//
//				qreal m1 = pi->mass, m2 = pi2->mass;
//				qreal y1 = (m1*v1.y() - m2*m_restitution*(v1.y() - v2.y()) + m2*v2.y())/(m1 + m2);
//				qreal y2 = y1 + m_restitution*(v1.y() - v2.y());
//
//				v1.setY(y1);
//				v2.setY(y2);
//
//				pi->speed = QVector2D( tr.inverted().map(v1.toPointF()) );
//				pi2->speed = QVector2D( tr.inverted().map(v2.toPointF()) );
//
//				pi->passive = true;
//				pi2->passive = true;
//			}
//		}
//	}
//
//	//3. Update locations
//	double totalEnergy = 0.0f;
//	for(m_glass::TParticlesMap::Iterator pi = m_glass->particles.begin(); pi != m_glass->particles.end(); ++pi)
//	{
//		//qreal frictionAcclerationX = pi->speed.x()*m_friction/pi->mass;
//		//qreal frictionAcclerationY = pi->speed.y()*m_friction/pi->mass;
//
//		//qreal frictionAcceleration = m_friction * c_gravityOfEarth / (c_pi * pi->radius*pi->radius);
//
//		//pi->speed.setX( pi->speed.x() + (-sign(pi->speed.x())*frictionAcceleration + m_gravityX)*m_time_s );
//		//pi->speed.setY( pi->speed.y() + (-sign(pi->speed.y())*frictionAcceleration + m_gravityY)*m_time_s );
//
//		QVector2D dv = m_gravity*m_time_s;
//		QVector2D dp = (pi->speed + dv/2)*m_time_s;
//
//		pi->speed += dv;
//		pi->pos += dp;
//
//		//pi->speed.setX( pi->speed.x() + m_gravityX*m_time_s );
//		//pi->speed.setY( pi->speed.y() + m_gravityY*m_time_s );
//
//		//pi->pos.setX(pi->pos.x() + pi->speed.x()*m_time_s);
//		//pi->pos.setY(pi->pos.y() + pi->speed.y()*m_time_s);
//
//		pi->passive = false;
//
//		totalEnergy += pi->speed.lengthSquared() * pi->mass;
//	}
//
//	m_glass->totalEnegry = totalEnergy;
//}

void SimplePhysicsEngine::writeProjectedSpeedAndPosition(Particle& p)
{
	QVector2D dv = m_gravity * m_time_s;
	QVector2D dp = (p.speed + dv/2) * m_time_s;

	p.projectedSpeed = p.speed + dv;
	p.projectedPos = p.pos + dp;
	p.posTime = 0.0;
}

void SimplePhysicsEngine::doCollisions(Particle& p)
{
	Collision c;
	if(findFirstCollision(p, c))
	{
		processCollision(c);//updates projected speed and position
		doCollisions(*c.particle);	//recursion is fishy here...
		if(c.type == Collision::WithParticle) doCollisions(*c.otherParticle);
	}
	p.passive = true;
}

bool SimplePhysicsEngine::findFirstCollision(Particle& p, Collision& out_collision) const
{
	qreal minTime = m_time_s*1.1;//slightly more so that minTime > m_time_s is garanted
	
	//1. Check for collisions with the border (both edges and vertices)
	for(QPolygonF::ConstIterator bi = m_glass->border.begin(); bi != m_glass->border.end(); ++bi)
	{
		//now we determine whether the final position corresponds to a contact with border
		QVector2D vert1 = QVector2D(*bi);
		QVector2D vert2;
		QVector2D* contactVert = NULL; //Will hold pointer to point1 or point2 if we have a possible contact with a point, not an edge
		if(bi != m_glass->border.end()-1)	vert2 = QVector2D(*(bi+1));
		else								vert2 = QVector2D(*(m_glass->border.begin()));
		qreal dstVert1 = (p.projectedPos - vert1).length();
		qreal dstVert2 = (p.projectedPos - vert2).length();
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

		if(dst < p.radius)
		{//contact
			//2. Determine contact time
			qreal contactTime = m_time_s*1.1;
			QVector2D directionVector;
			if(contactVert == NULL)
			{//contact with an edge
				qreal x1, y1, x2, y2;//points on the line
				{
					directionVector = vert2 - vert1;
					directionVector.normalize();
					x1 = vert1.x() - p.radius * directionVector.y();
					y1 = vert1.y() + p.radius * directionVector.x();
					x2 = vert2.x() - p.radius * directionVector.y();
					y2 = vert2.y() + p.radius * directionVector.x();
				}

				qreal A, B, C;//quadratic equation coefficients
				{
					qreal dx = x1 - x2;
					qreal dy = y1 - y2;
					if(qFuzzyIsNull(dx))
					{
						A = m_gravity.x()/2;
						B = p.speed.x();
						C = p.pos.x() - x1;
					}
					else if(qFuzzyIsNull(dy))
					{
						A = m_gravity.y()/2;
						B = p.speed.y();
						C = p.pos.y() - y1;
					}
					else
					{
						A = m_gravity.y()*dx/(2*dy) - m_gravity.x()/2;
						B = p.speed.y()*dx/dy - p.speed.x();
						C = (p.pos.y() - (x1*y2 - x2*y1)/dx)*dx/dy - p.pos.x();
					}
				}
				//now solve the At^2 + Bt + C = 0 for intersection times
				if(qFuzzyIsNull(A))
				{//linear equation, acceleration is zero
					contactTime = -C/B;
				}
				else
				{//quadratic
					qreal t1, t2;
					bool solved = magnet::math::quadraticSolve(B/A, C/A, t1, t2);
					if(solved)
					{
						if(t1 < 0)		contactTime = t2;
						else if(t2 < 0)	contactTime = t1;
						else			contactTime = qMin(t1, t2);
					}
				}

				//QVector2D contactPoint = p.pos + (p.speed + m_gravity*time/2)*time;
				//int neverCock = true;
			}
			else
			{//contact with a vertex
				qreal A, B, C, D, E;//Quartic equation coefficients
				{
					qreal posx_vx = p.pos.x() - contactVert->x();
					qreal posy_vy = p.pos.y() - contactVert->y();

					A = m_gravity.x()*m_gravity.x()/4 + m_gravity.y()*m_gravity.y()/4;
					B = m_gravity.x()*p.speed.x() + m_gravity.y()*p.speed.y();
					C = p.speed.x()*p.speed.x() + p.speed.y()*p.speed.y() + m_gravity.x()*posx_vx + m_gravity.y()*posy_vy;
					D = 2*p.speed.x()*posx_vx + 2*p.speed.y()*posy_vy;
					E = posx_vx*posx_vx + posy_vy*posy_vy - p.radius*p.radius;

					qreal roots[4];	//the roots will be here
					size_t nRoots;	//number of roots
					if(qFuzzyIsNull(A))
					{//quadratic equation (acceleration is zero)
						bool solved = magnet::math::quadraticSolve(D/C, E/C, roots[0], roots[1]);
						nRoots = solved ? 2 : 0;
					}
					else
					{//quartic equation
						nRoots = magnet::math::quarticSolve(B/A, C/A, D/A, E/A, roots[0], roots[1], roots[2], roots[3]);
					}

					for(int i = 0; i < nRoots; ++i)
					{
						qreal root = roots[i];
						if(root > 0.0 && root < contactTime) contactTime = root;
					}
				}
			}

			if(contactTime < minTime)
			{
				out_collision.type = contactVert ? Collision::WithVertex : Collision::WithEdge;
				out_collision.particle = &p;
				out_collision.contactTime_s = contactTime;
				out_collision.contactVal = contactVert ? *contactVert : directionVector;

				minTime = contactTime;
			}
		}
	}
	// 2. Now check for collisions with other particles
	for(Glass::TParticlesMap::Iterator pi2 = m_glass->particles.begin(); pi2 != m_glass->particles.end(); ++pi2)
	{
		Particle& p2 = *pi2;
		if(p == p2) continue;

		
		qreal minDistance = p.radius+p2.radius;
		if((p.projectedPos - p2.projectedPos).length() < minDistance)
		{//contact
			qreal A, B, C;//Quadratic equation coefficients
			{
				QVector2D dPos(p2.pos - p.pos);
				QVector2D dSpeed(p2.speed - p.speed);

				A = dSpeed.x()*dSpeed.x() + dSpeed.y()*dSpeed.y();
				B = 2*(dSpeed.x()*dPos.x() + dSpeed.y()*dPos.y());
				C = dPos.x()*dPos.x() + dPos.y()*dPos.y() - minDistance*minDistance;
			}

			qreal t1, t2;
			bool solved = magnet::math::quadraticSolve(B/A, C/A, t1, t2);
			if(solved)
			{
				qreal contactTime;
				if(t1 < 0)		contactTime = t2;
				else if(t2 < 0)	contactTime = t1;
				else			contactTime = qMin(t1, t2);


				if(contactTime < minTime)
				{
					out_collision.type = Collision::WithParticle;
					out_collision.particle = &p;
					out_collision.otherParticle = &p2;
					out_collision.contactTime_s = contactTime;

					minTime = contactTime;
				}
			}
		}
	}
	return minTime < m_time_s;
}

void SimplePhysicsEngine::processCollision(Collision& c)
{
	Particle& p = *c.particle;
	
	QVector2D dv = m_gravity * c.contactTime_s;
	QVector2D dp = (p.speed + dv/2)*c.contactTime_s;

	p.pos += dp;
	p.speed += dv;
	p.posTime += c.contactTime_s;

	if(c.type == Collision::WithParticle)
	{//with particle
		Particle& p2 = *c.otherParticle;
		QVector2D dp2 = (p2.speed + dv/2)*c.contactTime_s;
		
		p2.pos += dp2;
		p2.speed += dv;
		p2.posTime += c.contactTime_s;

		QVector2D pToP2(p2.pos - p.pos);
		QVector2D directionVector(-pToP2.y(), pToP2.x());

		qreal ang = -qAtan2(directionVector.y(), directionVector.x());

		QTransform tr;
		tr.rotateRadians(ang);

		QVector2D v1 = QVector2D(tr.map(p.speed.toPointF()));
		QVector2D v2 = QVector2D(tr.map(p2.speed.toPointF()));

		qreal m1 = p.mass, m2 = p2.mass;
		qreal y1 = (m1*v1.y() - m2*m_restitution*(v1.y() - v2.y()) + m2*v2.y())/(m1 + m2);
		qreal y2 = y1 + m_restitution*(v1.y() - v2.y());

		v1.setY(y1);
		v2.setY(y2);

		p.speed = QVector2D( tr.inverted().map(v1.toPointF()) );
		p2.speed = QVector2D( tr.inverted().map(v2.toPointF()) );

		//now calculate the projected speed and pos for remaining time
		qreal timeLeft = m_time_s - p.posTime;
		dv = m_gravity * timeLeft;

		dp = (p.speed + dv/2)*timeLeft;
		p.projectedPos = p.pos + dp;
		p.projectedSpeed = p.speed + dv;

		dp2 = (p2.speed + dv/2)*timeLeft;
		p2.projectedPos = p2.pos + dp2;
		p2.projectedSpeed = p2.speed + dv;
	}
	else
	{//with edge or vertex
		QVector2D directionVector;
		if(c.type == Collision::WithEdge)
		{//direction vector is present
			directionVector = c.contactVal;
		}
		else
		{//direction vector is vector from pos to contact vertex, rotated 90 degrees
			QVector2D posToVertex(c.contactVal - p.pos);
			directionVector.setX(-posToVertex.y());
			directionVector.setY(posToVertex.x());
		}
		qreal ang = -qAtan2(directionVector.y(), directionVector.x());
		
		QTransform tr;
		tr.rotateRadians(ang);

		p.speed = QVector2D(tr.map(p.speed.toPointF()));
		p.speed.setY(p.speed.y() * -m_restitution);
		p.speed = QVector2D(tr.inverted().map(p.speed.toPointF()));

		qreal timeLeft = m_time_s - p.posTime;
		dv = m_gravity * timeLeft;
		dp = (p.speed + dv/2)*timeLeft;

		p.projectedPos = p.pos + dp;
		p.projectedSpeed = p.speed += dv;
	}
}

qreal SimplePhysicsEngine::cosBetweenVectors(QVector2D v1, QVector2D v2)
{
	return	QVector2D::dotProduct(v1, v2) / (v1.length() * v2.length());

}
