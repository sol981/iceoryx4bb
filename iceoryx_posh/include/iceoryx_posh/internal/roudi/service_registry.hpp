// Copyright (c) 2019 by Robert Bosch GmbH. All rights reserved.
// Copyright (c) 2021 - 2022 by Apex.AI Inc. All rights reserved.
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
#ifndef IOX_POSH_ROUDI_SERVICE_REGISTRY_HPP
#define IOX_POSH_ROUDI_SERVICE_REGISTRY_HPP

#include "iceoryx_hoofs/cxx/expected.hpp"
#include "iceoryx_hoofs/cxx/vector.hpp"
#include "iceoryx_posh/capro/service_description.hpp"

#include <cstdint>
#include <map>
#include <utility>

namespace iox
{
namespace roudi
{
class ServiceRegistry
{
  public:
    enum class Error
    {
        SERVICE_REGISTRY_FULL,
    };

    using ReferenceCounter_t = uint64_t;
    struct ServiceDescriptionEntry
    {
        capro::ServiceDescription serviceDescription{};
        ReferenceCounter_t referenceCounter = 0U;
    };

    /// @todo #415 should be connected with iox::MAX_NUMBER_OF_SERVICES
    static constexpr uint32_t MAX_SERVICE_DESCRIPTIONS = 100U;
    using ServiceDescriptionVector_t = cxx::vector<ServiceDescriptionEntry, MAX_SERVICE_DESCRIPTIONS>;

    /// @brief Adds given service description to registry
    /// @param[in] serviceDescription, service to be added
    /// @return ServiceRegistryError, error wrapped in cxx::expected
    cxx::expected<Error> add(const capro::ServiceDescription& serviceDescription) noexcept;

    /// @brief Removes given service description from registry if service is found
    /// @param[in] serviceDescription, service to be removed
    void remove(const capro::ServiceDescription& serviceDescription) noexcept;

    /// @brief Searches for given service description in registry
    /// @param[in] searchResult, reference to the vector which will be filled with the results
    /// @param[in] service, string or wildcard (= iox::cxx::nullopt) to search for
    /// @param[in] instance, string or wildcard (= iox::cxx::nullopt) to search for
    void find(ServiceDescriptionVector_t& searchResult,
              const cxx::optional<capro::IdString_t>& service,
              const cxx::optional<capro::IdString_t>& instance) const noexcept;

    /// @brief Returns all service descriptions as copy
    /// @return ServiceDescriptionVector_t, copy of complete service registry
    const ServiceDescriptionVector_t getServices() const noexcept;

  private:
    /// @todo #859 replace std::multimap with prefix tree
    ::std::multimap<capro::IdString_t, uint64_t> m_serviceMap;
    ::std::multimap<capro::IdString_t, uint64_t> m_instanceMap;
    ServiceDescriptionVector_t m_serviceDescriptionVector;
};
} // namespace roudi
} // namespace iox

#endif // IOX_POSH_ROUDI_SERVICE_REGISTRY_HPP
