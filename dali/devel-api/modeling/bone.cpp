/*
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
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

// CLASS HEADER
#include <dali/devel-api/modeling/bone.h>

namespace Dali
{

Bone::Bone ()
{
}

Bone::Bone( const std::string& name, const Matrix& offsetMatrix )
: mName( name ),
  mOffsetMatrix( offsetMatrix )
{
}

Bone::~Bone()
{
}

Bone::Bone( const Bone& rhs )
: mName(rhs.mName),
  mOffsetMatrix(rhs.mOffsetMatrix)
{
}

Bone& Bone::operator=(const Bone& rhs)
{
  if (this != &rhs)
  {
    mName = rhs.mName;
    mOffsetMatrix  = rhs.mOffsetMatrix;
  }
  return *this;
}

const std::string& Bone::GetName() const
{
  return mName;
}

const Matrix& Bone::GetOffsetMatrix() const
{
  return mOffsetMatrix;
}

} // namespace Dali