/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkOBJMeshIOFactory_h
#define itkOBJMeshIOFactory_h
#include "ITKIOMeshExport.h"

#include "itkObjectFactoryBase.h"
#include "itkMeshIOBase.h"

namespace itk
{
/** \class OBJMeshIOFactory
   * \brief Create instances of OBJMeshIO objects using an object factory.
   * \ingroup ITKIOMesh
   */
class ITKIOMesh_EXPORT OBJMeshIOFactory:public ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef OBJMeshIOFactory           Self;
  typedef ObjectFactoryBase          Superclass;
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Class methods used to interface with the registered factories. */
  const char * GetITKSourceVersion(void) const override;

  const char * GetDescription(void) const override;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(OBJMeshIOFactory, ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
    {
    OBJMeshIOFactory::Pointer objFactory = OBJMeshIOFactory::New();

    ObjectFactoryBase::RegisterFactoryInternal(objFactory);
    }

protected:
  OBJMeshIOFactory();
  ~OBJMeshIOFactory() override;

  void PrintSelf(std::ostream & os, Indent indent) const override;

private:
  ITK_DISALLOW_COPY_AND_ASSIGN(OBJMeshIOFactory);
};
} // end namespace itk

#endif
