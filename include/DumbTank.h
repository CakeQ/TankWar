#ifndef DUMB_TANK_H
#define DUMB_TANK_H

#include "aitank.h"
//  CHEAT SHEET!
//
//  tanks move 1.7 per tick could be 1 in x, .7 in y and so on but either way its gonan be 1.7 wherever its pointing
//  shells move 3 per tick could be 2 in x, 1 in y and so on but either way its gonan be 3 wherever its pointing
//
//  to get our tanks data use
//  pos.getX() 
//	pos.getY()
//	pos.getTH() which is angle relative to worldspace right is zero down is 90 etc
//
//	on the mark functions they return a position of whats marked and call it p, so for them i is 
//  p.getX() 
//	p.getY()
//	p.getTH()
//
//  world space is 780 by 570
//	borders are 10 thick
//
//
//
class DumbTank : public AITank
{
	enum State { Searching, Engaging, Attacking, Dodging, Panic };

private:
	bool forwardIsEfficient, enemyVisable, shellVisible, enemySpottedThisTick, shellSpottedThisTick, enemyGhostPositonValid, enemyShellGhostPositonValid;
	bool turnOrMove;
	int shellGhostValidCounter, tankGhostValidCounter;
	float ticksToImpactUs, ticksToImpactOurGhost;
	Position enemyPosition, shellPosition;
	State state;
	sf::Vector2f PastLocations[5];
	sf::Vector2f HostilePastLocations[5];
	sf::Vector2f HostileShellPastLocations[5];
	sf::Vector2f ourPredictedLocationGhost;
	sf::Vector2f theirPredictedLocationGhost;
	sf::Vector2f theirShellPredictedLocationGhost;
public:
    DumbTank();
    ~DumbTank();
	void move();
	void reset();
	void collided();
	void markTarget(Position p);
	void markEnemy(Position p);
	void markBase(Position p);
	void markShell(Position p);
	bool isFiring(); 
	void score(int thisScore,int enemyScore);

	//Alex's utillity functions please use but dont mess with unless your Jo fixing math, ps Jo please save this
	sf::Vector2f getPredictedPosition(sf::Vector2f PastLocationsIn[5], float timeInTicks);
	void moveToIdeal(sf::Vector2f idealLocationIn); //broken*
	void lookToIdeal(sf::Vector2f idealLocationIn); //broken*
	float getTicksToImpact(sf::Vector2f theirTankPositionIn, bool shellIn);
	float getChangeInAngleToTarget(sf::Vector2f targetPositionIn, bool isTurret); // broken
	bool willTheseCollideInN(sf::Vector2f target, sf::Vector2f PastLocationsInOfProjectile[5], float ticksIn);

	//Dan's utility functions
	float getDistanceToTarget(sf::Vector2f target);
	void calculatePatrolPoints();
	void tankPatrolRoute();
	void getNextPatrolNode();

	//Dan's utility variables
	static const int patrolWidth = 2, patrolHeight = 4;
	const float mapHeight = 570.0f, mapWidth = 780.0f;
	sf::Vector2f patrolRoute[patrolHeight][patrolWidth], currentPatrolNode, previousPatrolNode;
};
#endif