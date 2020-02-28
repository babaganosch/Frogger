#pragma once

// GameObject represents objects which moves are drawn
#include <vector>

enum Message { ON_GROUND, HIT, PLAYER_DEATH, GAME_OVER, LEVEL_WIN, NO_MSG, QUIT };

class Component;

class GameObject
{
protected:
	std::vector<GameObject*> receivers;
	std::vector<Component*>  components;

public:
    int bbox_left, bbox_right, bbox_top, bbox_bot;
    int width, height;
	double horizontalPosition;
	double verticalPosition;
	bool   enabled;

	virtual ~GameObject();
    
	virtual void Create();
	virtual void AddComponent(Component * component);

	virtual void Init();
	virtual void Update(float dt);
	virtual void Destroy();
	virtual void AddReceiver(GameObject *go);
	virtual void Receive(Message m) {}
	void Send(Message m);
    
    template<typename T>
    T GetComponent() {
        for (Component * c : components) {
            T t = dynamic_cast<T>(c);  //ugly but it works...
            if (t != nullptr) {
                return t;
            }
        }

        return nullptr;
    }
};
