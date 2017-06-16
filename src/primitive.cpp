#include "primitive.hpp"

Primitive::Intersection::operator bool() const {
  return t >= 0;
}
