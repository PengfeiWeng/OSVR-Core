/** @file
    @brief Header

    @date 2014

    @author
    Ryan Pavlik
    <ryan@sensics.com>
    <http://sensics.com>
*/

// Copyright 2014 Sensics, Inc.
//
// All rights reserved.
//
// (Final version intended to be licensed under
// the Apache License, Version 2.0)

#ifndef INCLUDED_ServerImpl_h_GUID_BA15589C_D1AD_4BBE_4F93_8AC87043A982
#define INCLUDED_ServerImpl_h_GUID_BA15589C_D1AD_4BBE_4F93_8AC87043A982

// Internal Includes
#include <ogvr/Server/Server.h>
#include <ogvr/Connection/ConnectionPtr.h>
#include <ogvr/Util/SharedPtr.h>
#include <ogvr/PluginHost/RegistrationContext_fwd.h>

// Library/third-party includes
#include <boost/noncopyable.hpp>
#include <util/RunLoopManagerBoost.h>
#include <boost/thread.hpp>

// Standard includes
// - none

namespace ogvr {
namespace server {
    /// @brief Private implementation class for Server.
    class ServerImpl : boost::noncopyable {
      public:
        /// @brief Constructor
        ServerImpl(connection::ConnectionPtr const &conn);

        /// @brief Destructor (stops the thread first)
        ~ServerImpl();

        /// @brief Launch a thread running the server.
        void start();

        /// @brief Launch a thread running the server, and block until the
        /// server shuts down.
        void startAndAwaitShutdown();

        /// @brief Signal the server to stop, and block until it does so.
        void stop();

        /// @brief Signal the server to stop (if it is running) but return
        /// immediately.
        void signalStop();

        /// @brief Load named plugin
        void loadPlugin(std::string const &pluginName);

        /// @brief Trigger all hardware poll callbacks
        void triggerHardwarePoll();

        /// @brief The method called from the server thread repeatedly.
        /// @returns true if the loop should continue running
        bool loop();

      private:
        /// @brief Internal function to call a callable if the thread isn't
        /// running, or to queue up the callable if it is running.
        template <typename Callable> void m_callControlled(Callable f);
        connection::ConnectionPtr m_conn;
        shared_ptr<pluginhost::RegistrationContext> m_ctx;

        /// @brief Mutex controlling ability to check/change state of run loop
        boost::mutex m_runControl;
        /// @name Protected by m_runControl
        /// @{
        boost::thread m_thread;
        ::util::RunLoopManagerBoost m_run;
        bool m_running;
        /// @}
    };

} // namespace server
} // namespace ogvr

#endif // INCLUDED_ServerImpl_h_GUID_BA15589C_D1AD_4BBE_4F93_8AC87043A982
