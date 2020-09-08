// Copyright (c) 2020 by Robert Bosch GmbH. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#include "iceoryx_posh/internal/popo/building_blocks/condition_variable_data.hpp"
#include "iceoryx_posh/internal/popo/ports/subscriber_port_user.hpp"
#include "iceoryx_posh/mepoo/chunk_header.hpp"

using namespace iox;
using namespace iox::cxx;
using namespace iox::popo;
using namespace iox::capro;
using namespace iox::mepoo;

extern "C" {
#include "iceoryx_binding_c/subscriber.h"
}

SubscriberPortData* Subscriber_create(const char* const service,
                                      const char* const instance,
                                      const char* const event,
                                      const uint64_t historyRequest)
{
    return new SubscriberPortData(ServiceDescription{IdString(TruncateToCapacity, service),
                                                     IdString(TruncateToCapacity, instance),
                                                     IdString(TruncateToCapacity, event)},
                                  "AllHailHypnotoad!",
                                  VariantQueueTypes::SoFi_SingleProducerSingleConsumer,
                                  historyRequest);
}

void Subscriber_destroy(SubscriberPortData* const self)
{
    delete self;
}

void Subscriber_subscribe(SubscriberPortData* const self, const uint64_t queueCapacity)
{
    SubscriberPortUser(self).subscribe(queueCapacity);
}

void Subscriber_unsubscribe(SubscriberPortData* const self)
{
    SubscriberPortUser(self).unsubscribe();
}

iox_SubscribeState Subscriber_getSubscriptionState(SubscriberPortData* const self)
{
    switch (SubscriberPortUser(self).getSubscriptionState())
    {
    case SubscribeState::NOT_SUBSCRIBED:
        return iox_SubscribeState::SubscribeState_NOT_SUBSCRIBED;
    case SubscribeState::SUBSCRIBE_REQUESTED:
        return iox_SubscribeState::SubscribeState_SUBSCRIBE_REQUESTED;
    case SubscribeState::SUBSCRIBED:
        return iox_SubscribeState::SubscribeState_SUBSCRIBED;
    case SubscribeState::UNSUBSCRIBE_REQUESTED:
        return iox_SubscribeState::SubscribeState_UNSUBSCRIBE_REQUESTED;
    case SubscribeState::WAIT_FOR_OFFER:
        return iox_SubscribeState::SubscribeState_WAIT_FOR_OFFER;
    default:
        return iox_SubscribeState::SubscribeState_UNDEFINED;
    }
}

iox_popo_ChunkReceiveResult Subscriber_getChunk(SubscriberPortData* const self, const void** const payload)
{
    auto result = SubscriberPortUser(self).getChunk();
    if (result.has_error())
    {
        return (result.get_error() == ChunkReceiveError::TOO_MANY_CHUNKS_HELD_IN_PARALLEL)
                   ? ChunkReceiveResult_TOO_MANY_CHUNKS_HELD_IN_PARALLEL
                   : ChunkReceiveResult_INTERNAL_ERROR;
    }

    if (!result->has_value())
    {
        return ChunkReceiveResult_NO_CHUNK_RECEIVED;
    }

    *payload = (**result)->payload();
    return ChunkReceiveResult_SUCCESS;
}

void Subscriber_releaseChunk(SubscriberPortData* const self, const void* const chunk)
{
    SubscriberPortUser(self).releaseChunk(convertPayloadPointerToChunkHeader(chunk));
}

void Subscriber_releaseQueuedChunks(SubscriberPortData* const self)
{
    SubscriberPortUser(self).releaseQueuedChunks();
}

bool Subscriber_hasNewChunks(SubscriberPortData* const self)
{
    return SubscriberPortUser(self).hasNewChunks();
}

bool Subscriber_hasLostChunks(SubscriberPortData* const self)
{
    return SubscriberPortUser(self).hasLostChunks();
}

bool Subscriber_attachConditionVariable(SubscriberPortData* const self, ConditionVariableData* const cvHandle)
{
    return SubscriberPortUser(self).attachConditionVariable(cvHandle);
}

bool Subscriber_detachConditionVariable(SubscriberPortData* const self)
{
    return SubscriberPortUser(self).detachConditionVariable();
}

bool Subscriber_isConditionVariableAttached(SubscriberPortData* const self)
{
    return SubscriberPortUser(self).isConditionVariableAttached();
}

