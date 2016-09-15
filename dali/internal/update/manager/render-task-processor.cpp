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
#include <dali/internal/update/manager/render-task-processor.h>

// INTERNAL INCLUDES
#include <dali/internal/update/manager/sorted-layers.h>
#include <dali/internal/update/render-tasks/scene-graph-render-task.h>
#include <dali/internal/update/render-tasks/scene-graph-render-task-list.h>
#include <dali/internal/update/nodes/scene-graph-layer.h>
#include <dali/internal/render/common/render-item.h>
#include <dali/internal/render/common/render-tracker.h>
#include <dali/internal/render/common/render-instruction.h>
#include <dali/internal/render/common/render-instruction-container.h>
#include <dali/internal/render/renderers/render-renderer.h>
#include <dali/integration-api/debug.h>

#if defined(DEBUG_ENABLED)
extern Debug::Filter* gRenderTaskLogFilter;
#endif

namespace Dali
{

namespace Internal
{


namespace SceneGraph
{

namespace //Unnamed namespace
{

// Return false if the node or it's parents are exclusive to another render-task.
bool CheckExclusivity( const Node& node, const RenderTask& task )
{
  const RenderTask* exclusiveTo = node.GetExclusiveRenderTask();
  if( exclusiveTo )
  {
    return ( exclusiveTo == &task );
  }

  const Node* parent = node.GetParent();
  if ( parent )
  {
    return CheckExclusivity( *parent, task );
  }

  // No exclusive flags set.
  return true;
}

Layer* FindLayer( Node& node )
{
  Node* currentNode( &node );
  Layer* layer( NULL );
  while( currentNode )
  {
    if( ( layer = currentNode->GetLayer() ) != NULL )
    {
      return layer;
    }

    currentNode = currentNode->GetParent();
  }

  return NULL;
}

/**
 * Rebuild the Layer::colorRenderables and overlayRenderables members,
 * including only renderers which are included in the current render-task.
 * Returns true if all renderers have finished acquiring resources.
 *
 * @param[in] updateBufferIndex The current update buffer index.
 * @param[in] node The current node of the scene-graph.
 * @param[in] currentLayer The current layer containing lists of opaque/transparent renderables.
 * @param[in] renderTask The current render-task.
 * @param[in] inheritedDrawMode The draw mode of the parent
 * @param[in] currentClippingId The current Clipping Id
 *              Note: ClippingId is passed by reference, so it is permanently modified when traversing back up the tree for uniqueness.
 * @param[in] clippingDepth The current clipping depth
 */
bool AddRenderablesForTask( BufferIndex updateBufferIndex,
                            Node& node,
                            Layer& currentLayer,
                            RenderTask& renderTask,
                            int inheritedDrawMode,
                            uint32_t& currentClippingId,
                            uint32_t clippingDepth )
{
  bool resourcesFinished = true;

  // Short-circuit for invisible nodes
  if( !node.IsVisible( updateBufferIndex ) )
  {
    return resourcesFinished;
  }

  // Check whether node is exclusive to a different render-task
  const RenderTask* exclusiveTo = node.GetExclusiveRenderTask();
  if( exclusiveTo && ( exclusiveTo != &renderTask ) )
  {
    return resourcesFinished;
  }

  // Assume all children go to this layer (if this node is a layer).
  Layer* layer = node.GetLayer();
  if( layer )
  {
    // Layers do not inherit the DrawMode from their parents
    inheritedDrawMode = node.GetDrawMode();
  }
  else
  {
    // This node is not a layer.
    layer = &currentLayer;
    inheritedDrawMode |= node.GetDrawMode();
  }

  DALI_ASSERT_DEBUG( NULL != layer );

  // Update the clipping Id and depth for this node (if clipping is enabled).
  if( DALI_UNLIKELY( node.GetClippingMode() != ClippingMode::DISABLED ) )
  {
    ++currentClippingId; // This modifies the reference passed in as well as the local value, causing the value to be global to the recursion.
    ++clippingDepth;     // This only modifies the local value (which is passed in when the method recurses).
  }
  // Set the information in the node.
  node.SetClippingInformation( currentClippingId, clippingDepth );

  const unsigned int count = node.GetRendererCount();
  for( unsigned int i = 0; i < count; ++i )
  {
    SceneGraph::Renderer* renderer = node.GetRendererAt( i );
    bool ready = false;
    bool complete = false;
    renderer->GetReadyAndComplete( ready, complete );

    DALI_LOG_INFO( gRenderTaskLogFilter, Debug::General, "Testing renderable:%p ready:%s complete:%s\n", renderer, ready ? "T" : "F", complete ? "T" : "F" );

    resourcesFinished &= complete;

    if( ready ) // IE. should be rendered (all resources are available)
    {
      // Normal is the more-likely draw mode to occur.
      if( DALI_LIKELY( inheritedDrawMode == DrawMode::NORMAL ) )
      {
        layer->colorRenderables.PushBack( Renderable( &node, renderer ) );
      }
      else
      {
        layer->overlayRenderables.PushBack( Renderable( &node, renderer ) );
      }
    }
  }

  // Recurse children.
  NodeContainer& children = node.GetChildren();
  const NodeIter endIter = children.End();
  for( NodeIter iter = children.Begin(); iter != endIter; ++iter )
  {
    Node& child = **iter;
    bool childResourcesComplete = AddRenderablesForTask( updateBufferIndex, child, *layer, renderTask, inheritedDrawMode, currentClippingId, clippingDepth );
    resourcesFinished &= childResourcesComplete;
  }

  return resourcesFinished;
}

} // Anonymous namespace.


RenderTaskProcessor::RenderTaskProcessor()
{
}

RenderTaskProcessor::~RenderTaskProcessor()
{
}

void RenderTaskProcessor::Process( BufferIndex updateBufferIndex,
                                   RenderTaskList& renderTasks,
                                   Layer& rootNode,
                                   SortedLayerPointers& sortedLayers,
                                   GeometryBatcher& geometryBatcher,
                                   RenderInstructionContainer& instructions )
{
  RenderTaskList::RenderTaskContainer& taskContainer = renderTasks.GetTasks();

  if( taskContainer.IsEmpty() )
  {
    // Early-exit if there are no tasks to process
    return;
  }

  // For each render-task:
  //   1) Prepare the render-task
  //   2) Clear the layer-stored lists of renderers (TODO check if the layer is not changed and don't clear in this case)
  //   3) Traverse the scene-graph, filling the lists for the current render-task
  //   4) Prepare render-instructions

  DALI_LOG_INFO( gRenderTaskLogFilter, Debug::General, "RenderTaskProcessor::Process() Offscreens first\n" );

  // First process off screen render tasks - we may need the results of these for the on screen renders
  uint32_t clippingId = 0u;
  bool hasClippingNodes = false;

  RenderTaskList::RenderTaskContainer::ConstIterator endIter = taskContainer.End();
  for( RenderTaskList::RenderTaskContainer::Iterator iter = taskContainer.Begin(); endIter != iter; ++iter )
  {
    RenderTask& renderTask = **iter;

    // Off screen only.
    if( ( ( 0 == renderTask.GetFrameBufferId() ) && ( renderTask.GetFrameBuffer() == 0 ) ) ||
        ( !renderTask.ReadyToRender( updateBufferIndex ) ) )
    {
      // Skip to next task.
      continue;
    }

    Node* sourceNode = renderTask.GetSourceNode();
    DALI_ASSERT_DEBUG( NULL != sourceNode ); // Otherwise Prepare() should return false

    // Check that the source node is not exclusive to another task.
    if( ! CheckExclusivity( *sourceNode, renderTask ) )
    {
      continue;
    }

    Layer* layer = FindLayer( *sourceNode );
    if( !layer )
    {
      // Skip to next task as no layer.
      continue;
    }

    bool resourcesFinished = false;
    if( renderTask.IsRenderRequired() )
    {
      size_t layerCount( sortedLayers.size() );
      for( size_t i(0); i<layerCount; ++i )
      {
        sortedLayers[i]->ClearRenderables();
      }

      resourcesFinished = AddRenderablesForTask( updateBufferIndex,
                                                 *sourceNode,
                                                 *layer,
                                                 renderTask,
                                                 sourceNode->GetDrawMode(),
                                                 clippingId,
                                                 0u );

      renderTask.SetResourcesFinished( resourcesFinished );

      // If the clipping Id is still 0 after adding all Renderables, there is no clipping required for this RenderTaskList.
      hasClippingNodes = clippingId != 0u;

      mRenderInstructionProcessor.Prepare( updateBufferIndex,
                                  sortedLayers,
                                  renderTask,
                                  renderTask.GetCullMode(),
                                  geometryBatcher,
                                  hasClippingNodes,
                                  instructions );
    }
    else
    {
      renderTask.SetResourcesFinished( resourcesFinished );
    }
  }

  DALI_LOG_INFO( gRenderTaskLogFilter, Debug::General, "RenderTaskProcessor::Process() Onscreen\n" );

  // Now that the off screen renders are done we can process on screen render tasks.
  // Reset the clipping Id for the OnScreen render tasks.
  clippingId = 0u;
  for ( RenderTaskList::RenderTaskContainer::Iterator iter = taskContainer.Begin(); endIter != iter; ++iter )
  {
    RenderTask& renderTask = **iter;

    // On screen only.
    if( ( 0 != renderTask.GetFrameBufferId() ) ||
        ( renderTask.GetFrameBuffer() != 0 )   ||
        ( !renderTask.ReadyToRender( updateBufferIndex ) ) )
    {
      // Skip to next task.
      continue;
    }

    Node* sourceNode = renderTask.GetSourceNode();
    DALI_ASSERT_DEBUG( NULL != sourceNode ); // Otherwise Prepare() should return false.

    // Check that the source node is not exclusive to another task.
    if( ! CheckExclusivity( *sourceNode, renderTask ) )
    {
      continue;
    }

    Layer* layer = FindLayer( *sourceNode );
    if( !layer )
    {
      // Skip to next task as no layer.
      continue;
    }

    bool resourcesFinished = false;
    if( renderTask.IsRenderRequired() )
    {
      size_t layerCount( sortedLayers.size() );
      for( size_t i(0); i < layerCount; ++i )
      {
        sortedLayers[i]->ClearRenderables();
      }

      resourcesFinished = AddRenderablesForTask( updateBufferIndex,
                                                 *sourceNode,
                                                 *layer,
                                                 renderTask,
                                                 sourceNode->GetDrawMode(),
                                                 clippingId,
                                                 0u );

      // If the clipping Id is still 0 after adding all Renderables, there is no clipping required for this RenderTaskList.
      hasClippingNodes = clippingId != 0;

      mRenderInstructionProcessor.Prepare( updateBufferIndex,
                                  sortedLayers,
                                  renderTask,
                                  renderTask.GetCullMode(),
                                  geometryBatcher,
                                  hasClippingNodes,
                                  instructions );
    }

    renderTask.SetResourcesFinished( resourcesFinished );
  }
}


} // SceneGraph

} // Internal

} // Dali
