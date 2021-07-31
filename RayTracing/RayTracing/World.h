#pragma once
#include <memory>
#include <vector>
#include "Hittable.h"

class World :
    public Hittable
{
public:
    World() {}
    World(std::shared_ptr<Hittable> object);

    void clear();
    void add(std::shared_ptr<Hittable> object);

    virtual bool hit(const Ray& r, double t_min, double t_max, Hit& hit) const;

public:
    std::vector<std::shared_ptr<Hittable>> _objects;
};

