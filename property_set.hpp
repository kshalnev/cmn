#pragma once

#include "type_id.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <utility>

namespace utils
{

template <typename TKey>
class TPropertySet
{
private:
  class IValueHolder
  {
  public:
    virtual ~IValueHolder() = default;
    virtual void const * GetValue() const = 0;
    virtual TypeId GetTypeId() const = 0;
  };

  template <typename TValue>
  class TValueHolder : public IValueHolder
  {
  public:
    TValueHolder(TValue const & value) : m_value(value) {}
    TValueHolder(TValue && value) : m_value(std::move(value)) {}
    TValueHolder(TValueHolder const &) = delete;
    TValueHolder & operator=(TValueHolder const &) = delete;
    void const * GetValue() const override
    {
      return std::addressof(m_value);
    }
    TypeId GetTypeId() const override
    {
      return TypeId::GetTypeId<TValue>();
    }

  private:
    TValue const m_value;
  };

public:
  TPropertySet() {}
  TPropertySet(TPropertySet const &) = delete;
  TPropertySet(TPropertySet && other)
    : m_values(std::move(other.m_values))
  {}

  TPropertySet & operator=(TPropertySet const &) = delete;
  TPropertySet & operator=(TPropertySet && other)
  {
    m_values = std::move(other.m_values);
    return *this;
  }

  template <typename TValue>
  void Set(TKey const & key, TValue && value)
  {
    std::unique_ptr<IValueHolder> holder(
      new TValueHolder<TValue>(std::forward<TValue>(value)));
    m_values[key] = std::move(holder);
  }

  template <typename TValue>
  TValue const * Get(TKey const & key) const
  {
    auto const itr = m_values.find(key);
    if (itr == m_values.end())
      return nullptr;
    if (itr->second->GetTypeId() != TypeId::GetTypeId<TValue>())
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
    return (itr->second->GetTypeId() == TypeId::GetTypeId<TValue>());
  }

private:
  std::map<TKey, std::unique_ptr<IValueHolder>> m_values;
};

using PropertySet = TPropertySet<std::string>;

} // namespace utils
