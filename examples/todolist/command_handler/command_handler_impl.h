/*
* Copyright 2018, TeamDev Ltd. All rights reserved.
*
* Redistribution and use in source and/or binary forms, with or without
* modification, must retain the above copyright notice and the following
* disclaimer.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef TODOLIST_COMMAND_HANDLER_IMPL_H
#define TODOLIST_COMMAND_HANDLER_IMPL_H

#include "grpc++/grpc++.h"
#include "spine/client/command_service.grpc.pb.h"
#include <spine/client/query.pb.h>

#include "spine/types.h"
#include "spine/command_factory.h"
#include "spine/actor_request_factory_params.h"
#include "spine/actor_request_factory.h"
#include "spine/query_factory.h"

#include "command_handler.h"

#include <memory>

using namespace spine::client;

class CommandHandlerImpl
	:	public CommandHandler
{
	
public:

	CommandHandlerImpl();

	void post_command(spine::examples::todolist::CreateBasicTask & client_task);
	spine::examples::todolist::TaskListView const & get_tasks();

private:

	std::unique_ptr<spine::core::UserId> make_user_id(const std::string & value);
	std::unique_ptr<spine::time::ZoneOffset> make_zone_offset(const std::string &zone_id, int amount);

	std::shared_ptr<grpc::Channel> channel_;
	std::unique_ptr<CommandService::Stub> stub_;

	ActorRequestFactoryParams parameters_;
	CommandFactoryPtr command_factory_;
};

#endif // TODOLIST_COMMAND_HANDLER_IMPL_H