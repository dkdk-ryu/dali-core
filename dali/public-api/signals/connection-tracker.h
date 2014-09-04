#ifndef __DALI_CONNECTION_TRACKER_H__
#define __DALI_CONNECTION_TRACKER_H__

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
#include <dali/public-api/common/dali-common.h>
#include <dali/public-api/common/vector-wrapper.h>
#include <dali/public-api/signals/connection-tracker-interface.h>

namespace Dali DALI_IMPORT_API
{

class CallbackBase;
class SlotObserver;
class SlotConnection;

/**
 * @brief Connection tracker concrete implementation
 */
class ConnectionTracker : public ConnectionTrackerInterface
{
public:

  /**
   * @brief Constructor.
   */
  ConnectionTracker();

  /**
   * @brief Virtual destructor.
   */
  virtual ~ConnectionTracker();

  /**
   * @brief Disconnect all signals from this object.
   */
  void DisconnectAll();

  /**
   * @copydoc ConnectionTrackerInterface::SignalConnected
   */
  virtual void SignalConnected( SlotObserver* slotObserver, CallbackBase* callback );

  /**
   * @copydoc ConnectionTrackerInterface::SignalDisconnected
   */
  virtual void SignalDisconnected( SlotObserver* slotObserver, CallbackBase* callback );

  /**
   * @brief returns the connection count
   * @return the connection count
   */
  std::size_t GetConnectionCount() const;

private:

  ConnectionTracker( const ConnectionTracker& );            ///< undefined copy constructor
  ConnectionTracker& operator=( const ConnectionTracker& ); ///< undefined assignment operator

private:

  std::vector< SlotConnection* > mConnections; ///< Vector of connection pointers
};

} // namespace Dali

#endif // __DALI_CONNECTION_TRACKER_H__
