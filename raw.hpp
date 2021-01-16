#pragma once
#include <ph/common/policies.hpp>

//template <class, template<class...> class...>
//class Raw;
//template <class... T>
//Raw (T...) -> Raw<std::common_type_t<T...>>;



////////////////////////////////////////////////////////////////
/// @brief Handles a raw array with uninitialized elements.
///
///        Calls no constructors when allocating space.
///        Calls no destructors when deallocating space.
////////////////////////////////////////////////////////////////
template <class T, class CheckSize = Policy::Check<true>>
class Raw
{
public:
      using self              = Raw;
      using size_type         = size_t;
      using value_type        = T;
      using reference         = value_type&;
      using const_reference   = value_type const&;
      using pointer           = value_type*;
      using const_pointer     = value_type const*;
      using pointer_const     = value_type * const;
      
      
      ////////////////////////////////////////////////////////////////
      /// @brief Constructor, allocates "size" number of elements
      ////////////////////////////////////////////////////////////////
      Raw (size_type size = 1) : m_begin ((pointer) malloc (sizeof(value_type) * size)), m_size (size), m_malloced (true)  {
            
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief
      ////////////////////////////////////////////////////////////////
      Raw (self const& other) : m_begin ((pointer) malloc (sizeof(value_type) * other.m_size)), m_size (other.m_size), m_malloced (true) {
            memmove(m_begin, other.m_begin, sizeof(value_type) * m_size);
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief
      ////////////////////////////////////////////////////////////////
      Raw (self&& other) : Raw () {
            swap (*this, other);
      }
      
      //////////////////////////////////////////////
      ///
      //////////////////////////////////////////////
      Raw (value_type * p, size_t s) : m_begin (p), m_size (s), m_malloced (false) {
           
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief Destructor, Free's all memory
      ////////////////////////////////////////////////////////////////
      ~Raw () {
            if (m_malloced)
            free (m_begin);
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief Expand array with "size" elements
      ////////////////////////////////////////////////////////////////
      self& operator += (size_type size) {
            m_size += size;
            m_begin = (pointer) realloc (m_begin, sizeof(value_type) * m_size);
            return *this;
      }
      self& operator *= (size_type size) {
            m_size *= size;
            m_begin = (pointer) realloc (m_begin, sizeof(value_type) * m_size);
            return *this;
      }
      self& operator /= (size_type size) {
            m_size /= size;
            m_begin = (pointer) realloc (m_begin, sizeof(value_type) * m_size);
            return *this;
      }
      self& operator ++ () {
            m_begin = (pointer) realloc(m_begin, sizeof(value_type) * ++m_size);
            return *this;
      }
//      self& operator+= (self const& other) {std::cout << m_size << std::endl;
//            m_begin = (pointer) realloc(m_begin, sizeof(value_type) * (m_size + other.m_size));
//            memmove(m_begin + m_size, other.m_begin, other.m_size);
//            m_size += other.m_size;
//            return *this;
//      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief Shrink, pop last "size" elements
      ////////////////////////////////////////////////////////////////
      self& operator -= (size_type size) {
            assert(m_size - size >= 0);
            m_size -= size;
            m_begin = (pointer) realloc (m_begin, m_size);
            return *this;
      }
      self& operator -- () {
            assert(m_size - 1 >= 0);
            m_begin = (pointer) realloc(m_begin, --m_size);
            return *this;
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief Get element by index
      ////////////////////////////////////////////////////////////////
      pointer operator [] (int i) {
            assert(i < m_size and i >= 0);
            return m_begin + i;
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief Get element by index
      ////////////////////////////////////////////////////////////////
      pointer const operator [] (int i) const {
            //            assert(i < m_size and i >= 0);
            return m_begin + i;
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief Get array size
      ////////////////////////////////////////////////////////////////
      size_type size () const {
            return m_size;
      }
      
      friend void swap (self& lhs, self& rhs) {
            using std::swap;
            swap (lhs.m_begin, rhs.m_begin);
            swap (lhs.m_size, rhs.m_size);
      }
      
      self& operator= (self other) {
            swap (*this, other);
            return *this;
      }
      
private:
      pointer           m_begin;
      size_type         m_size;
      bool const        m_malloced;
};
