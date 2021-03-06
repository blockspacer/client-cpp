/*
 * Copyright 2018, TeamDev. All rights reserved.
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

#ifndef SPINE_QUERYFACTORY_H
#define SPINE_QUERYFACTORY_H

#include <memory>
#include <vector>
#include <Poco/UUIDGenerator.h>
#include <spine/client/query.pb.h>

#include "spine/types.h"
#include "spine/util/message_utils.hpp"
#include "spine/util/target_utils.hpp"

namespace google {
namespace protobuf {
    class FieldMask;
}}

namespace spine
{

namespace core
{
    class ActorContext;
}

namespace client
{

/**
 * Public API for creating \b Query instances, using the ActorRequestFactory
 * configuration.
 *
 * @see ActorRequestFactory:query_factory()
 *
 * @author Vladimir Moiseiev
 */

class QueryFactory
{
public:
    QueryFactory(std::unique_ptr<core::ActorContext>&& actor_context);

public:
    /**
     * Creates a \b Query to read all states of a certain entity.
     *
     * @tparam T Protobuf Message type of a target entity
     * @return an instance of Query formed according to the passed parameters
     */
    template <typename T, typename = enable_param_if_protobuf_message<T>>
    QueryPtr all()
    {
        return make_query(
                T::descriptor()->file()->options().GetExtension(type_url_prefix),
                T::descriptor()->full_name()
        );
    };

    /**
     * Creates a \b Query to read all entity states with the \b FieldMask
     * applied to each of the results.
     *
     * Allows to set property paths for a \b FieldMask, applied to each of the query
     * results. This processing is performed according to the
     * [FieldMask specs](https://goo.gl/tW5wIU).
     *
     * In case the \b paths array contains entries inapplicable to the resulting entity
     * (e.g. a \b path references a missing field), such invalid paths
     * are silently ignored.
     *
     * @tparam T Protobuf Message type of a target entity
     * @param masks   the property paths for the \b FieldMask applied
     *                    to each of results
     * @return an instance of \b Query formed according to the passed parameters
     */
    template <typename T, typename = enable_param_if_protobuf_message<T>>
    QueryPtr all_with_mask(const std::vector<std::string>& masks)
    {
        return make_query(
                T::descriptor()->file()->options().GetExtension(type_url_prefix),
                T::descriptor()->full_name(),
                masks
        );
    };

    /**
     * Creates a \b Query to read certain entity states by IDs.
     *
     * Allows to specify a set of identifiers to be used during the \b Query processing.
     * The processing results will contain only the entities, which IDs are present among
     * the \b ids.
     *
     * Unlike \b by_ids_with_masks(), the \b Query processing
     * will not change the resulting entities.
     *
     * @tparam T Protobuf Message type of a target entity
     * @tparam I Protobuf Message type of the entity IDs
     * @param ids         the entity IDs of interest
     * @return an instance of  Query formed according to he passed parameters
     */
    template <typename T, typename = enable_param_if_protobuf_message<T>,
              typename I, typename = enable_param_if_protobuf_message<I>
              >
    QueryPtr by_ids(const std::vector<std::unique_ptr<I>>& ids)
    {
        return make_query(
                T::descriptor()->file()->options().GetExtension(type_url_prefix),
                T::descriptor()->full_name(),
                ids
        );
    };

    /**
     * Creates a \b Query to read certain entity states by IDs with the \b FieldMask
     * applied to each of the results.
     *
     * Allows to specify a set of identifiers to be used during the \b Query processing.
     * The processing results will contain only the entities, which IDs are present among
     * the \b ids.
     *
     * Allows to set property paths for a \b FieldMask, applied to each of the query
     * results. This processing is performed according to the
     * [FieldMask specs](https://goo.gl/tW5wIU).
     *
     * In case the \b paths array contains entries inapplicable to the resulting entity
     * (e.g. a \b path references a missing field),
     * such invalid paths are silently ignored.
     *
     * @tparam T Protobuf Message type of a target entity
     * @tparam I Protobuf Message type of the entity IDs
     * @param ids         the entity IDs of interest
     * @param masks   the property paths for the \b FieldMask applied
     *                    to each of results
     * @return an instance of \b Query formed according to the passed parameters
     */
    template <typename T, typename = enable_param_if_protobuf_message<T>,
              typename I, typename = enable_param_if_protobuf_message<I>
              >
    QueryPtr by_ids_with_masks(const std::vector<std::string>& masks,
                               const std::vector<std::unique_ptr<I>>& ids)
    {
        return make_query(
                T::descriptor()->file()->options().GetExtension(type_url_prefix),
                T::descriptor()->full_name(),
                masks,
                ids
        );
    };

private:
    QueryId *create_query_id();
    QueryPtr make_query(const std::string& prefix, const std::string& type);
    QueryPtr make_query(const std::string& prefix, const std::string& type,
                        const std::vector<std::string>& masks);

    template <typename T, typename = enable_param_if_protobuf_message<T>>
    QueryPtr make_query(const std::string& prefix, const std::string& type,
                        const std::vector<std::unique_ptr<T>>& ids)
    {
        std::unique_ptr<Target> target = compose_target(prefix, type, ids);
        return for_query(std::move(target));
    }

    template <typename T, typename = enable_param_if_protobuf_message<T>>
    QueryPtr make_query(const std::string& prefix, const std::string& type,
                        const std::vector<std::string>& masks,
                        const std::vector<std::unique_ptr<T>>& ids)
    {
        std::unique_ptr<Target> target = compose_target(prefix, type, ids);
        return for_query(std::move(target), std::move(make_field_mask(masks)));
    }


    std::unique_ptr<Query> for_query(std::unique_ptr<Target>&& target);
    std::unique_ptr<Query> for_query(std::unique_ptr<Target>&& target,
                                     std::unique_ptr<google::protobuf::FieldMask> && field_mask);

    std::unique_ptr<google::protobuf::FieldMask> make_field_mask(const std::vector<std::string>& masks);
private:
    std::unique_ptr<core::ActorContext> actor_context_;
    Poco::UUIDGenerator uuid_generator_;
};

}} //namespace

#endif //SPINE_QUERYFACTORY_H
