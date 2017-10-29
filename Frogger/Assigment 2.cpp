#include <TL-Engine.h>
#include <iostream>	

#include <unordered_map>
using namespace tle;





// Contains all maths required for the game
namespace Maths
{
	// This vec3 class is used for basic vector mathmatics
	class vec3
	{
	public:
		float x, y, z;
		//Constructors
		vec3();
		vec3(float _x, float _y, float _z);
		float length(); // Gets the length of the vec3's
		vec3 normalize(); // normalizes the vec3's
		float dot(vec3& vec); // Gets the dot product between 2 vec3's
		float distanceBetween(vec3& vec); // Calculates the distance between 2 vec3's

		// Overload operator + operator to add 2 vec3's together
		vec3 vec3::operator+(const vec3& vec)
		{
			return vec3(this->x + vec.x, this->y + vec.y, this->z + vec.z);
		}

		// Overload operator - operator to minus 2 vec3's together
		vec3 vec3::operator-(const vec3& vec)
		{
			return vec3(this->x - vec.x, this->y - vec.y, this->z - vec.z);
		}

		// Overload operator + operator to add 2 vec3's together
		void vec3::operator+=(const vec3& vec)
		{
			this->x += vec.x;
			this->y += vec.y;
			this->z += vec.z;
		}

		// Overload operator - operator to minus 2 vec3's together
		void vec3::operator-=(const vec3& vec)
		{
			this->x -= vec.x;
			this->y -= vec.y;
			this->z -= vec.z;
		}
	};

	// Vec3 Constructor
	vec3::vec3()
	{
		x = 0.0f, y = 0.0f, z = 0.0f;
	}

	// Vec3 Constructor with parameters
	vec3::vec3(float _x, float _y, float _z)
	{
		x = _x, y = _y, z = _z;
	}

	// Used to calculate the length of the vec3
	float vec3::length()
	{
		return sqrt(x*x + y*y + z*z);
	}

	// Used to normalise the vec3 so its length == 1
	vec3 vec3::normalize()
	{
		vec3 vec;
		float len = length();
		if (len != 0)
		{
			vec.x = x / len;
			vec.y = y / len;
			vec.z = z / len;
		}
		return vec;
	}

	// Gets the dot product between 2 vec3's
	float vec3::dot(vec3& vec)
	{
		return	x*vec.x + y*vec.y + z*vec.z;
	}

	// Calculates the distance between 2 vec3's
	float vec3::distanceBetween(vec3& vec)
	{
		float xa = x - vec.x, ya = y - vec.y, za = z - vec.z;
		return sqrt(xa*xa + ya*ya + za*za);
	}

	// This class is used to store all bounding box code
	class BoundingBox
	{
	public:
		float width, height, length, x, y, z; // Bouding box's x, ,y ,width and height
		int dimensions; // Bounding box's dimensions
		// Constructors used to initilize the class
		BoundingBox(float _width, float _length);
		BoundingBox(float _x, float _y, float _width, float _length);
		BoundingBox(float _x, float _y, float _z, float _width, float _height, float _length);
	};
	// Class initialization constructor
	BoundingBox::BoundingBox(float _width, float _height)
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		width = _width;
		height = _height;
		length = 0.0f;
		dimensions = 2;
	}
	// Class initialization constructor
	BoundingBox::BoundingBox(float _x, float _z, float _width, float _height)
	{
		x = _x;
		y = 0.0f;
		z = _z;
		width = _width;
		height = _height;
		length = 0.0f;
		dimensions = 2;
	}
	// Class initialization constructor
	BoundingBox::BoundingBox(float _x, float _y, float _z, float _width, float _height, float _length)
	{
		x = _x;
		y = _y;
		z = _z;
		width = _width;
		height = _height;
		length = _length;
		dimensions = 3;
	}

	// This function is used for sphere in box colision detection.
	// It makes sure a sphere never leaves a box at all
	bool sphereInBox(vec3 sphereLocation, float sphereRad, BoundingBox box)
	{
		if (box.dimensions == 2) // is 2D bounding box
		{
			if (box.x > sphereLocation.x - sphereRad || // x
				box.x + box.width < sphereLocation.x + sphereRad || // x + width
				box.z > sphereLocation.z - sphereRad || // y
				box.z + box.height< sphereLocation.z + sphereRad) // y + height
			{
				return false;
			}
		}
		return true;
	}
	// This function is used for point to box colison detection.
	// It checks to see if a point is actualy inside a box
	bool pontToBox(vec3 sphereLocation, BoundingBox box)
	{
		if (box.dimensions == 2)
		{
			if (box.x < sphereLocation.x && // x
				box.x + box.width > sphereLocation.x&& // x + width
				box.z < sphereLocation.z && // y
				box.z + box.height > sphereLocation.z // y + height
				)
			{
				return true;
			}
		}
		return false;
	}
	// This function is used for sphereToBox colisoon detection.
	// It checks to see if a sphere is inside a bouding box utilizing point to box
	bool sphereToBox(vec3 sphereLocation, float sphereRad, BoundingBox box)
	{
		if (box.dimensions == 2)
		{
			// Modifying the local instance of 'box' to incorporate the radius.
			box.x -= sphereRad / 4;
			box.z -= sphereRad / 4;
			box.width += sphereRad;
			box.height += sphereRad;
			return pontToBox(sphereLocation, box);
		}
		return false;
	}
	// This function is used to check is a point is inside a sphere
	bool pointToSphere(Maths::vec3 spherePosition, float sphereRad, Maths::vec3 pointPosition)
	{
		float xa, ya, za;
		xa = spherePosition.x - pointPosition.x;
		ya = spherePosition.y - pointPosition.y;
		za = spherePosition.z - pointPosition.z;
		float result = sqrtf(xa*xa + ya*ya + za*za); // Uses Pythagoras's Therom to get the length of the hypotinuse between the 2 vectors
		return result < sphereRad; // Check to see if the length is less then the rad
	}
	// This function is used for sphere to sphere colison detection using point to sphere as a inheritance.
	bool sphereToSphere(Maths::vec3 spherePosition1, float sphereRad1, Maths::vec3 spherePosition2, float sphereRad2)
	{
		return pointToSphere(spherePosition1, sphereRad1 + sphereRad2, spherePosition2);
	}

}

const EKeyCode kMoveLeft = Key_Z; // Key for moving left
const EKeyCode kMoveRight = Key_X; // Key for moving right
const EKeyCode kMoveForward = EKeyCode(192); // Key for moving forward
const EKeyCode kMoveBack = EKeyCode(191); // Key for moving backward
const EKeyCode kPause = Key_P; // Key for pausing the game
const EKeyCode kExit = Key_Escape; // Key for leaving the game
const EKeyCode kCameraPanUp = Key_Up; // Key for moving the camera axis up
const EKeyCode kCameraPanDown = Key_Down; // Key for moving the camera axis down
const EKeyCode kCameraArcReset = Key_C; // Key for reseting the camera axis
const EKeyCode kGameReset = Key_Space; // Key for reseting the game

// File and folder paths required for models and resource loading
const char* kMediaLocation = "./Media";
const char* kFont = "Comic Sans MS";

const char* kSkyboxFileLocation = "skybox.x";
const char* kSurfaceFileLocation = "surface.x";
const char* kIsland1FileLocation = "island1.x";
const char* kIsland2FileLocation = "island2.x";
const char* kFrogFileLocation = "frog.x";
const char* kDummyFilePath = "Dummy.x";
const char* kTyreFilePath = "tyre.x";
const char* kPlantFilePath = "plant.x";

const float kGameSpeed = 0.30f; // Used to sync everything up to, the higher it is the faster the game logic works
const float kRequestedFPS = 60; // FPS you want the game to run at
const float kFrameTime = 1.0f / kRequestedFPS; // Time in microsecconds a frame should last for
const float kRightAngle = 90.0f; // Right angle in degrees
const float kSpeedLane1 = 0.5f; // Speed of trafic in lane 1
const float kSpeedLane2 = kSpeedLane1 * 2; // Speed of trafic in lane 2
const float kSpeedLane3 = kSpeedLane1 * 3; // Speed of trafic in lane 3
const float kSpeedLane4 = kSpeedLane1 * 4; // Speed of trafic in lane 4
const float kMapWidth = 120.0f; // Game Map width
const float kWaterAreaHeight = 40.0f; // Water map area height
const float kMapIsland1Height = 60.0f; // Island map area height
const float kMapHeight = 110.0f; // Overall Map height
const float kFrogRad = 4.5f;// Frogs radious
const float kEpsilon = 0.0001f; // used in equals calulations with floats
const float kTileSize = 10.0f; //Size of a tile in the game
const float kFrogJumpDistance = kTileSize; // How far the frog should jump
const float kFrogJumpHeight = 10.0f; // How height the frog should jump
const float kFrogRestingY = 0.0f; // Where the frog should be when not moving on the Y axis
const float kPI = acosf(-1); // PI
const float kCarRespawnTimer = 2.0f; // How long before a car should respawn
const float kCarSurface = 0.0f; // Where a car should be on the Y axis
const float kCarUnderGround = -100.0f; // Where a car should be when not visible
const float kFrogCrossingTime = 20.0f; // how long a player has before the frog dies from a timeout
const float kTyreScale = 10.0f; // How much a tyre should be scaled up by
const float kTyreRad = 0.45f * kTyreScale; // calulate what the tyre radious should be
const float kTyreSpeed = 0.5f; // Calculate how fast a tyre should go
const float kTreeRad = 3.0f; // How wide a tree is

const float kCameraMaxPanUp = 30.0f; // Camera max pan
const float kCameraMinPanDown = -10.0f; // Camera pan min
const float kCameraPanIncrement = 0.15f; // Camera pan increment
const float kCameraDefaultArc = 0.0f; // Camera default arc
const float kFrogDrowndingSpeed = 0.1f; // How fast the frog should sink when drownding

const int kFontSize = 36; // Font size

const char* kCarSkins[5] = { "transit.x" , "rover.x" ,"truck.x","rangeRover.x","pickUp.x"}; // Array that stores all trafic

// Aray that contains all bounding box's for all various car models
const Maths::BoundingBox* kCarBoundingBoxs[5] = {
	new Maths::BoundingBox(12.2432f,1.65308f),
	new Maths::BoundingBox(11.8138f,1.58666f),
	new Maths::BoundingBox(55.6999f,1.316057f),
	new Maths::BoundingBox(11.06804f,1.36512f),
	new Maths::BoundingBox(12.29478f,1.16196f)
};

float gameClock = 0.0f; // Seconds passed in the game
float lastFrame = 0.0f; // How long the last frame took to process
float cameraArc = 0.0f; // The current camera arc
float gameTimeCountdown = kFrogCrossingTime; // Game countdown

int fps = 0; // Current FPS
int currentFrogIndex; // Current Frog in array
int score = 0; // Overall score

IMesh* tyreMesh; // Mesh tat stores the tyres
ICamera* camera; // TLEngine Camera

const Maths::BoundingBox kGameArea(-60.0f, 10.0f, kMapWidth, kMapHeight); // Game area bounding box, used for frog , car and tyre colison
const Maths::BoundingBox kWaterArea(-60.0f, 10.0f + kMapIsland1Height, kMapWidth, kWaterAreaHeight); // Wanter bounding box, so we know when the frog should be attached to a tyre
const Maths::BoundingBox kWinArea(-60.0f, 10.0f + kMapIsland1Height + kWaterAreaHeight, kMapWidth, kTileSize); // The bounding box where the player will land when they win

I3DEngine* myEngine; // TL Engine implimentation

unordered_map<std::string, IMesh*> meshPointers; // All mesh pointers all stored in a map

enum Direction { Forward = 0,  Right = 1 , Back = 2, Left = 3}; // Directions objects can move in. Values hardcoded as maths is done with them
enum FrogState { Waiting, Crossing, Safe, Dead }; // Frog crossing states
enum GameState { Playing, Paused, Over }; // Game states
enum CarType { Transit, Rover , Truck, RangeRover, PickUp}; // Various car types
enum CarState {Driving, Respawning}; // Car states

GameState currentGameState; // The current game state



//  This class is used to store all data regarding the world object and inherits from MAths::vec3
class Entity : public Maths::vec3
{
public:
	// Constructor for Entity
	Entity(std::string _meshFileLocation, float x, float y, float z) : vec3(x, y, z)
	{
		meshFileLocation = _meshFileLocation;
		createModel(x, y, z);
	}
	void recalcPosition(); // Used to get data from TLEngine about the models position
	IModel* getModel(); // Gets the entitys model
private:
	std::string meshFileLocation;
	// Initilizes and creates the model
	void createModel(float x, float y, float z);
protected:
	IModel* entityModel;
};

vector<Entity> trees; // Will store the tree model entitys

 // Used to get data from TLEngine about the models position
void Entity::recalcPosition()
{
	x = entityModel->GetX();
	y = entityModel->GetY();
	z = entityModel->GetZ();
}
// Gets the entitys model
IModel* Entity::getModel()
{
	return entityModel;
}

// Initilizes and creates the model
void Entity::createModel(float x, float y, float z)
{
	IMesh* entityMesh;
	std::unordered_map<std::string, IMesh*>::iterator it = meshPointers.find(meshFileLocation);
	if (it != meshPointers.end())
	{
		entityMesh = meshPointers[meshFileLocation];
	}
	else
	{
		entityMesh = myEngine->LoadMesh(meshFileLocation);
		meshPointers[meshFileLocation] = entityMesh;
	}
	entityModel = entityMesh->CreateModel(x, y, z);
}

// This class stores all data regaring the tyre chain
class TyreChain
{
public:
	// Constructor to define information about the chain such as position and length
	TyreChain(float x, float y, float z, Direction _direction = Direction::Left, int _chainLength = 1)
	{
		chainLength = _chainLength;
		direction = _direction;
		initilize(x, y, z);
		setDirection(direction);
	}
	// Direction the chain will be floating in
	void setDirection(Direction _direction);
	// Called each frame to move the chain forward
	void moveForward();
	vector<Entity*> getChain();
	vector<Entity*> getChainDummys();
private:
	// initilized all tyres in chain
	void initilize(float x, float y, float z);
	vector<Entity*> frogAttachDummyList;
	Direction direction;
	vector<Entity*> tyreChain;
	int chainLength;
};

// Direction the chain will be floating in
void TyreChain::setDirection(Direction _direction)
{
	direction = _direction;
}

// Called each frame to move the chain forward
void TyreChain::moveForward()
{
	for (int i = 0; i < chainLength; i++)
	{
		Entity* currentEntity = tyreChain[i];
		currentEntity->getModel()->MoveLocalX(((kGameSpeed / kTyreScale) * kTyreSpeed) * ((direction - 2)*-1));
		if (!sphereInBox(*currentEntity, kTyreRad, kGameArea))
		{
			currentEntity->getModel()->SetX((kGameArea.x + kTyreRad) * ((direction - 2)*-1));
			setDirection(direction);
		}
		currentEntity->recalcPosition();
	}
}

// initilized all tyres in chain
void TyreChain::initilize(float x, float y, float z)
{
	for (int i = 0; i < chainLength; i++)
	{
		Entity* tyre = new Entity(kTyreFilePath, x + (i * kTileSize), y, z);
		tyre->getModel()->Scale(kTyreScale);
		tyreChain.push_back(tyre);
		Entity* frogAttachDummy = new Entity(kDummyFilePath, 0.0f, 0.0f, 0.0f);
		frogAttachDummy->getModel()->Scale(1.0f/kTyreScale);
		frogAttachDummyList.push_back(frogAttachDummy);
		frogAttachDummy->getModel()->AttachToParent(tyre->getModel());
	}
}

// Gets all Tyres in chain
vector<Entity*> TyreChain::getChain()
{
	return tyreChain;
}
 // Gets all Tyres dummy models in chain
vector<Entity*> TyreChain::getChainDummys()
{
	return frogAttachDummyList;
}


vector<TyreChain> tyres; // Define Array that will store Tyres

// This class is used to define all information about the Frog and pulls in functions from the entity class
class Frog : public Entity
{
public:
	// Frog constructor
	Frog(std::string _meshFileLocation, float x, float y, float z, FrogState _currentState = FrogState::Waiting) : Entity(_meshFileLocation, x, 0.0f, 0.0f)
	{
		startingPosiiton = vec3(x, y, z); // Where the frog started from
		movmentDummy = new Entity(kDummyFilePath, 0.0f, y, z);
		cameraDummy = new Entity(kDummyFilePath, 0.0f, 0.0f, 0.0f);
		currentState = _currentState;
		setState(currentState);
		cameraDummy->getModel()->AttachToParent(movmentDummy->getModel());
		attachToParent(movmentDummy);
		isMoving = false;
		lastScorePosition = 0.0f;//Last positon on the z axis inwhich the user scores a x points
		drowned = false;
	}
	void tick(); // Update frog movment
	void move(Direction _direction); // Request from movemnt
	void setState(FrogState _currentState); // Set frogs current state
	void attachCamera(ICamera* camera); // attach the camera to the frogs camera dummy
	FrogState getCurrentState(); // returns the current state of the frog
	vec3 startingPosiiton;
	bool drowned;
private:
	// Attach the entity to the frog
	void attachToParent(Entity* _parent);
	FrogState currentState;
	Direction direction;
	bool isMoving;
	// What direction the frog is moving
	Direction movmentDirection; // Will be initilized when required
	// How far left does the frog need to move in the jump
	float distanceLeftOfJump; // Will be initilized when required
	float lastScorePosition;
	Entity* movmentDummy;
	Entity*	cameraDummy;
	Entity* parent;
};


vector<Frog*> frogs; // Will store frogs used in game

// Update frog movment and tyre colison code
void Frog::tick()
{
	cameraDummy->getModel()->ResetOrientation();
	cameraDummy->getModel()->RotateX(cameraArc);
	if (currentState == FrogState::Safe) return; //  if frog is safe continue
	if (currentState == FrogState::Dead && drowned) // if frog is dead by drownding make it sink
	{
		movmentDummy->getModel()->MoveY(-kFrogDrowndingSpeed);
	}
	else if (isMoving) // Check to see if the frog should be moving
	{
		float speed = kGameSpeed; // Set a base frog movment speed
		float distanceProgress = distanceLeftOfJump / kFrogJumpDistance; // Work out how far the frog has moved in range of 0-1
		if (speed > distanceLeftOfJump) // Shoudl be ending move cycle
		{
			speed = distanceLeftOfJump; //Calc final jump distance
			entityModel->SetY(kFrogRestingY); // move frog to 0.0f
			isMoving = false;
			if (lastScorePosition + (kTileSize - kEpsilon) < movmentDummy->getModel()->GetZ()) // Should add 10 points to users score
			{
				score += 10;
				lastScorePosition = movmentDummy->getModel()->GetZ();
			}
			//Check that the frog is in the area of the water
			if (sphereInBox(*this,
				kFrogRad, kWaterArea))
			{
				
				bool onTyre = false; // Used to see if the frog is on a tyre
				for (int i = 0; i < tyres.size(); i++) // Loop through all tyre chains
				{
					vector<Entity*> tyreList = tyres[i].getChain();
					vector<Entity*> tyreDummyList = tyres[i].getChainDummys();
					for (int x = 0; x < tyreList.size(); x++) // Loop through all tyres in list
					{
						Entity* tyre = tyreList[x];
						Entity* tyreDummy = tyreDummyList[x];
						if (Maths::sphereToSphere(*tyre, kTyreRad, *this, kFrogRad)) // Frog is on a tyre and should be attached to it
						{
							attachToParent(tyreDummy); // Attach frog to dummy
							getModel()->SetLocalPosition(0.0f, 0.0f, 0.0f);
							onTyre = true;
						}
					}
				}
				if (!onTyre) // did not land on tyre, is so kill it
				{
					setState(FrogState::Dead);
					drowned = true;
					if (parent != movmentDummy)
					{
						float currentX = parent->getModel()->GetX();
						attachToParent(movmentDummy);
						getModel()->SetX(currentX);
					}
					currentFrogIndex++;
					gameTimeCountdown = kFrogCrossingTime;
					if (currentFrogIndex < frogs.size())frogs[currentFrogIndex]->attachCamera(camera);
				}
				
			}

		}
		else
		{
			entityModel->SetY(kFrogJumpHeight*sinf(kPI * (distanceLeftOfJump/ kFrogJumpDistance))); // Move frog x distance based on game speed
		}
		distanceLeftOfJump -= speed;
		movmentDummy->getModel()->MoveZ(movmentDirection % 2 == 0 ? speed * ((movmentDirection - 1)*-1) : 0.0f);
		entityModel->MoveX(movmentDirection % 2 == 1 ? speed * ((movmentDirection - 2)*-1) : 0.0f);
		recalcPosition();

		// if frog is in win area, set it to blue and switch frog
		if (sphereInBox(*this,
			kFrogRad, kWinArea))
		{
			setState(FrogState::Safe);
			currentFrogIndex++;
			gameTimeCountdown = kFrogCrossingTime;
			if (currentFrogIndex < frogs.size())frogs[currentFrogIndex]->attachCamera(camera);
		}
	}
}

// Choose which direction you wish to move the frog in
void Frog::move(Direction _direction)
{
	direction = _direction;
	if (isMoving) return;
	bool canMove = true; // We need to check to see if the frog will collide with anything
	for (int i = 0; i < trees.size(); i++)
	{
		Entity tree = trees[i];
		// Make sure the frog has not collided with any trees
		if (sphereToSphere(Maths::vec3(entityModel->GetX() + (direction % 2 == 1 ? (kTileSize - kEpsilon)*((direction - 2)*-1) : 0.0f),
			entityModel->GetY(),
			entityModel->GetZ() + (direction % 2 == 0 ? (kTileSize - kEpsilon)*((direction - 1)*-1) : 0.0f)),
			kFrogRad, tree, kTreeRad))
		{
			canMove = false;
		}
	}
	// If the frog is inside the game box then it can condinue moving
	if (!sphereInBox(Maths::vec3(entityModel->GetX() + (direction % 2 == 1 ? (kTileSize - kEpsilon)*((direction - 2)*-1) : 0.0f),
		entityModel->GetY(),
		entityModel->GetZ() + (direction % 2 == 0 ? (kTileSize - kEpsilon)*((direction - 1)*-1) : 0.0f)),
		kFrogRad, kGameArea))
	{

		canMove = false;
	}
	if (canMove)
	{
		// Initilize all movnent code that will be handled by tick()
		isMoving = true;
		entityModel->ResetOrientation();
		entityModel->RotateLocalY(kRightAngle * direction);
		movmentDirection = direction;
		distanceLeftOfJump = kFrogJumpDistance;
		if (direction == Direction::Back || direction == Direction::Forward)
		{
			if (parent != movmentDummy)
			{
				float currentX = parent->getModel()->GetX();
				attachToParent(movmentDummy);
				getModel()->SetX(currentX);
			}
		}
	}
	recalcPosition();
}

//Set the frogs current stte
void Frog::setState(FrogState _currentState)
{
	currentState = _currentState;
	switch (currentState)
	{
	case FrogState::Crossing:
	case FrogState::Waiting:
		entityModel->SetSkin("frog.bmp");
		break;
	case FrogState::Dead:
		entityModel->SetSkin("frog_red.bmp");
		break;
	case FrogState::Safe:
		entityModel->SetSkin("frog_blue.bmp");
		break;
	}
}

// Attach the camera to the frog
void Frog::attachCamera(ICamera* camera)
{
	camera->AttachToParent(cameraDummy->getModel());
}

//Attach the frog to the parent spesified
void Frog::attachToParent(Entity* _parent)
{
	parent = _parent;
	getModel()->AttachToParent(parent->getModel());
}

// get the frogs current state
FrogState Frog::getCurrentState()
{
	return currentState;
}

// Class that contains all the cars code for movment etc.
class Vehicle : public Entity
{
public:
	// Car class constructor
	Vehicle(CarType _carType, float x, float y, float z, Direction _direction = Direction::Left, float _speedScale = 1.0f) : Entity(kCarSkins[_carType], x, y, z)
	{
		setDirection(_direction);
		speedScale = _speedScale;
		carType = _carType;
		respawnCountDown = kCarRespawnTimer;
		carState = CarState::Driving;
		boundingBox = new Maths::BoundingBox(x,z,kCarBoundingBoxs[_carType]->width, kCarBoundingBoxs[_carType]->height);
	}
	// Sets the cars diection
	void setDirection(Direction _direction);
	void moveForward(); // Mvoes the car forward on its local plane
	void setSpeedScale(float _speedScale); // Sets the cars speed scale
	CarState getCarState(); // Sets the cars current state
	Maths::BoundingBox* boundingBox;
private:
	void updateBoundingBox(); // Update the cars bouding box
	CarState carState;
	float respawnCountDown;
	float speedScale;
	Direction direction;
	CarType carType;
};

// Set the cars current state
void Vehicle::setDirection(Direction _direction)
{
	entityModel->ResetOrientation();
	direction = _direction;
	entityModel->RotateLocalY(kRightAngle * _direction);
}

// Move the car forawrd along it local plane
void Vehicle::moveForward()
{
	entityModel->MoveLocalZ(kGameSpeed * speedScale);
	recalcPosition();
	updateBoundingBox();
	float carLengthHalfed = kCarBoundingBoxs[carType]->height / 2;
	if (!sphereInBox(*this, carLengthHalfed, kGameArea))
	{
		carState = CarState::Respawning;
		respawnCountDown -= lastFrame;
		entityModel->RotateLocalX(1.0f);
	}
	if (respawnCountDown < 0.0f)
	{
		carState = CarState::Driving;
		respawnCountDown = kCarRespawnTimer;
		entityModel->SetX((kGameArea.x + carLengthHalfed) * ((direction - 2)*-1));
		entityModel->SetY(kCarSurface);
		setDirection(direction);
	}
}

// Set the cars speed scale
void Vehicle::setSpeedScale(float _speedScale)
{
	speedScale = _speedScale;
}

// get the cars state
CarState Vehicle::getCarState()
{
	return carState;
}

// updae the cars bounding box with new x,y and z posiitons
void Vehicle::updateBoundingBox()
{
	boundingBox->x = entityModel->GetX();
	boundingBox->y = entityModel->GetY();
	boundingBox->z = entityModel->GetZ();
}

// Set up all frogs with neseserry data
void initilizeEntitys()
{
	score = 0;
	currentFrogIndex = 0;
	frogs.clear(); // Clear frogs incase in seccond + rounds
	frogs.push_back(new Frog(kFrogFileLocation, -10.0f, 0.0f, 15.0f, FrogState::Crossing));
	frogs.push_back(new Frog(kFrogFileLocation, 0.0f, 0.0f, 15.0f));
	frogs.push_back(new Frog(kFrogFileLocation, 10.0f, 0.0f, 15.0f));

	frogs[0]->attachCamera(camera);
}

// All main function code
void main()
{
	//initilzie engine
	myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed(); // open window

	//Select games media folder
	myEngine->AddMediaFolder(kMediaLocation);

	//Initilize games meshes
	IMesh* skyBoxMesh = myEngine->LoadMesh(kSkyboxFileLocation);
	IMesh* surfaceMesh = myEngine->LoadMesh(kSurfaceFileLocation);
	IMesh* islandOneMesh = myEngine->LoadMesh(kIsland1FileLocation);
	IMesh* islandTwoMesh = myEngine->LoadMesh(kIsland2FileLocation);
	IMesh* plantMesh = myEngine->LoadMesh(kPlantFilePath);

	// Create games models
	IModel* skyBoxModel = skyBoxMesh->CreateModel(0.0f, -1000.0f, 0.0f);
	IModel* surfaceModel = surfaceMesh->CreateModel(0.0f, -2.5f, 0.0f);
	IModel* islandOneModel = islandOneMesh->CreateModel(0.0f, -5.0f, 40.0f);
	IModel* islandTwoModel = islandTwoMesh->CreateModel(0.0f, -5.0f, 115.0f);

	//Trees
	trees.push_back(Entity(kPlantFilePath, -50.0f, 0.0f, 115.0f));
	trees.push_back(Entity(kPlantFilePath, -30.0f, 0.0f, 115.0f));
	trees.push_back(Entity(kPlantFilePath, -10.0f, 0.0f, 115.0f));
	trees.push_back(Entity(kPlantFilePath, 10.0f, 0.0f, 115.0f));
	trees.push_back(Entity(kPlantFilePath, 30.0f, 0.0f, 115.0f));
	trees.push_back(Entity(kPlantFilePath, 50.0f, 0.0f, 115.0f));

	// Initilzie camera's poition
	camera = myEngine->CreateCamera(kManual, 0.0f, 40.0f, -60.0f);
	camera->RotateLocalX(20.0f);

	
	// Create 2 frogs that will be underground, this is to pre load the textures into the game so it wont cause any freez's
	new Frog(kFrogFileLocation, -20.0f, -100.0f, 15.0f, FrogState::Dead);
	new Frog(kFrogFileLocation, -30.0f, -100.0f, 15.0f, FrogState::Safe);

	vector<Vehicle> vehicles; // Will store all cars in this vector
	// Cars that will be in there various lains
	//Lane 1
	vehicles.push_back(Vehicle(CarType::PickUp, -50.0f, 0.0f, 25.0f, Direction::Right, kSpeedLane1));
	vehicles.push_back(Vehicle(CarType::RangeRover, 0.0f, 0.0f, 25.0f, Direction::Right, kSpeedLane1));
	//Lane 2
	vehicles.push_back(Vehicle(CarType::Transit, 50.0f, 0.0f, 35.0f, Direction::Left, kSpeedLane2));
	vehicles.push_back(Vehicle(CarType::PickUp, 10.0f, 0.0f, 35.0f, Direction::Left, kSpeedLane2));
	vehicles.push_back(Vehicle(CarType::Transit, -20.0f, 0.0f, 35.0f, Direction::Left, kSpeedLane2));
	//Lane 3
	vehicles.push_back(Vehicle(CarType::Rover, 20.0f, 0.0f, 45.0f, Direction::Right, kSpeedLane3));
	vehicles.push_back(Vehicle(CarType::Rover, -50.0f, 0.0f, 45.0f, Direction::Right, kSpeedLane3));
	//Lane 1
	vehicles.push_back(Vehicle(CarType::PickUp, -30.0f, 0.0f, 55.0f, Direction::Left, kSpeedLane4));
	vehicles.push_back(Vehicle(CarType::RangeRover, 2.0f, 0.0f, 55.0f, Direction::Left, kSpeedLane4));

	initilizeEntitys(); // Initilzie frogs

	// Intilize tyre rows
	//Row1
	tyres.push_back(TyreChain(20.0f, -2.0f, 75.0f, Direction::Left, 3));
	//Row2
	tyres.push_back(TyreChain(-20.0f, -2.0f, 85.0f, Direction::Right, 3));
	//Row3
	tyres.push_back(TyreChain(10.0f, -2.0f, 95.0f, Direction::Left, 3));
	//Row4
	tyres.push_back(TyreChain(-50.0f, -2.0f, 105.0f, Direction::Right, 3));

	// Initilzie current game state
	currentGameState = GameState::Playing;

	// Sefine fon we wish to use
	IFont* font = myEngine->LoadFont(kFont, kFontSize);
	int textHeight = font->MeasureTextHeight("|"); // Gets the text height of the largest ASCI Charicter

	float deltaTime = gameClock; // Gets last secconds time
	while (myEngine->IsRunning()) // Check to see if the game is running and loop
	{
		lastFrame = 0.0f; //  Set last frame process time to 0
		myEngine->DrawScene(); // Draw frame
		fps++; // add 1 to FPS
		//FPS managment
		while (lastFrame<kFrameTime)
		{
			lastFrame += myEngine->Timer();
		}
		if (gameClock > deltaTime + 1.0f)
		{
			std::cout << "FPS:" << fps << std::endl;
			fps = 0;
			deltaTime = gameClock;
		}
		gameClock += lastFrame;// Add last frame time to clock

		// Text Update
		font->Draw("Score: " + to_string(score), 0, textHeight);

		// Exit Check
		if (myEngine->KeyHit(kExit))
		{
			myEngine->Stop();
			continue;
		}

		// Here we will puase / Resuse the game if the game is not over
		if (myEngine->KeyHit(kPause) && currentGameState != GameState::Over)
		{
			currentGameState = currentGameState == GameState::Paused ? GameState::Playing : GameState::Paused;
		}

		if (currentGameState == GameState::Playing && frogs.size()>0)
		{
			// will display current time to screen
			font->Draw("Time: " + to_string((int)gameTimeCountdown), 0, 0);
			// vehicle code
			for (int i = 0; i < vehicles.size(); i++)
			{
				vehicles[i].moveForward(); // Move car forward
				if (currentFrogIndex < frogs.size())
				{
					Frog* currentFrog = frogs[currentFrogIndex];
					// Check to see if the car has collided with the frog
					if (Maths::sphereToBox(*currentFrog,kFrogRad, *vehicles[i].boundingBox))
					{
						currentFrog->getModel()->ScaleY(0.01f);
						currentFrog->setState(FrogState::Dead);
						currentFrogIndex++;
						gameTimeCountdown = kFrogCrossingTime;
						if (currentFrogIndex < frogs.size())frogs[currentFrogIndex]->attachCamera(camera);
					}
				}
			}
			// Loop through all tyres and move them forawrd
			for (int i = 0; i < tyres.size(); i++)
			{
				tyres[i].moveForward();
			}
			// Check to see if we should move the camera up
			if (myEngine->KeyHeld(kCameraPanUp))
			{
				float increment = cameraArc + kCameraPanIncrement < kCameraMaxPanUp ?
					kCameraPanIncrement :
					cameraArc < kCameraPanUp ?
					kCameraMaxPanUp - cameraArc :
					0.0f;
				cameraArc += increment;
			}
			// Check to see if we should move the camera down
			if (myEngine->KeyHeld(kCameraPanDown))
			{
				float increment = cameraArc - kCameraPanIncrement > kCameraMinPanDown ?
					kCameraPanIncrement :
					cameraArc > kCameraMinPanDown ?
					cameraArc - kCameraMinPanDown :
					0.0f;
				cameraArc -= increment;
			}
			// Check to see if we should reset the camera arc settings
			if (myEngine->KeyHit(kCameraArcReset)) cameraArc = kCameraDefaultArc;

			// Check to see if there is a frog currently being used
			if (currentFrogIndex < frogs.size())
			{
				gameTimeCountdown -= lastFrame; // Counts down game time
				Frog* currentFrog = frogs[currentFrogIndex];
				if (gameTimeCountdown <= 0.0f) // Frog Should Die
				{
					currentFrog->setState(FrogState::Dead);
					currentFrogIndex++;
					gameTimeCountdown = kFrogCrossingTime;
					if (currentFrogIndex < frogs.size())frogs[currentFrogIndex]->attachCamera(camera);
				}
				// Frog movment code
				if (myEngine->KeyHeld(kMoveForward))
				{
					currentFrog->move(Direction::Forward);
				}
				else if (myEngine->KeyHeld(kMoveBack))
				{
					currentFrog->move(Direction::Back);
				}
				else if (myEngine->KeyHeld(kMoveRight))
				{
					currentFrog->move(Direction::Right);
				}
				else if (myEngine->KeyHeld(kMoveLeft))
				{
					currentFrog->move(Direction::Left);
				}
			}
			else
			{
				// Frog reset code
				currentFrogIndex = 0;
				vector<Frog*> tempFrogs;
				bool first = true;
				for (int i = 0; i < frogs.size(); i++)
				{
					Frog* frog = frogs[i];
					// If there is a frog that is still alive then reset it back to the start
					if (frog->getCurrentState() == FrogState::Safe)
					{
						
						frog->getModel()->SetY(-100.0f); // move the old frog undergorung
						Frog* newFrog = new Frog(kFrogFileLocation, frog->startingPosiiton.x, frog->startingPosiiton.y, frog->startingPosiiton.z, first ? FrogState::Crossing : FrogState::Waiting); // Create a new frog
						if(first)newFrog->attachCamera(camera);
						tempFrogs.push_back(newFrog);
						first = false;
					}

					meshPointers[kFrogFileLocation]->RemoveModel(frog->getModel());
				}
				frogs = tempFrogs; // Replace old frog array with new one
				if (first)currentGameState=GameState::Over;
			}
			// Loop through all frogs and exicute there code
			for (int i = 0; i < frogs.size(); i++)
			{
				Frog* currentFrog = frogs[i];
				currentFrog->tick();
			}

		}
		// If game is over display game over message and allow user to restart
		else if (currentGameState = GameState::Over)
		{
			if (myEngine->KeyHit(kGameReset))
			{
				initilizeEntitys();
				currentGameState = GameState::Playing;
			}
			font->Draw("Game Over", 0, 0);
			font->Draw("Press Space To Restart", 0, textHeight*2);
		}
	}


	// Clean up
	myEngine->Delete();
}
