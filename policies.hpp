#pragma once





struct Policy
{
      struct Common
      {
      protected:
            ~Common(){}
      };
  
      template <bool, class...>
      struct Check;
      
      template <>
      struct Check<true> : Common
      {
            template <class Lambda>
            static void check (Lambda f) {
                  f ();
            }
      };
      
      
};
