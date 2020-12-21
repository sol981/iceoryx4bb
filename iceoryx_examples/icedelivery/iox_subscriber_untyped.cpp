// Copyright (c) 2020 by Robert Bosch GmbH, Apex.AI Inc. All rights reserved.
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

#include "iceoryx_posh/popo/subscriber.hpp"
#include "iceoryx_posh/popo/user_trigger.hpp"
#include "iceoryx_posh/popo/wait_set.hpp"
#include "iceoryx_posh/runtime/posh_runtime.hpp"
#include "topic_data.hpp"

#include <csignal>
#include <iostream>

bool killswitch = false;

static void sigHandler(int f_sig [[gnu::unused]])
{
    // caught SIGINT, now exit gracefully
    killswitch = true;
}

int main()
{
    // register sigHandler for SIGINT
    signal(SIGINT, sigHandler);

    // initialize runtime
    iox::runtime::PoshRuntime::initRuntime("iox-ex-subscriber-untyped");

    // initialized subscriber
    iox::popo::SubscriberOptions subscriberOptions;
    subscriberOptions.queueCapacity = 10U;
    iox::popo::UntypedSubscriber untypedSubscriber({"Radar", "FrontLeft", "Object"}, subscriberOptions);
    untypedSubscriber.subscribe();

    // run until interrupted by Ctrl-C
    while (!killswitch)
    {
        if (untypedSubscriber.getSubscriptionState() == iox::SubscribeState::SUBSCRIBED)
        {
            untypedSubscriber.take()
                .and_then([](iox::popo::Sample<const void>& sample) {
                    auto object = static_cast<const RadarObject*>(sample.get());
                    std::cout << "Got value: (" << object->x << ", " << object->y << ", " << object->z << ")"
                              << std::endl;
                })
                .if_empty([] { std::cout << "Didn't get a value, but do something anyway." << std::endl; })
                .or_else([](iox::popo::ChunkReceiveError) { std::cout << "Error receiving chunk." << std::endl; });
        }
        else
        {
            std::cout << "Not subscribed!" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    untypedSubscriber.unsubscribe();

    return (EXIT_SUCCESS);
}
