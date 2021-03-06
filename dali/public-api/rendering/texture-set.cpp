/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
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
#include <dali/public-api/rendering/texture-set.h>          // Dali::TextureSet

// INTERNAL INCLUDES
#include <dali/internal/event/rendering/texture-set-impl.h> // Dali::Internal::TextureSet
#include <dali/internal/event/rendering/sampler-impl.h>     // Dali::Internal::Sampler
#include <dali/internal/event/rendering/shader-impl.h>      // Dali::Internal::Shader

namespace Dali
{

TextureSet TextureSet::New()
{
  Internal::TextureSetPtr textureSet = Internal::TextureSet::New();
  return TextureSet( textureSet.Get() );
}

TextureSet::TextureSet()
{
}

TextureSet::~TextureSet()
{
}

TextureSet::TextureSet( const TextureSet& handle )
: BaseHandle( handle )
{
}

TextureSet TextureSet::DownCast( BaseHandle handle )
{
  return TextureSet( dynamic_cast<Dali::Internal::TextureSet*>(handle.GetObjectPtr()));
}

TextureSet& TextureSet::operator=( const TextureSet& handle )
{
  BaseHandle::operator=( handle );
  return *this;
}

void TextureSet::SetTexture( size_t index, Texture texture )
{
  if( texture )
  {
    Internal::TexturePtr texturePtr( &GetImplementation( texture ) );
    GetImplementation(*this).SetTexture( index, texturePtr );
  }
  else
  {
    GetImplementation(*this).SetTexture( index, NULL );
  }
}

Texture TextureSet::GetTexture( size_t index ) const
{
  Internal::Texture* texturePtr = GetImplementation(*this).GetTexture( index );
  return Dali::Texture( texturePtr );
}

void TextureSet::SetSampler( size_t index, Sampler sampler )
{
  if( sampler )
  {
    Internal::SamplerPtr samplerPtr( &GetImplementation( sampler ) );
    GetImplementation(*this).SetSampler( index, samplerPtr );
  }
  else
  {
    GetImplementation(*this).SetSampler( index, NULL );
  }
}

Sampler TextureSet::GetSampler( size_t index ) const
{
  Internal::Sampler* samplerPtr = GetImplementation(*this).GetSampler( index );
  return Dali::Sampler( samplerPtr );
}

size_t TextureSet::GetTextureCount() const
{
  return GetImplementation(*this).GetTextureCount();
}

TextureSet::TextureSet( Internal::TextureSet* pointer )
: BaseHandle( pointer )
{
}

} //namespace Dali
