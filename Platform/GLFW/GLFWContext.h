//-------------------------------------------------------------------------
//  Name: GLFWContext.h
//
//  Desc: This class ensures that glfw is init/destroyed only once no matter the
//  number of instances.
//
//  Author: Aayush Bade 2026 (aayushbade14.github.io/Portfolio)
//-------------------------------------------------------------------------

#pragma once

namespace Crochet
{
  class GLFWContext
  {
    private:
      
      static inline int m_RefCount = 0;

    public:
    
      GLFWContext();

      ~GLFWContext();
  };
}


