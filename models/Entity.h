#ifndef ENTITY_H
#define ENTITY_H

#include <string>
using namespace std;

// lớp cha trừu tượng 

class Entity {
  public:
    virtual ~Entity();

    virtual string getId() const = 0;

    virtual void display() const = 0;
};

// định nghĩa hàm destructor ở dưới
inline Entity::~Entity() {}

#endif