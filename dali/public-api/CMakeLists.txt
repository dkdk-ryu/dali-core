#
# Add public-api source files here
#
SET(SOURCES ${SOURCES}
  ${CMAKE_CURRENT_SOURCE_DIR}/actors/actor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/actors/camera-actor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/actors/custom-actor.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/actors/custom-actor-impl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/actors/layer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/animation/animation.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/animation/alpha-function.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/animation/constraint.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/animation/constraint-source.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/animation/key-frames.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/animation/linear-constrainer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/animation/path.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/animation/time-period.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/common/constants.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/common/dali-common.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/common/dali-vector.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/common/extents.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/common/stage.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/events/gesture.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/events/gesture-detector.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/events/hover-event.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/events/key-event.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/events/long-press-gesture.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/events/long-press-gesture-detector.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/events/wheel-event.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/events/pan-gesture.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/events/pan-gesture-detector.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/events/pinch-gesture.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/events/pinch-gesture-detector.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/events/tap-gesture.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/events/tap-gesture-detector.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/events/touch-point.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/events/touch-event.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/events/touch-data.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/images/image.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/images/pixel.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/images/pixel-data.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/images/buffer-image.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/images/frame-buffer-image.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/images/encoded-buffer-image.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/images/resource-image.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/images/native-image.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/math/compile-time-math.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/math/degree.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/math/matrix.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/math/matrix3.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/math/quaternion.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/math/vector2.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/math/vector3.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/math/vector4.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/any.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/base-handle.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/handle.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/base-object.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/object-registry.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-array.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-conditions.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-input.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-key.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-map.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-notification.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-types.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-value.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/ref-object.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/render-tasks/render-task.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/render-tasks/render-task-list.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/rendering/frame-buffer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/rendering/geometry.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/rendering/property-buffer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/rendering/texture.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/rendering/texture-set.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/rendering/renderer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/rendering/sampler.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/rendering/shader.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/signals/callback.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/signals/connection-tracker.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/signals/connection-tracker-interface.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/signals/signal-slot-connections.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/signals/signal-slot-observers.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/signals/base-signal.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/signals/functor-delegate.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/type-info.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/type-registry.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/object/weak-handle.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/dali-core-version.cpp

  PARENT_SCOPE )

#
# Add public header files here
#
SET(PUBLIC_API_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/dali-core.h
  ${CMAKE_CURRENT_SOURCE_DIR}/dali-core-version.h

  ${CMAKE_CURRENT_SOURCE_DIR}/actors/actor.h
  ${CMAKE_CURRENT_SOURCE_DIR}/actors/actor-enumerations.h
  ${CMAKE_CURRENT_SOURCE_DIR}/actors/camera-actor.h
  ${CMAKE_CURRENT_SOURCE_DIR}/actors/custom-actor.h
  ${CMAKE_CURRENT_SOURCE_DIR}/actors/custom-actor-impl.h
  ${CMAKE_CURRENT_SOURCE_DIR}/actors/draw-mode.h
  ${CMAKE_CURRENT_SOURCE_DIR}/actors/layer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/actors/sampling.h

  ${CMAKE_CURRENT_SOURCE_DIR}/animation/alpha-function.h
  ${CMAKE_CURRENT_SOURCE_DIR}/animation/animation.h
  ${CMAKE_CURRENT_SOURCE_DIR}/animation/constraint.h
  ${CMAKE_CURRENT_SOURCE_DIR}/animation/constraints.h
  ${CMAKE_CURRENT_SOURCE_DIR}/animation/constraint-source.h
  ${CMAKE_CURRENT_SOURCE_DIR}/animation/key-frames.h
  ${CMAKE_CURRENT_SOURCE_DIR}/animation/linear-constrainer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/animation/path.h
  ${CMAKE_CURRENT_SOURCE_DIR}/animation/time-period.h

  ${CMAKE_CURRENT_SOURCE_DIR}/common/compile-time-assert.h
  ${CMAKE_CURRENT_SOURCE_DIR}/common/constants.h
  ${CMAKE_CURRENT_SOURCE_DIR}/common/dali-common.h
  ${CMAKE_CURRENT_SOURCE_DIR}/common/dali-vector.h
  ${CMAKE_CURRENT_SOURCE_DIR}/common/extents.h
  ${CMAKE_CURRENT_SOURCE_DIR}/common/intrusive-ptr.h
  ${CMAKE_CURRENT_SOURCE_DIR}/common/loading-state.h
  ${CMAKE_CURRENT_SOURCE_DIR}/common/stage.h
  ${CMAKE_CURRENT_SOURCE_DIR}/common/type-traits.h
  ${CMAKE_CURRENT_SOURCE_DIR}/common/vector-wrapper.h
  ${CMAKE_CURRENT_SOURCE_DIR}/common/view-mode.h

  ${CMAKE_CURRENT_SOURCE_DIR}/events/device.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/gesture.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/gesture-detector.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/hover-event.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/key-event.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/long-press-gesture.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/long-press-gesture-detector.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/wheel-event.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/pan-gesture.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/pan-gesture-detector.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/pinch-gesture.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/pinch-gesture-detector.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/point-state.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/tap-gesture.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/tap-gesture-detector.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/touch-point.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/touch-event.h
  ${CMAKE_CURRENT_SOURCE_DIR}/events/touch-data.h

  ${CMAKE_CURRENT_SOURCE_DIR}/images/buffer-image.h
  ${CMAKE_CURRENT_SOURCE_DIR}/images/encoded-buffer-image.h
  ${CMAKE_CURRENT_SOURCE_DIR}/images/frame-buffer-image.h
  ${CMAKE_CURRENT_SOURCE_DIR}/images/image.h
  ${CMAKE_CURRENT_SOURCE_DIR}/images/image-operations.h
  ${CMAKE_CURRENT_SOURCE_DIR}/images/pixel.h
  ${CMAKE_CURRENT_SOURCE_DIR}/images/pixel-data.h
  ${CMAKE_CURRENT_SOURCE_DIR}/images/resource-image.h
  ${CMAKE_CURRENT_SOURCE_DIR}/images/native-image.h
  ${CMAKE_CURRENT_SOURCE_DIR}/images/native-image-interface.h

  ${CMAKE_CURRENT_SOURCE_DIR}/math/angle-axis.h
  ${CMAKE_CURRENT_SOURCE_DIR}/math/compile-time-math.h
  ${CMAKE_CURRENT_SOURCE_DIR}/math/degree.h
  ${CMAKE_CURRENT_SOURCE_DIR}/math/math-utils.h
  ${CMAKE_CURRENT_SOURCE_DIR}/math/matrix.h
  ${CMAKE_CURRENT_SOURCE_DIR}/math/matrix3.h
  ${CMAKE_CURRENT_SOURCE_DIR}/math/quaternion.h
  ${CMAKE_CURRENT_SOURCE_DIR}/math/radian.h
  ${CMAKE_CURRENT_SOURCE_DIR}/math/random.h
  ${CMAKE_CURRENT_SOURCE_DIR}/math/rect.h
  ${CMAKE_CURRENT_SOURCE_DIR}/math/uint-16-pair.h
  ${CMAKE_CURRENT_SOURCE_DIR}/math/vector2.h
  ${CMAKE_CURRENT_SOURCE_DIR}/math/vector3.h
  ${CMAKE_CURRENT_SOURCE_DIR}/math/vector4.h
  ${CMAKE_CURRENT_SOURCE_DIR}/math/viewport.h

  ${CMAKE_CURRENT_SOURCE_DIR}/object/any.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/base-handle.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/base-object.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/handle.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/object-registry.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-array.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-conditions.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-index-ranges.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-input.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-key.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-map.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-notification-declarations.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-notification.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-types.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property-value.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/property.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/ref-object.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/type-info.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/type-registry.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/type-registry-helper.h
  ${CMAKE_CURRENT_SOURCE_DIR}/object/weak-handle.h

  ${CMAKE_CURRENT_SOURCE_DIR}/render-tasks/render-task.h
  ${CMAKE_CURRENT_SOURCE_DIR}/render-tasks/render-task-list.h

  ${CMAKE_CURRENT_SOURCE_DIR}/rendering/frame-buffer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/rendering/geometry.h
  ${CMAKE_CURRENT_SOURCE_DIR}/rendering/property-buffer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/rendering/texture.h
  ${CMAKE_CURRENT_SOURCE_DIR}/rendering/texture-set.h
  ${CMAKE_CURRENT_SOURCE_DIR}/rendering/renderer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/rendering/sampler.h
  ${CMAKE_CURRENT_SOURCE_DIR}/rendering/shader.h

  ${CMAKE_CURRENT_SOURCE_DIR}/size-negotiation/relayout-container.h

  ${CMAKE_CURRENT_SOURCE_DIR}/signals/base-signal.h
  ${CMAKE_CURRENT_SOURCE_DIR}/signals/callback.h
  ${CMAKE_CURRENT_SOURCE_DIR}/signals/connection-tracker-interface.h
  ${CMAKE_CURRENT_SOURCE_DIR}/signals/connection-tracker.h
  ${CMAKE_CURRENT_SOURCE_DIR}/signals/dali-signal.h
  ${CMAKE_CURRENT_SOURCE_DIR}/signals/functor-delegate.h
  ${CMAKE_CURRENT_SOURCE_DIR}/signals/signal-slot-connections.h
  ${CMAKE_CURRENT_SOURCE_DIR}/signals/signal-slot-observers.h
  ${CMAKE_CURRENT_SOURCE_DIR}/signals/slot-delegate.h

  PARENT_SCOPE )
