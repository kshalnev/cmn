#include "variant.hpp"

#include <cassert>

namespace utils
{

Variant::Variant()
  : m_vt(vt_empty)
{}

Variant::Variant(Variant const & other)
  : m_vt(vt_empty)
{
  Variant::operator=(other);
}

Variant::Variant(Variant && other)
  : m_vt(vt_empty)
{
  Variant::operator=(std::move(other));
}

Variant & Variant::operator=(Variant const & other)
{
  if (other.m_vt == vt_string)
  {
    std::string const & s = *reinterpret_cast<std::string const *>(other.m_value.m_hstr);
    Set(s);
  }
  else if (other.m_vt == vt_wstring)
  {
    std::wstring const & s = *reinterpret_cast<std::wstring const *>(other.m_value.m_hwstr);
    Set(s);
  }
  else
  {
    Clear();
    ::memcpy(&m_value, &other.m_value, sizeof(ValueHolder));
    m_vt = other.m_vt;
  }
  return *this;
}

Variant & Variant::operator=(Variant && other)
{
  if (other.m_vt == vt_string)
  {
    std::string & s = *reinterpret_cast<std::string *>(other.m_value.m_hstr);
    Set(std::move(s));
  }
  else if (other.m_vt == vt_wstring)
  {
    std::wstring & s = *reinterpret_cast<std::wstring *>(other.m_value.m_hwstr);
    Set(std::move(s));
  }
  else
  {
    Clear();
    ::memcpy(&m_value, &other.m_value, sizeof(ValueHolder));
    m_vt = other.m_vt;
  }
  other.Clear();
  return *this;
}

Variant::~Variant()
{
  Clear();
}

void Variant::Clear()
{
  if (m_vt == vt_string)
  {
    using namespace std;
    string & s = *reinterpret_cast<std::string *>(m_value.m_hstr);
    s.~string();
  }
  else if (m_vt == vt_wstring)
  {
    using namespace std;
    wstring & s = *reinterpret_cast<std::wstring *>(m_value.m_hwstr);
    s.~wstring();
  }
  m_vt = vt_empty;
}

void Variant::Set(int8_t value)
{
  Clear();
  m_value.m_int8 = value;
  m_vt = vt_int8;
}

void Variant::Set(uint8_t value)
{
  Clear();
  m_value.m_uint8 = value;
  m_vt = vt_uint8;
}

void Variant::Set(int16_t value)
{
  Clear();
  m_value.m_int16 = value;
  m_vt = vt_int16;
}

void Variant::Set(uint16_t value)
{
  Clear();
  m_value.m_uint16 = value;
  m_vt = vt_uint16;
}

void Variant::Set(int32_t value)
{
  Clear();
  m_value.m_int32 = value;
  m_vt = vt_int32;
}

void Variant::Set(uint32_t value)
{
  Clear();
  m_value.m_uint32 = value;
  m_vt = vt_uint32;
}

void Variant::Set(int64_t const & value)
{
  Clear();
  m_value.m_int64 = value;
  m_vt = vt_int64;
}

void Variant::Set(uint64_t const & value)
{
  Clear();
  m_value.m_uint64 = value;
  m_vt = vt_uint64;
}

void Variant::Set(float value)
{
  Clear();
  m_value.m_float = value;
  m_vt = vt_float;
}

void Variant::Set(double const & value)
{
  Clear();
  m_value.m_double = value;
  m_vt = vt_double;
}

void Variant::Set(std::string const & value)
{
  if (m_vt == vt_string)
  {
    std::string & s = *reinterpret_cast<std::string *>(m_value.m_hstr);
    s = value;
  }
  else
  {
    Clear();
    new (m_value.m_hstr) std::string(value);
    m_vt = vt_string;
  }
}

void Variant::Set(std::string && value)
{
  if (m_vt == vt_string)
  {
    std::string & s = *reinterpret_cast<std::string *>(m_value.m_hstr);
    s = std::move(value);
  }
  else
  {
    Clear();
    new (m_value.m_hstr) std::string(std::move(value));
    m_vt = vt_string;
  }
}

void Variant::Set(std::wstring const & value)
{
  if (m_vt == vt_wstring)
  {
    std::wstring & s = *reinterpret_cast<std::wstring *>(m_value.m_hwstr);
    s = value;
  }
  else
  {
    Clear();
    new (m_value.m_hwstr) std::wstring(value);
    m_vt = vt_wstring;
  }
}

void Variant::Set(std::wstring && value)
{
  if (m_vt == vt_wstring)
  {
    std::wstring & s = *reinterpret_cast<std::wstring *>(m_value.m_hwstr);
    s = std::move(value);
  }
  else
  {
    Clear();
    new (m_value.m_hwstr) std::wstring(std::move(value));
    m_vt = vt_wstring;
  }
}

bool Variant::Get(Visitor & visitor) const
{
  switch (m_vt)
  {
  case vt_empty: return false;
  case vt_int8: visitor.Set(m_value.m_int8); return true;
  case vt_uint8: visitor.Set(m_value.m_uint8); return true;
  case vt_int16: visitor.Set(m_value.m_int16); return true;
  case vt_uint16: visitor.Set(m_value.m_uint16); return true;
  case vt_int32: visitor.Set(m_value.m_int32); return true;
  case vt_uint32: visitor.Set(m_value.m_uint32); return true;
  case vt_int64: visitor.Set(m_value.m_int64); return true;
  case vt_uint64: visitor.Set(m_value.m_uint64); return true;
  case vt_float: visitor.Set(m_value.m_float); return true;
  case vt_double: visitor.Set(m_value.m_double); return true;
  case vt_string:
    {
      std::string const & s = *reinterpret_cast<std::string const *>(m_value.m_hstr);
      visitor.Set(s);
      return true;
    }
  case vt_wstring:
    {
      std::wstring const & s = *reinterpret_cast<std::wstring const *>(m_value.m_hwstr);
      visitor.Set(s);
      return true;
    }
  }
  assert(false);
  return false;
}

} // namespace utils
