/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVOLImageIOFactory.h
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

Copyright (c) 2001 Insight Consortium
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * The name of the Insight Consortium, nor the names of any consortium members,
   nor of any contributors, may be used to endorse or promote products derived
   from this software without specific prior written permission.

  * Modified source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#ifndef __itkVOLImageIOFactory_h
#define __itkVOLImageIOFactory_h

#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace itk
{
/** \class VOLImageIOFactory
 * \brief Create instances of VOLImageIO objects using an object factory.
 *
 */

class ITK_EXPORT VOLImageIOFactory : public ObjectFactoryBase
{
public:

  /**
   * Standard "Self" typedef.
   */
  typedef VOLImageIOFactory   Self;

  /**
   * Standard "Superclass" typedef.
   */
  typedef Object  Superclass;

  /** 
   * Smart pointer typedef support.
   */
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /**
   * Class Methods used to interface with the registered factories
   */
  virtual const char* GetITKSourceVersion();
  virtual const char* GetDescription() const;

  /** 
   * Run-time type information (and related methods).
   */
  itkTypeMacro(VOLImageIOFactory, ObjectFactoryBase);
  
  /**
   * Method for creation through the object factory.
   */
  static VOLImageIOFactory* New()
  {
      return new VOLImageIOFactory;
  }
  /**
   * Register one factory of this type 
   */
  static void RegisterOneFactory(void)
  {
    VOLImageIOFactory::Pointer volFactory = VOLImageIOFactory::New();
    ObjectFactoryBase::RegisterFactory(volFactory);
  }
 

protected:
  VOLImageIOFactory();
  ~VOLImageIOFactory();
  virtual void PrintSelf(std::ostream& os, Indent indent) const;

private:
  Self(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
};

} // end namespace itk

#endif



