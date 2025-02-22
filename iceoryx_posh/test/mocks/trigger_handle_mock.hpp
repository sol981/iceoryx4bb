// Copyright (c) 2022 by Apex.AI Inc. All rights reserved.
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
//
// SPDX-License-Identifier: Apache-2.0

#ifndef IOX_POSH_MOCKS_TRIGGER_HANDLE_MOCK_HPP
#define IOX_POSH_MOCKS_TRIGGER_HANDLE_MOCK_HPP

#include "iceoryx_posh/popo/trigger_handle.hpp"

#include "test.hpp"

using namespace ::testing;
using ::testing::_;

class MockTriggeHandle
{
  public:
    MockTriggeHandle(iox::popo::ConditionVariableData&,
                     const iox::cxx::function<void(uint64_t)>,
                     const uint64_t) noexcept {};
    MockTriggeHandle() noexcept
    {
    }
    ~MockTriggeHandle() = default;

    MockTriggeHandle(const MockTriggeHandle&) = delete;
    MockTriggeHandle& operator=(const MockTriggeHandle&) = delete;
    MockTriggeHandle(MockTriggeHandle&& other) noexcept
        : triggerId(other.triggerId)
    {
    }
    MockTriggeHandle& operator=(MockTriggeHandle&& rhs) noexcept
    {
        triggerId = rhs.triggerId;
        return *this;
    }

#ifdef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif
    MOCK_METHOD(bool, isValid, (), (const, noexcept));
    MOCK_METHOD(bool, wasTriggered, (), (const, noexcept));
    MOCK_METHOD(void, trigger, (), (noexcept));
    MOCK_METHOD(void, reset, (), (noexcept));
    MOCK_METHOD(void, invalidate, (), (noexcept));
    MOCK_METHOD(uint64_t, getUniqueId, (), (const, noexcept));
    MOCK_METHOD(iox::popo::ConditionVariableData*, getConditionVariableData, (), (noexcept));
#ifdef __clang__
#pragma GCC diagnostic pop
#endif

    explicit operator bool() const
    {
        return operatorBoolMock();
    }

#ifdef __clang__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#endif
    MOCK_METHOD(bool, operatorBoolMock, (), (const));
#ifdef __clang__
#pragma GCC diagnostic pop
#endif

    uint64_t triggerId{0};
};

#endif // IOX_POSH_MOCKS_TRIGGER_HANDLE_MOCK_HPP
