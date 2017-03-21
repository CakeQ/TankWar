#include "DumbTank.h"


DumbTank::DumbTank() // Construtor
{

	for (int i = 0; i <= 5; i++) {
		PastLocations[i].x = 0.0f;
		PastLocations[i].y = 0.0f;
	}
	for (int i = 0; i <= 5; i++) {
		HostilePastLocations[i].x = 0.0f;
		HostilePastLocations[i].y = 0.0f;
	}
	calculatePatrolPoints(); //Calculating the points which the tank will use when patrolling. ~Dan
	currentPatrolNode = patrolRoute[3][0];
	previousPatrolNode = patrolRoute[2][0];
	state = Searching;
	turnOrMove = true;
}

DumbTank::~DumbTank(){} // Destructor


void DumbTank::reset()
{
	for (int i = 0; i <= 5; i++) {
		PastLocations[i].x = 0.0f;
		PastLocations[i].y = 0.0f;
	}
	for (int i = 0; i <= 5; i++) {
		HostilePastLocations[i].x = 0.0f;
		HostilePastLocations[i].y = 0.0f;
	}
	state = Searching;
}

void DumbTank::move()
{
	enemyVisable = enemySpottedThisTick;
	shellVisible = shellSpottedThisTick;

	if (!enemyVisable) {
		tankGhostValidCounter = 0;
	}
	if (!shellVisible) {
		shellGhostValidCounter = 0;
	}
	if (tankGhostValidCounter > 5) {
		enemyGhostPositonValid = true;
	}
	if (shellGhostValidCounter > 5) {
		enemyShellGhostPositonValid = true;
	}



	if (state == Searching) {																	//P3
		//set ideal location to nearest search pattern grid
	    //turretGoLeft();
		//std::cout << turretTh << std::endl;
		
		tankPatrolRoute();
		//std::cout << "Tank location: " << getX() << " , " << getY() << std::endl;
		//std::cout << "Next location: " << currentPatrolNode.x << " , " << currentPatrolNode.y << std::endl;
		//std::cout << "Distance to next node:" << getDistanceToTarget(currentPatrolNode) << std::endl;


		//enemy found? Switch to Engaging
		//state = Engaging;
		//enemy found? RIGHT BEHIND US !?!?! switch to Panic
		//state = Panic;
	}
	if (state == Engaging) {																	//P3
		//set ideal location to nearest position is safe radius from enemy


		//ideal situation? Switch to Attacking
		//been shot at? Switch to Dodging
		//lost enemy? Switch to Searching
	}
	if (state == Attacking) {																	//P3
		//set ideal location just into safe radius from enemy (to charge them)

		

		//shot fired? switch back to Engaging
	}
	if (state == Dodging) {																	//P1
		//is it gonna hit our ghost? 
		//no were good
		//yes!?! is it going to hit us?!?! 
		//no, okay then stop moving.
		//yes well shit will it hit our ghost if we reverse for 5 ticks?
		//no Phew. 
		//yes?!?! well fuck now we really gotta think about this.   
		
		//dodge
		//dodged? switch back to Engaging
	}
	if (state == Panic) {																	//P2
		//RUN AWAY!  

		//we good? yep. do we see it? 
		// no switch to Searching
		//yes switch to Engaging
	}

	// updates most resent past location
	PastLocations[0].x = pos.getX();
	PastLocations[0].y = pos.getY();
	// moves the array of 5 past locations along 1 so the most recent update replaces the oldest
	for (int i = 4; i >= 1; i--) {
		PastLocations[i] = PastLocations[i - 1];
		//std::cout << "Location: " << PastLocations[i].x << PastLocations[i].y << std::endl;
	}


	if (enemyGhostPositonValid) {
		theirPredictedLocationGhost = getPredictedPosition(HostilePastLocations, 100.0f);
	}
	if (enemyShellGhostPositonValid) {
		theirShellPredictedLocationGhost = getPredictedPosition(HostileShellPastLocations, 100.0f);
	}
	ourPredictedLocationGhost = getPredictedPosition(PastLocations, 100.0f);

	enemySpottedThisTick = false;
	shellSpottedThisTick = false;
	enemyGhostPositonValid = false;
	enemyShellGhostPositonValid = false;
}

void DumbTank::collided()
{
	
}

void DumbTank::markTarget(Position p)
{
	//std::cout << "Target spotted at (" <<p.getX() << ", " << p.getY() << ")\n"; 
}

void DumbTank::markEnemy(Position p)
{
	enemyPosition = p;
	enemySpottedThisTick = true;
	tankGhostValidCounter++;
	// updates least resent past location
	HostilePastLocations[0].x = p.getX();
	HostilePastLocations[0].y = p.getY();
	// moves the array of 5 past locations along 1 so the most recent update replaces the oldest
	for (int i = 4; i >= 1; i--) {
		HostilePastLocations[i] = HostilePastLocations[i - 1];
		//std::cout << "Location of hostile: " << PastLocations[i].x << PastLocations[i].y << std::endl;
	}


}

void DumbTank::markBase(Position p)
{
	//std::cout << "Base spotted at (" <<p.getX() << ", " << p.getY() << ")\n"; 
}

void DumbTank::markShell(Position p)
{
	shellPosition = p;
	shellSpottedThisTick = true;
	shellGhostValidCounter++;

	// updates least resent past location
	HostileShellPastLocations[0].x = p.getX();
	HostileShellPastLocations[0].y = p.getY();
	// moves the array of 5 past locations along 1 so the most recent update replaces the oldest
	for (int i = 4; i >= 1; i--) {
		HostileShellPastLocations[i] = HostileShellPastLocations[i - 1];
		//std::cout << "Location of hostile shell: " << HostileShellPastLocations[i].x << HostileShellPastLocations[i].y << std::endl;
	}
}

bool DumbTank::isFiring()
{
	return false;
} 

void DumbTank::score(int thisScore,int enemyScore)
{
	std::cout << "MyScore: " << thisScore << "\tEnemy score: " << enemyScore << std::endl;
}


//Alex's utillity functions please use but dont mess with unless your Jo fixing math, ps Jo please save this
sf::Vector2f DumbTank::getPredictedPosition(sf::Vector2f PastLocationsIn[5], float timeInTicks) {
	//Get the difference betweeen the past 5 positions
	sf::Vector2f differenceInPositions[5];
	for (int i = 0; i <= 4; i++) {
		differenceInPositions[i] = PastLocationsIn[i + 1] - PastLocationsIn[i];
	}
	//Get the average of the differences
	sf::Vector2f averageMovementPerTick;
	averageMovementPerTick.x = (differenceInPositions[0].x + differenceInPositions[1].x + differenceInPositions[2].x + differenceInPositions[3].x + differenceInPositions[4].x) / 5;
	averageMovementPerTick.y = (differenceInPositions[0].y + differenceInPositions[1].y + differenceInPositions[2].y + differenceInPositions[3].y + differenceInPositions[4].y) / 5;

	//Get the average of the differences
	sf::Vector2f predictedChangeInPosition;
	predictedChangeInPosition = averageMovementPerTick * timeInTicks;

	return(predictedChangeInPosition);
}

float DumbTank::getTicksToImpact(sf::Vector2f theirTankPositionIn, bool shellIn) {
	//get distance between us and them 
	sf::Vector2f differenceInPosition;
	differenceInPosition.x = pos.getX() - theirTankPositionIn.x;
	differenceInPosition.y = pos.getY() - theirTankPositionIn.y;

	//turn those values positive (direction doesnt matter and it messes up the maths)
	if (differenceInPosition.x < 0) {
		differenceInPosition.x = fabs(differenceInPosition.x);
	}
	if (differenceInPosition.y < 0) {
		differenceInPosition.y = fabs(differenceInPosition.y);
	}

	//get ticks taken 
	float ticksTakenToImpact;
	if (shellIn) {
		ticksTakenToImpact = (sqrt(pow(differenceInPosition.x,2) + pow(differenceInPosition.y,2))) / 3;
	}
	if (!shellIn) {
		ticksTakenToImpact = (sqrt(pow(differenceInPosition.x,2) + pow(differenceInPosition.y,2))) / 1.7;
	}


	return(ticksTakenToImpact);
}

void DumbTank::lookToIdeal(sf::Vector2f idealLocationIn) {
	float angleToTarget = getChangeInAngleToTarget(idealLocationIn,true);
	if (angleToTarget < 0.5 && angleToTarget > -0.5) {
		stopTurret();
	} else {
		if (angleToTarget < 0) {
			turretGoLeft();
		}
		if (angleToTarget > 0) {
			turretGoRight();
		}
	}
}


void DumbTank::moveToIdeal(sf::Vector2f idealLocationIn) {
	stop();
	float distance = getDistanceToTarget(idealLocationIn);

	//get angle to target
	float angleToLocation;
	angleToLocation = getChangeInAngleToTarget(idealLocationIn, false);

	//check if angle to ideal location is great enough for it to be worth going backwards
	if (angleToLocation >= -90 && angleToLocation <= 90) {
		forwardIsEfficient = true;
		std::cout << "forwards" << std:: endl;
	}
	else {
		forwardIsEfficient = false;
		std::cout << "backwards" << std::endl;
	}

	std::cout << angleToLocation << std::endl;

	if (distance > 1.7) {
		if (turnOrMove) {
			if (forwardIsEfficient) {
				//if its left of us move tank body left
				if (angleToLocation < 1.5 && angleToLocation > -1.5) {
					stop();
				}
				else {
					if (angleToLocation < 0) {
						goLeft();
					}
					//if its right of us move tank body right
					if (angleToLocation > 0) {
						goRight();
					}
				}
			}
			else {

				//if (angleToLocation)
				if (angleToLocation  < 181.5 && angleToLocation > 178.5) {				// if it looks right to reverse to something it passes by 360 and fucks its logic
					stop();
				}
				else {
					//if its left of us move tank body right because we are going backwards
					if (angleToLocation > 180) {
						goRight();
					}
					//if its right of us move tank body left because we are going backwards
					if (angleToLocation < 180) {
						goLeft();
					}
				}
			}
		}
		if (!turnOrMove) {
			//if our current target is in front of us more than it is behind us go forward, else go backward
			if (forwardIsEfficient) {
				stop();
				goForward();
			}
			else {
				stop();
				goBackward();
			}
		}
	}
	else {
		stop();
	}

	if ((angleToLocation + 180) <= 181.5 && (angleToLocation + 180) >= 178.5) {
		turnOrMove = false;
	}
	else if (angleToLocation < 181.5 && angleToLocation > 178.5) {
		turnOrMove = false;
	}
	else {
		turnOrMove = !turnOrMove;
	}
}


float DumbTank::getChangeInAngleToTarget(sf::Vector2f targetPositionIn, bool isTurret) {
	//get difference in positions
	float dX = targetPositionIn.x - pos.getX();
	float distance = getDistanceToTarget(targetPositionIn);

	//get angle to ideal location
	float angleOfIncident;
	angleOfIncident = (acos(dX / distance));
	angleOfIncident = angleOfIncident * (180 / 3.14);
	float angleToLocation;

	if (pos.getY() > targetPositionIn.y) {
		angleToLocation = 360 - angleOfIncident;
	}
	if (pos.getY() < targetPositionIn.y) {
		angleToLocation = angleOfIncident;
	}
	// get the difference in angle (capable of being negative as that means to go left)
	float differenceInAngle;
	if (isTurret) {
		differenceInAngle = angleToLocation - turretTh;
	} else {
		differenceInAngle = angleToLocation - pos.getTh();
	}

	//std::cout << "angleToLocation: " << angleToLocation << std::endl;
	//std::cout <<"turretTh: " << turretTh << std::endl;
	//std::cout << "angleOfIncident: " << angleOfIncident << std::endl;

	return(differenceInAngle);
}

bool DumbTank::willTheseCollideInN(sf::Vector2f PositionIn, sf::Vector2f PastLocationsInOfProjectile[5], float ticksIn) {
	//get predicted position 
	sf::Vector2f predictedPosition;
	predictedPosition = getPredictedPosition(PastLocationsInOfProjectile, ticksIn);
	
	if (predictedPosition.x >= PositionIn.x - 50 && predictedPosition.x <= PositionIn.x + 50 && predictedPosition.y >= PositionIn.y - 50 && predictedPosition.y <= PositionIn.y + 50) {
		return true;
	} else {
		return false;
	}
}

float DumbTank::getDistanceToTarget(sf::Vector2f target)
{
	//get difference in positions
	float dX, dY;
	dX = target.x - pos.getX();
	dY = target.y - pos.getY();

	//get distence between us and the ideal location
	float distance = sqrt(pow(dX, 2) + pow(dY, 2));
	return distance;
}
//static const int patrolWidth = 2, patrolHeight = 4;
//const float mapHeight = 570.0f, mapWidth = 780.0f;
void DumbTank::calculatePatrolPoints() {
	float xSpacer = mapWidth / (patrolWidth * 4);
	float ySpacer = mapHeight / (patrolHeight * 2);

	for (int i = 0; i < patrolHeight; i++) {
		for(int j = 0; j < patrolWidth; j++){
			
			//X calculation
			if ((i == 0) || (i == (patrolHeight - 1))) {
				patrolRoute[i][j].x = ((j * (xSpacer * 2)) + (xSpacer * 3));
			}
			else {
				patrolRoute[i][j].x = ((j * (xSpacer * 6)) + xSpacer);
			}

			//Y calculation
			patrolRoute[i][j].y = ((i * (ySpacer * 2)) + ySpacer);
		}
	}
}

void DumbTank::tankPatrolRoute()
{
	//Once arrived, set current node as previous node
	if (getDistanceToTarget(currentPatrolNode) < 15) {
		getNextPatrolNode();
		std::cout << "ARRIVED" << std::endl;
	}

	moveToIdeal(currentPatrolNode);
}

void DumbTank::getNextPatrolNode()
{
	for (int i = 0; i < patrolHeight; i++) {
		for (int j = 0; j < patrolWidth; j++) {
			if (currentPatrolNode == patrolRoute[i][j]) {
				int tempX, tempY;
				//x calculation
				if (i == 0) {
					tempX = j - 1;
					if (tempX < 0)
						tempX = 0;
				}
				else if (i == (patrolHeight - 1)) {
					tempX = j + 1;
					if (tempX > (patrolWidth - 1))
						tempX = patrolWidth - 1;
				}
				else tempX = j;

				//y calculation
				if (j == 0) {
					tempY = i + 1;
					if (tempY > (patrolHeight - 1))
						tempY = patrolHeight - 1;
				}
				else if (j == (patrolWidth - 1)) {
					tempY = i - 1;
					if (tempY < 0)
						tempY = 0;
				}
				else tempY = i;

				std::cout << "[" << tempX << "][" << tempY << "]" << std::endl;
				previousPatrolNode = currentPatrolNode;
				currentPatrolNode = patrolRoute[tempY][tempX];
				return;
			}
		}
	}
}
