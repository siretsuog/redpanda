// Copyright 2020 Redpanda Data, Inc.
//
// Use of this software is governed by the Business Source License
// included in the file licenses/BSL.md
//
// As of the Change Date specified in that file, in accordance with
// the Business Source License, use of this software will be governed
// by the Apache License, Version 2.0

#include "compression/compression.h"

#include "compression/internal/gzip_compressor.h"
#include "compression/internal/lz4_frame_compressor.h"
#include "compression/internal/snappy_java_compressor.h"
#include "compression/internal/zstd_compressor.h"
#include "vassert.h"
#include "vlog.h"

namespace compression {

/*
 * There are no users of this, but we need to make sure it isn't compiled away
 * in release mode so that it still shows up in the registered loggers list
 * which rpk needs to be in sync with.
 *
 * Putting it here instead of in its own compilation unit seemed to have tricked
 * the optimizer. Added a log statement below too, just for good measure.
 *
 * As soon as rpk supports logger name discovery via admin api this can be fully
 * removed.
 */
ss::logger complog{"compression"};

iobuf compressor::compress(const iobuf& io, type t) {
    switch (t) {
    case type::none:
        throw std::runtime_error("compressor: nothing to compress for 'none'");
    case type::gzip:
        return internal::gzip_compressor::compress(io);
    case type::snappy:
        return internal::snappy_java_compressor::compress(io);
    case type::lz4:
        return internal::lz4_frame_compressor::compress(io);
    case type::zstd:
        return internal::zstd_compressor::compress(io);
    default:
        vlog(complog.error, "Cannot compress type {}", t);
        vassert(false, "Cannot compress type {}", t);
    }
}
iobuf compressor::uncompress(const iobuf& io, type t) {
    if (io.empty()) {
        throw std::runtime_error(
          fmt::format("Asked to decomrpess:{} an empty buffer:{}", (int)t, io));
    }
    switch (t) {
    case type::none:
        throw std::runtime_error(
          "compressor: nothing to uncompress for 'none'");
    case type::gzip:
        return internal::gzip_compressor::uncompress(io);
    case type::snappy:
        return internal::snappy_java_compressor::uncompress(io);
    case type::lz4:
        return internal::lz4_frame_compressor::uncompress(io);
    case type::zstd:
        return internal::zstd_compressor::uncompress(io);
    default:
        vassert(false, "Cannot uncompress type {}", t);
    }
}

} // namespace compression
