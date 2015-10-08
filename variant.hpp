#pragma once

#include <cstdint>
#include <string>
#include <utility>

namespace utils
{

class Variant
{
public:
  class Visitor
  {
  public:
    virtual ~Visitor() = default;
    virtual void Set(int8_t value) = 0;
    virtual void Set(uint8_t value) = 0;
    virtual void Set(int16_t value) = 0;
    virtual void Set(uint16_t value) = 0;
    virtual void Set(int32_t value) = 0;
    virtual void Set(uint32_t value) = 0;
    virtual void Set(int64_t const & value) = 0;
    virtual void Set(uint64_t const & value) = 0;
    virtual void Set(float value) = 0;
    virtual void Set(double const & value) = 0;
    virtual void Set(std::string const & value) = 0;
    virtual void Set(std::wstring const & value) = 0;
  };

  enum Type
  {
    vt_empty = 0,
    vt_int8,
    vt_uint8,
    vt_int16,
    vt_uint16,
    vt_int32,
    vt_uint32,
    vt_int64,
    vt_uint64,
    vt_float,
    vt_double,
    vt_string,
    vt_wstring
  };

  Variant();
  Variant(Variant const & other);
  Variant(Variant && other);

  template <typename TValue>
  Variant(TValue && value) : m_vt(vt_empty)
  {
    Set(std::forward<TValue>(value));
  }

  Variant & operator=(Variant const & value);
  Variant & operator=(Variant && value);

  template <typename TValue>
  Variant & operator=(TValue && value)
  {
    Set(std::forward<TValue>(value));
    return *this;
  }

  ~Variant();

  Type GetType() const { return m_vt; }

  void Set(int8_t value);
  void Set(uint8_t value);
  void Set(int16_t value);
  void Set(uint16_t value);
  void Set(int32_t value);
  void Set(uint32_t value);
  void Set(int64_t const & value);
  void Set(uint64_t const & value);
  void Set(float value);
  void Set(double const & value);
  void Set(std::string const & value);
  void Set(std::string && value);
  void Set(std::wstring const & value);
  void Set(std::wstring && value);

  void Clear();

  bool Get(Visitor & visitor) const;

private:
  union ValueHolder
  {
    int8_t m_int8;
    uint8_t m_uint8;
    int16_t m_int16;
    uint16_t m_uint16;
    int32_t m_int32;
    uint32_t m_uint32;
    int64_t m_int64;
    uint64_t m_uint64;
    float m_float;
    double m_double;
    char m_hstr[sizeof(std::string)];
    char m_hwstr[sizeof(std::wstring)];
  };

  ValueHolder m_value;
  Type m_vt;
};

template <typename T>
class VariantValue
{
private:
  struct VariantValueGetter : public Variant::Visitor
  {
    VariantValueGetter(T & value, bool & valid)
        : m_value(value), m_valid(valid)
    {}

    void Set(int8_t value) override { SetValue(value); }
    void Set(uint8_t value) override { SetValue(value); }
    void Set(int16_t value) override { SetValue(value); }
    void Set(uint16_t value) override { SetValue(value); }
    void Set(int32_t value) override { SetValue(value); }
    void Set(uint32_t value) override { SetValue(value); }
    void Set(int64_t const & value) override { SetValue(value); }
    void Set(uint64_t const & value) override { SetValue(value); }
    void Set(float value) override { SetValue(value); }
    void Set(double const & value) override { SetValue(value); }
    void Set(std::string const & value) override { SetValue(value); }
    void Set(std::wstring const & value) override { SetValue(value); }

    void SetValue(T const & value)
    {
      m_value = value;
      m_valid = true;
    }

    template <typename U> void SetValue(U const &) {}

    T & m_value;
    bool & m_valid;
  };

public:
  VariantValue() : m_valid(false) {}
  VariantValue(VariantValue const &) = default;
  VariantValue(Variant const & variant) : m_valid(false)
  {
    VariantValueGetter getter(m_value, m_valid);
    variant.Get(getter);
  }
  VariantValue(VariantValue && value)
      : m_value(std::move(value.m_value)), m_valid(value.m_valid)
  {
    value.m_valid = false;
  }

  VariantValue & operator=(VariantValue const &) = default;
  VariantValue & operator=(VariantValue && value)
  {
    m_value = std::move(value.m_value);
    m_valid = value.m_valid;
    value.m_valid = true;
    return *this;
  }
  VariantValue & operator=(Variant const & variant)
  {
    bool valid = false;
    T value = T();
    VariantValueGetter getter(value, valid);
    variant.Get(getter);
    m_value = std::move(value);
    m_valid = valid;
    return *this;
  }

  bool IsValid() const { return m_valid; }
  T const & Get() const { return m_value; }

private:
  T m_value;
  bool m_valid;
};

} // namespace utils
