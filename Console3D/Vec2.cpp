#include <math.h>
#include "Vec2.h"

double length(vec2 const& v) { 
	return sqrt(v.x * v.x + v.y * v.y); 
}