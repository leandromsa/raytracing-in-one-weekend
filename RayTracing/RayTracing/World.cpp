#include "World.h"

World::World(std::shared_ptr<Hittable> object)
{
	add(object);
}

void World::clear()
{
	_objects.clear();
}

void World::add(std::shared_ptr<Hittable> object)
{
	_objects.push_back(object);
}

bool World::hit(const Ray& r, double t_min, double t_max, Hit& hit) const
{
    Hit temp_hit;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& object : _objects) {
        if (object->hit(r, t_min, closest_so_far, temp_hit)) {
            hit_anything = true;
            closest_so_far = temp_hit.t;
            hit = temp_hit;
        }
    }

    return hit_anything;
}
