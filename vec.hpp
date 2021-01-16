#pragma once
#include "raw.hpp"
#include "iteratorFacade.hpp"
#include <ph/common/policies.hpp>

namespace ph {



//template <class, template<class...> class... Policies>
//class vec;






////////////////////////////////////////////////////////////////
/// @brief Handles the construction and destruction of
///        elements in the raw vec class "Raw"
///
///        vec<A> allo;
///        allo += 3;                     // push back 3
///        allo += 4;                     // push back 3
///        cout << allo[1] << endl;       // get second element
///        for(auto const& i : allo)      // print vec
///               cout << i << endl;
////////////////////////////////////////////////////////////////
template <class T, class CheckSize = Policy::Check<true>>
class vec
{
public:
      using self = vec;
      using size_type         = size_t;
      using value_type        = T;
      using reference         = value_type&;
      using reference_const   = value_type const&;
      using pointer           = value_type*;
      using const_pointer     = value_type const*;
      using pointer_const     = value_type * const;
      
      class iterator : public IteratorFacade<iterator, T>{
            pointer current = nullptr;
      public:
            iterator (pointer current) : current (current) {
                  
            }
            virtual reference dereference () const  {
                  return *current;
            }
            virtual void increment () {
                  ++current;
            }
            virtual void decrement () {
                  --current;
            }
            virtual bool equals (iterator const& other) const  {
                  return current == other.current;
            }
            virtual bool less (iterator const& other) const  {
                  return current < other.current;
            }
            virtual bool greater (iterator const& other) const {
                  return current > other.current;
            }
      };
      iterator begin () {
            return m_raw[0];
      }
      iterator end () {
            return m_raw[m_constructed];
      }

      
      
      //////////////////////////////////////////////
      ///
      //////////////////////////////////////////////
      vec () : m_raw (1), m_constructed (0) {
            
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief Constructor, allocates "size" elements
      ////////////////////////////////////////////////////////////////
      vec (size_type size) : m_raw (size), m_constructed (size)  {
            
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief
      ////////////////////////////////////////////////////////////////
      template <class... U, size_t N = sizeof...(U) + 1>
      vec (reference_const v1, U const&... u) : m_raw (N), m_constructed (N){
            new (m_raw[0]) T(v1);
            int i = 1;
            ((new (m_raw[i++]) T(u)), ...);
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief
      ////////////////////////////////////////////////////////////////
      template <size_t N>
      vec (T const(&a)[N]) : m_raw (N), m_constructed (N) {
            memmove(m_raw[0], a, N * sizeof(T));
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief
      ////////////////////////////////////////////////////////////////
      vec (self const& other) : m_raw (other.m_raw), m_constructed (other.m_constructed) {
//            cout<<"h"<<*this<<endl;
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief
      ////////////////////////////////////////////////////////////////
      vec (self&& other) : vec () {
            swap (*this, other);
      }
      
      //////////////////////////////////////////////
      ///
      //////////////////////////////////////////////
//      template <class U>
//      vec (vec<U> const& other) : vec () {
//            swap (*this, other);
//      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief
      ////////////////////////////////////////////////////////////////
      vec (value_type const* p, size_t s) : m_raw (s), m_constructed (s){
            memcpy(m_raw[0], p, s * sizeof(value_type));
      }
      
      //////////////////////////////////////////////
      ///
      //////////////////////////////////////////////
      vec (value_type * p, size_t s) : m_raw (p, s), m_constructed (s){
            
      }
      
      //////////////////////////////////////////////
      ///
      //////////////////////////////////////////////
      vec (std::pair<value_type*, size_t> p) : m_raw (p.first, p.second), m_constructed (p.second) {
            
      }
      
      //////////////////////////////////////////////
      ///
      //////////////////////////////////////////////
      template <class U>
      vec (vec<U> const& other) : m_raw (other.size()), m_constructed (other.size()) {
            for (int i = 0; i < m_constructed; ++i) {
                  new (m_raw[i]) T (other[i]);
            }
      }
      
      //////////////////////////////////////////////
      ///
      //////////////////////////////////////////////
//      template <class U>
//      vec (vec<U> && other) : m_raw (other.size()), m_constructed (other.size()) {
//            for (int i = 0; i < m_constructed; ++i) {
//                  new (m_raw[i]) T (other[i]);
//            }
//      }
      
      
      
      ////////////////////////////////////////////////////////////////
      /// @brief Destructor, calling each element's destructor
      ///        before "Raw" free's the actual storage
      ////////////////////////////////////////////////////////////////
      virtual ~vec () {
            if constexpr (!std::is_class_v<T>){
                  for (int i = 0; i < m_constructed; ++i) {
                        m_raw[i]->~value_type();
                  }
            }
            
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief
      ////////////////////////////////////////////////////////////////
      pointer data () {
            return m_raw[0];
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief
      ////////////////////////////////////////////////////////////////
      pointer const data () const {
            return m_raw[0];
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief Append new element, construct it with argument "value"
      ///
      ///        If "Raw" has no more uninitialized elements, expand
      ///        it before constructing
      ////////////////////////////////////////////////////////////////
      template <class U, typename = std::enable_if_t<std::is_constructible_v<T, U>>>
      self& operator += (U const& value) {
            if (m_raw.size() == m_constructed)
            {
                  m_raw *= 2;
            }
//            cout << m_raw.size() << " ";
            new (m_raw[m_constructed++]) T(value);
            
            return *this;
      }
      
//      template <class... Args>
//      self& operator += (Args const&... args) {//std::cout << "fitta " << other << std::endl;
//            if (m_raw.size() == m_constructed)
//            {
//                  m_raw *= 2;
//            }
//            //            cout << m_raw.size() << " ";
//            new (m_raw[m_constructed++]) T(args...);
//
//            return *this;
//      }
      
      self& operator += (self const& other) {//std::cout << "fitta " << other << std::endl;
            
            m_raw += other.m_constructed;
            memcpy(m_raw[0] + m_constructed, other.m_raw[0], other.m_constructed * sizeof(value_type));
            m_constructed += other.m_constructed;
            return *this;
      }
      
      
      
      ////////////////////////////////////////////////////////////////
      /// @brief destruct last element and leave it unitialized
      ////////////////////////////////////////////////////////////////
      self& operator -- () {

            CheckSize::check([s = m_constructed](){assert(s >= 1);});

            m_raw[m_constructed-1]->~T();
            --m_constructed;
            return *this;
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief get number of elements constructed
      ////////////////////////////////////////////////////////////////
      size_type size () const {
            return m_constructed;
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief get element by index
      ////////////////////////////////////////////////////////////////
      reference operator [] (int i) {
            CheckSize::check([m_constructed = m_constructed, i = i](){assert(i < m_constructed and i >= 0);});
            
            return *m_raw[i];
      }
      
      ////////////////////////////////////////////////////////////////
      /// @brief get element by index
      ////////////////////////////////////////////////////////////////
      reference const operator [] (int i) const {
            CheckSize::check([m_constructed = m_constructed, i = i](){assert(i < m_constructed and i >= 0);});
   
            return *m_raw[i];
      }
      
      //////////////////////////////////////////////
      ///
      //////////////////////////////////////////////
      reference get (int i) {
            return this->operator[](i);
      }
      
      //////////////////////////////////////////////
      ///
      //////////////////////////////////////////////
      reference const get (int i) const {
            return this->operator[](i);
      }
      
      template <class U>
      operator U () {
            CheckSize::check([m_constructed = m_constructed](){assert(m_constructed > 0);});
            
            return *m_raw[0];
      }
      
      
      self& operator= (self other) {
            swap(*this, other);
            return *this;
      }
      
      friend void swap (self& lhs, self& rhs) {
            using std::swap;
            swap (lhs.m_raw, rhs.m_raw);
            swap (lhs.m_constructed, rhs.m_constructed);
      }
      
//      friend std::ostream& operator<< (std::ostream& os, ph::vec<T> const& a) {
//            os << "{ ";
//            for (int i = 0; i < a.size(); ++i)
//                  os << a[i] << " ";
//            os << "}";
//            return os;
//      }
      
      
      explicit operator T* () {
            return m_raw[0];
      }
      
      
      template <class U>
      operator U** () {
            return vec<U> (*this);
      }
      
      template <class U>
      explicit operator U* &&() {
            U* u = m_raw[0];
            size_t s = m_constructed;
            m_raw[0] = nullptr;
            m_constructed = 0;
            return vec<U> (u, s);
      }
      
      
      
private:
      Raw<T, CheckSize>      m_raw;
      size_type   m_constructed;
};



template <class T>
inline std::ostream& operator<< (std::ostream& os, ph::vec<T> const& a) {
      os << "{ " << endl;
      for (int i = 0; i < a.size(); ++i)
      os << "\t" << a[i] << endl;
      os << "}";
      return os;
}

//inline std::ostream& operator<< (std::ostream& os, ph::vec<char const*> const& a) {
//      os << "{ ";
//      for (int i = 0; i < a.size(); ++i)
//      os << a[i] << " ";
//      os << "}";
//      return os;
//}






template <class... T>
vec (T...) -> vec<std::common_type_t<T...>>;

template <size_t N, class T>
vec (T const (&)[N]) -> vec<T>;




}



