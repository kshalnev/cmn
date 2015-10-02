#include "include/property_set.hpp"

#include <iostream>

int main(int argc, const char * argv[])
{
  int i = 1024;

  utils::PropertySet properties;

  properties.Set<double>("distance", 123.456);
  properties.Set<int>("flat", 789);
  properties.Set<string>("city_name", "Stockholm");
  properties.Set<vector<string>>("string_array", {"hello", "world"});
  properties.Set<int *>("int_ptr", &i);
  properties.Set<nullptr_t>("null", nullptr);

  auto a = properties.Get<double>("distance");
  auto b = properties.Get<int>("flat");
  auto c = properties.Get<string>("city_name");
  auto d = properties.Get<vector<string>>("string_array");
  auto e = properties.Get<bool>("string_array");
  auto f = properties.Get<int *>("int_ptr");
  auto g = properties.Get<nullptr_t>("null");

  return 0;
}
