class GameObject {
public:

	bool exist = true;
	char label[255];

	char texture_name[64];

	ZSpire::Transform transform;

	void setLabel(const char* label);

	
};

void addObject(GameObject object);

void createNewObject();

GameObject getObject(unsigned int pos);

GameObject* getObjectPtr(unsigned int pos);

unsigned int getObjectsAmount();