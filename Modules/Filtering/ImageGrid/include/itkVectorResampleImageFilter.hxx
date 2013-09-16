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
#ifndef __itkVectorResampleImageFilter_hxx
#define __itkVectorResampleImageFilter_hxx

#include "itkVectorResampleImageFilter.h"
#include "itkIdentityTransform.h"
#include "itkVectorLinearInterpolateImageFunction.h"
#include "itkProgressReporter.h"
#include "itkImageRegionIteratorWithIndex.h"

namespace itk
{
/**
 * Initialize new instance
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
VectorResampleImageFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::VectorResampleImageFilter()
{
  m_OutputSpacing.Fill(1.0);
  m_OutputOrigin.Fill(0.0);
  m_OutputDirection.SetIdentity();
  m_Size.Fill(0);
  m_OutputStartIndex.Fill(0);

  m_Transform = IdentityTransform< TInterpolatorPrecisionType, ImageDimension >::New();
  m_Interpolator = VectorLinearInterpolateImageFunction< InputImageType, TInterpolatorPrecisionType >::New();

  m_DefaultPixelValue = NumericTraits< PixelType >::ZeroValue( m_DefaultPixelValue );
}

/**
 * Print out a description of self
 *
 * \todo Add details about this class
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void
VectorResampleImageFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::PrintSelf(std::ostream & os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "DefaultPixelValue: "
     << static_cast< typename NumericTraits< PixelType >::PrintType >( m_DefaultPixelValue )
     << std::endl;
  os << indent << "Size: " << m_Size << std::endl;
  os << indent << "OutputStartIndex: " << m_OutputStartIndex << std::endl;
  os << indent << "OutputSpacing: " << m_OutputSpacing << std::endl;
  os << indent << "OutputOrigin: " << m_OutputOrigin << std::endl;
  os << indent << "OutputDirection: " << m_OutputDirection << std::endl;
  os << indent << "Transform: " << m_Transform.GetPointer() << std::endl;
  os << indent << "Interpolator: " << m_Interpolator.GetPointer() << std::endl;
}

/**
 * Set the output image spacing.
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void
VectorResampleImageFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::SetOutputSpacing(const double *spacing)
{
  SpacingType s(spacing);

  this->SetOutputSpacing(s);
}

/**
 * Set the output image origin.
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void
VectorResampleImageFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::SetOutputOrigin(const double *origin)
{
  OriginPointType p(origin);

  this->SetOutputOrigin(p);
}

/**
 * Set up state of filter before multi-threading.
 * InterpolatorType::SetInputImage is not thread-safe and hence
 * has to be set up before ThreadedGenerateData
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void
VectorResampleImageFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::BeforeThreadedGenerateData()
{
  if ( !m_Interpolator )
    {
    itkExceptionMacro(<< "Interpolator not set");
    }

  // Connect input image to interpolator
  m_Interpolator->SetInputImage( this->GetInput() );

  if (m_DefaultPixelValue.Size() == 0)
    {
    NumericTraits<PixelType>::SetLength( m_DefaultPixelValue,
      this->GetInput()->GetNumberOfComponentsPerPixel() );
    m_DefaultPixelValue.Fill(0);
    }
}

/**
 * Set up state of filter after multi-threading.
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void
VectorResampleImageFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::AfterThreadedGenerateData()
{
  // Disconnect input image from the interpolator
  m_Interpolator->SetInputImage(NULL);
}

/**
 * ThreadedGenerateData
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void
VectorResampleImageFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::ThreadedGenerateData(
  const OutputImageRegionType & outputRegionForThread,
  ThreadIdType threadId)
{
  itkDebugMacro(<< "Actually executing");

  // Get the output pointers
  OutputImagePointer outputPtr = this->GetOutput();

  // Get this input pointers
  InputImageConstPointer inputPtr = this->GetInput();

  // Create an iterator that will walk the output region for this thread.
  typedef ImageRegionIteratorWithIndex< TOutputImage > OutputIterator;

  OutputIterator outIt(outputPtr, outputRegionForThread);

  // Define a few indices that will be used to translate from an input pixel
  // to an output pixel
  PointType outputPoint;         // Coordinates of current output pixel
  PointType inputPoint;          // Coordinates of current input pixel

  typedef ContinuousIndex< TInterpolatorPrecisionType, ImageDimension > ContinuousIndexType;
  ContinuousIndexType inputIndex;

  // Doc says this only works for VectorImage, but Image implementation says otherwise...
  const unsigned int numberOfComponents = this->GetInput()->GetNumberOfComponentsPerPixel();

  // Support for progress methods/callbacks
  ProgressReporter progress( this, threadId, outputRegionForThread.GetNumberOfPixels() );

  typedef typename InterpolatorType::OutputType OutputType;

  // Walk the output region
  outIt.GoToBegin();

  while ( !outIt.IsAtEnd() )
    {
    // Determine the index of the current output pixel
    outputPtr->TransformIndexToPhysicalPoint(outIt.GetIndex(), outputPoint);

    // Compute corresponding input pixel position
    inputPoint = m_Transform->TransformPoint(outputPoint);
    inputPtr->TransformPhysicalPointToContinuousIndex(inputPoint, inputIndex);

    // Evaluate input at right position and copy to the output
    if ( m_Interpolator->IsInsideBuffer(inputIndex) )
      {
      PixelType        pixval;
      NumericTraits< PixelType >::SetLength( pixval, numberOfComponents );

      const OutputType value =
        m_Interpolator->EvaluateAtContinuousIndex(inputIndex);
      for ( unsigned int i = 0; i < numberOfComponents; i++ )
        {
        pixval[i] = static_cast< PixelComponentType >( value[i] );
        }

      outIt.Set(pixval);
      }
    else
      {
      outIt.Set(m_DefaultPixelValue); // default background value
      }

    progress.CompletedPixel();
    ++outIt;
    }
}

/**
 * Inform pipeline of necessary input image region
 *
 * Determining the actual input region is non-trivial, especially
 * when we cannot assume anything about the transform being used.
 * So we do the easy thing and request the entire input image.
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void
VectorResampleImageFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::GenerateInputRequestedRegion()
{
  // call the superclass's implementation of this method
  Superclass::GenerateInputRequestedRegion();

  if ( !this->GetInput() )
    {
    return;
    }

  // get pointers to the input and output
  InputImagePointer inputPtr  =
    const_cast< TInputImage * >( this->GetInput() );

  // Request the entire input image
  InputImageRegionType inputRegion;
  inputRegion = inputPtr->GetLargestPossibleRegion();
  inputPtr->SetRequestedRegion(inputRegion);
}

/**
 * Inform pipeline of required output region
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
void
VectorResampleImageFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::GenerateOutputInformation()
{
  // call the superclass' implementation of this method
  Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  OutputImagePointer outputPtr = this->GetOutput();
  if ( !outputPtr )
    {
    return;
    }

  // Set the size of the output region
  typename TOutputImage::RegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize(m_Size);
  outputLargestPossibleRegion.SetIndex(m_OutputStartIndex);
  outputPtr->SetLargestPossibleRegion(outputLargestPossibleRegion);

  // Set spacing and origin
  outputPtr->SetSpacing(m_OutputSpacing);
  outputPtr->SetOrigin(m_OutputOrigin);
  outputPtr->SetDirection(m_OutputDirection);
}

/**
 * Verify if any of the components has been modified.
 */
template< typename TInputImage, typename TOutputImage, typename TInterpolatorPrecisionType >
ModifiedTimeType
VectorResampleImageFilter< TInputImage, TOutputImage, TInterpolatorPrecisionType >
::GetMTime(void) const
{
  ModifiedTimeType latestTime = Object::GetMTime();

  if ( m_Transform )
    {
    if ( latestTime < m_Transform->GetMTime() )
      {
      latestTime = m_Transform->GetMTime();
      }
    }

  if ( m_Interpolator )
    {
    if ( latestTime < m_Interpolator->GetMTime() )
      {
      latestTime = m_Interpolator->GetMTime();
      }
    }

  return latestTime;
}
} // end namespace itk

#endif
