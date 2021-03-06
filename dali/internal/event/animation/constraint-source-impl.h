#ifndef __DALI_INTERNAL_CONSTRAINT_SOURCE_H__
#define __DALI_INTERNAL_CONSTRAINT_SOURCE_H__

/*
 * Copyright (c) 2014 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

// INTERNAL INCLUDES
#include <dali/public-api/common/vector-wrapper.h>
#include <dali/public-api/animation/constraint-source.h>
#include <dali/internal/event/common/object-impl.h>

namespace Dali
{

namespace Internal
{

struct Source;
typedef std::vector<Source> SourceContainer;
typedef SourceContainer::iterator SourceIter;

/**
 * The source of an input property for a constraint.
 */
struct Source
{
  /**
   * Default constructor
   */
  Source()
  : sourceType( OBJECT_PROPERTY ),
    propertyIndex( Property::INVALID_INDEX ),
    object( NULL )
  {
  }

  /**
   * Create a constraint source from a public handle.
   * The internal object is not referenced by the Internal::Source; therefore
   * the owner of this object is responsible for observing the Object's lifetime.
   */
  Source( Dali::ConstraintSource& source )
  : sourceType( source.sourceType ),
    propertyIndex( source.propertyIndex ),
    object( NULL )
  {
    if ( source.object )
    {
      object = &dynamic_cast< Object& > ( GetImplementation(source.object) );
    }
  }

  SourceType sourceType; ///< The source type

  Property::Index propertyIndex; ///< The index of the source property

  Object* object; ///< The target object; only valid if sourceType == OBJECT_PROPERTY
};

} // namespace Internal

} // namespace Dali

#endif // __DALI_INTERNAL_CONSTRAINT_SOURCE_H__
