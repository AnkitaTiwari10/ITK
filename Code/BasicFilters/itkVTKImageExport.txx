/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkVTKImageExport.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkVTKImageExport_txx
#define __itkVTKImageExport_txx
#include "itkVTKImageExport.h"

namespace itk
{

/**
 * The constructor records the name of the pixel's scalar type for the
 * image to be sent to vtkImageImport's ScalarTypeCallback.
 */
template <class TInputImage, typename TVTKRealType>
VTKImageExport<TInputImage, TVTKRealType>::VTKImageExport()
{
  typedef typename InputImageType::PixelType ScalarType;

  if(typeid(ScalarType) == typeid(double))
    {
    m_ScalarTypeName = "double";
    }
  else if(typeid(ScalarType) == typeid(float))
    {
    m_ScalarTypeName = "float";
    }
  else if(typeid(ScalarType) == typeid(long))
    {
    m_ScalarTypeName = "long";
    }
  else if(typeid(ScalarType) == typeid(unsigned long))
    {
    m_ScalarTypeName = "unsigned long";
    }
  else if(typeid(ScalarType) == typeid(int))
    {
    m_ScalarTypeName = "int";
    }
  else if(typeid(ScalarType) == typeid(unsigned int))
    {
    m_ScalarTypeName = "unsigned int";
    }
  else if(typeid(ScalarType) == typeid(short))
    {
    m_ScalarTypeName = "short";
    }
  else if(typeid(ScalarType) == typeid(unsigned short))
    {
    m_ScalarTypeName = "unsigned short";
    }
  else if(typeid(ScalarType) == typeid(char))
    {
    m_ScalarTypeName = "char";
    }
  else if(typeid(ScalarType) == typeid(unsigned char))
    {
    m_ScalarTypeName = "unsigned char";
    }
  else
    {
    itkExceptionMacro(<<"Type currently not supported");
    }
}

template <class TInputImage, typename TVTKRealType>
void VTKImageExport<TInputImage, TVTKRealType>::PrintSelf(std::ostream& os,
                                            Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


/**
 * Set the input image for this filter.
 */
template <class TInputImage, typename TVTKRealType>
void VTKImageExport<TInputImage, TVTKRealType>::SetInput(const InputImageType* input)
{
  this->ProcessObject::SetNthInput(0, 
                                   const_cast<TInputImage*>(input) );
}


/**
 * Get the current input image.
 */
template <class TInputImage, typename TVTKRealType>
typename VTKImageExport<TInputImage, TVTKRealType>::InputImageType *
VTKImageExport<TInputImage, TVTKRealType>::GetInput(void)
{
  return static_cast<TInputImage*>(
    this->ProcessObject::GetInput(0));
}


/**
 * Implements the WholeExtentCallback.  This returns a pointer to an
 * array of six integers describing the VTK-style extent of the image.
 * This corresponds to the ITK image's LargestPossibleRegion.
 */
template <class TInputImage, typename TVTKRealType>
int* VTKImageExport<TInputImage, TVTKRealType>::WholeExtentCallback()
{
  InputImagePointer input = this->GetInput();
  InputRegionType region = input->GetLargestPossibleRegion();
  InputSizeType size = region.GetSize();
  InputIndexType index = region.GetIndex();

  unsigned int i=0;
  // Fill in the known portion of the extent.
  for(;i < InputImageDimension;++i)
    {
    m_WholeExtent[i*2] = int(index[i]);
    m_WholeExtent[i*2+1] = int(index[i]+size[i])-1;
    }
  // Fill in the extent for dimensions up to three.
  for(;i < 3; ++i)
    {
    m_WholeExtent[i*2] = 0;
    m_WholeExtent[i*2+1] = 0;
    }
  return m_WholeExtent;
}


/**
 * Implements the SpacingCallback.  This returns a pointer to an array
 * of three floating point values describing the spacing of the image.
 */
template <class TInputImage, typename TVTKRealType>
typename VTKImageExport<TInputImage, TVTKRealType>::VTKSpacingType * 
VTKImageExport<TInputImage, TVTKRealType>::SpacingCallback()
{
  InputImagePointer input = this->GetInput();
  const typename TInputImage::SpacingType& spacing = input->GetSpacing();
  
  unsigned int i=0;
  // Fill in the known portion of the spacing.
  for(;i < InputImageDimension;++i)
    {
    m_DataSpacing[i] = VTKSpacingType (spacing[i]);
    }
  // Fill up the spacing with defaults up to three dimensions.
  for(;i < 3;++i)
    {
    m_DataSpacing[i] = 1;
    }
  return m_DataSpacing;
}


/**
 * Implements the OriginCallback.  This returns a pointer to an array
 * of three floating point values describing the origin of the image.
 */
template <class TInputImage, typename TVTKRealType>
typename VTKImageExport<TInputImage, TVTKRealType>::VTKOriginType * 
VTKImageExport<TInputImage, TVTKRealType>::OriginCallback()
{
  InputImagePointer input = this->GetInput();
  const typename TInputImage::PointType& origin = input->GetOrigin();

  unsigned int i=0;
  // Fill in the known portion of the origin.
  for(;i < InputImageDimension;++i)
    {
    m_DataOrigin[i] = VTKOriginType (origin[i]);
    }
  // Fill up the origin with defaults up to three dimensions.
  for(;i < 3;++i)
    {
    m_DataOrigin[i] = 0;
    }
  return m_DataOrigin;
}


/**
 * Implements the ScalarTypeCallback.  This returns the name of the
 * scalar value type of the image.
 */
template <class TInputImage, typename TVTKRealType>
const char* VTKImageExport<TInputImage, TVTKRealType>::ScalarTypeCallback()
{
  return m_ScalarTypeName.c_str();
}


/**
 * Implements the NumberOfComponentsCallback.  This returns the number of
 * components per pixel for the image.
 *
 * Currently, only one pixel component is supported by this class.
 */
template <class TInputImage, typename TVTKRealType>
int VTKImageExport<TInputImage, TVTKRealType>::NumberOfComponentsCallback()
{
  return 1;
}


/**
 * Implements the PropagateUpdateExtentCallback.  This takes the
 * update extent from VTK and translates it into a RequestedRegion for
 * the input image.  This requested region is then propagated through
 * the ITK pipeline.
 */
template <class TInputImage, typename TVTKRealType>
void VTKImageExport<TInputImage, TVTKRealType>::PropagateUpdateExtentCallback(int* extent)
{  
  InputSizeType size;
  InputIndexType index;

  for(unsigned int i=0;i < InputImageDimension;++i)
    {
    index[i] = extent[i*2];
    size[i] = (extent[i*2+1]-extent[i*2])+1;
    }
  
  InputRegionType region;
  region.SetSize(size);
  region.SetIndex(index);
  
  InputImagePointer input = this->GetInput();
  input->SetRequestedRegion(region);
}


/**
 * Implements the DataExtentCallback.  This returns a pointer to an
 * array of six integers describing the VTK-style extent of the
 * buffered portion of the image.  This corresponds to the ITK image's
 * BufferedRegion.
 */
template <class TInputImage, typename TVTKRealType>
int* VTKImageExport<TInputImage, TVTKRealType>::DataExtentCallback()
{
  InputImagePointer input = this->GetInput();
  InputRegionType region = input->GetBufferedRegion();
  InputSizeType size = region.GetSize();
  InputIndexType index = region.GetIndex();

  unsigned int i=0;
  for(;i < InputImageDimension;++i)
    {
    m_DataExtent[i*2] = int(index[i]);
    m_DataExtent[i*2+1] = int(index[i]+size[i])-1;
    }
  for(;i < 3; ++i)
    {
    m_DataExtent[i*2] = 0;
    m_DataExtent[i*2+1] = 0;
    }
  return m_DataExtent;
}


/**
 * Implements the BufferPointerCallback.  This returns a pointer to
 * the image's memory buffer.
 */
template <class TInputImage, typename TVTKRealType>
void* VTKImageExport<TInputImage, TVTKRealType>::BufferPointerCallback()
{
  InputImagePointer input = this->GetInput();
  return input->GetBufferPointer();
}

template <class TInputImage, typename TVTKRealType>
typename VTKImageExport<TInputImage, TVTKRealType>::VTKSpacingType * 
VTKImageExport<TInputImage, TVTKRealType>::SpacingCallbackFunction(void* userData)
{
  return static_cast<VTKImageExport<TInputImage, TVTKRealType>*>(userData)->
    SpacingCallback();
}

template <class TInputImage, typename TVTKRealType>
typename VTKImageExport<TInputImage, TVTKRealType>::VTKOriginType * 
VTKImageExport<TInputImage, TVTKRealType>::OriginCallbackFunction(void* userData)
{
  return static_cast<VTKImageExport<TInputImage,TVTKRealType>*>(userData)->
    OriginCallback();
}

template <class TInputImage, typename TVTKRealType>
typename VTKImageExport<TInputImage, TVTKRealType>::SpacingCallbackType
VTKImageExport<TInputImage, TVTKRealType>::GetSpacingCallback() const
{
  return &VTKImageExport<TInputImage, TVTKRealType>::SpacingCallbackFunction;
}

template <class TInputImage, typename TVTKRealType>
typename VTKImageExport<TInputImage, TVTKRealType>::OriginCallbackType
VTKImageExport<TInputImage, TVTKRealType>::GetOriginCallback() const
{
  return &VTKImageExport<TInputImage, TVTKRealType>::OriginCallbackFunction;
}


} // end namespace itk

#endif
