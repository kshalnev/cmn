#include "property_set.hpp"
#include "variant.hpp"

#include <cassert>
#include <iostream>

void test_Variant()
{
  utils::Variant v1("text string");
  assert(v1.GetType() == utils::Variant::vt_string);

  utils::VariantValue<std::string> v1value = v1;
  assert(v1value.IsValid());
  assert(v1value.Get() == "text string");

  utils::Variant v2(L"text wstring");
  assert(v2.GetType() == utils::Variant::vt_wstring);

  v1value = v2;
  assert(!v1value.IsValid());

  std::swap(v1, v2);
  assert(v1.GetType() == utils::Variant::vt_wstring);
  assert(v2.GetType() == utils::Variant::vt_string);

  utils::Variant v3;
  assert(v3.GetType() == utils::Variant::vt_empty);
  v3 = (int8_t)12;
  assert(v3.GetType() == utils::Variant::vt_int8);
  v3 = (uint8_t)12;
  assert(v3.GetType() == utils::Variant::vt_uint8);
  v3 = "text";
  assert(v3.GetType() == utils::Variant::vt_string);
  v3 = (int32_t)65537;
  assert(v3.GetType() == utils::Variant::vt_int32);
  v3.Clear();
  assert(v3.GetType() == utils::Variant::vt_empty);
}

void test_PropertySet()
{
  utils::PropertySet properties;

  int i = 1024;

  properties.Set<double>("distance", 123.456);
  properties.Set<int>("flat", 789);
  properties.Set<std::string>("city_name", "Stockholm");
  properties.Set<std::vector<std::string>>("string_array", {"hello", "world"});
  properties.Set<int *>("int_ptr", &i);
  properties.Set<std::nullptr_t>("null", nullptr);

  auto a = properties.Get<double>("distance");
  assert(a != nullptr && 123.456 == *a);

  auto b = properties.Get<int>("flat");
  assert(b != nullptr && 789 == *b);

  auto c = properties.Get<std::string>("city_name");
  assert(c != nullptr && *c == "Stockholm");

  auto d = properties.Get<std::vector<std::string>>("string_array");
  assert(d != nullptr && *d == std::vector<std::string>({"hello", "world"}));

  auto e = properties.Get<bool>("string_array");
  assert(nullptr == e);

  auto f = properties.Get<int *>("int_ptr");
  assert(f != nullptr && *f == &i);

  auto g = properties.Get<std::nullptr_t>("null");
  assert(g != nullptr && nullptr == *g);

  utils::PropertySet tmp;
  std::swap(tmp, properties);
}

int main(int /* argc */, const char * /* argv */ [])
{
  test_Variant();
  test_PropertySet();
  return 0;
}
