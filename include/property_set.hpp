#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <utility>

namespace utils
{

namespace impl
{

template <typename T>
struct TTypeToId
{
  static void const * GetId()
  {
    static int i;
    return &i;
  }
};

class IValueHolder
{
public:
  virtual ~IValueHolder() = default;
  virtual void const * GetValue() const = 0;
  virtual void const * GetTypeId() const = 0;
};

template <typename TValue>
class TValueHolder : public IValueHolder
{
public:
  TValueHolder(TValue const & value) : m_value(value) {}
  TValueHolder(TValue && value) : m_value(move(value)) {}
  TValueHolder(TValueHolder const &) = delete;
  TValueHolder & operator=(TValueHolder const &) = delete;
  void const * GetValue() const override
  {
    return addressof(m_value);
  }
  void const * GetTypeId() const override
  {
    return TTypeToId<TValue>::GetId();
  }

private:
  TValue const m_value;
};

} // namespace impl

template <typename TKey>
class TPropertySet
{
public:
  TPropertySet() {}
  TPropertySet(TPropertySet const &) = delete;
  TPropertySet(TPropertySet && other)
    : m_values(move(other.m_values))
  {}

  TPropertySet & operator=(TPropertySet const &) = delete;
  TPropertySet & operator=(TPropertySet && other)
  {
    m_values = move(other.m_values);
    return *this;
  }

  template <typename TValue>
  void Set(TKey const & key, TValue && value)
  {
    unique_ptr<impl::IValueHolder> holder(
      new impl::TValueHolder<TValue>(forward<TValue>(value)));
    m_values[key] = move(holder);
  }

  template <typename TValue>
  TValue const * Get(TKey const & key) const
  {
    auto const itr = m_values.find(key);
    if (itr == m_values.end())
      return nullptr;
    if (itr->second->GetTypeId() != impl::TTypeToId<TValue>::GetId())
      return nullptr;
    return static_cast<TValue const *>(itr->second->GetValue());
  }

  bool Has(TKey const & key) const
  {
    return m_values.end() != m_values.find(key);
  }

  template <typename TValue>
  bool Is(TKey const & key) const
  {
    auto const itr = m_values.find(key);
    if (itr == m_values.end())
      return false;
    return (itr->second->GetTypeId() == impl::TTypeToId<TValue>::GetId());
  }

  void swap(TPropertySet<TKey> & other)
  {
    m_values.swap(other.m_values);
  }

private:
  std::map<TKey, std::unique_ptr<impl::IValueHolder>> m_values;
};

template <typename TKey>
inline void swap(TPropertySet<TKey> & a, TPropertySet<TKey> & b)
{
  a.swap(b);
}

using PropertySet = TPropertySet<string>;

} // namespace utils
