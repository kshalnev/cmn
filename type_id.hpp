#pragma once

namespace utils
{

class TypeId
{
public:
  TypeId() : m_h(nullptr) {}
  TypeId(TypeId const &) = default;
  TypeId & operator=(TypeId const &) = default;

  bool operator==(TypeId const & other) const
  {
    return m_h == other.m_h;
  }

  bool operator!=(TypeId const & other) const
  {
    return m_h != other.m_h;
  }

  template <typename T>
  static TypeId GetTypeId()
  {
    static int i = 0;
    return TypeId(&i);
  }

private:
  TypeId(void const * h) : m_h(h) {}

  void const * m_h;
};

}  // namespace utils
