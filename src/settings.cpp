/**
 * Copyright (c) 2011-2017 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/network/settings.hpp>

#include <bitcoin/bitcoin.hpp>

namespace libbitcoin {
namespace network {

using namespace bc::asio;
using namespace bc::message;

// Common default values (no settings context).
settings::settings()
  : threads(0),
    protocol_maximum(version::level::maximum),
    protocol_minimum(version::level::minimum),
    services(version::service::none),
    relay_transactions(false),
    validate_checksum(false),
    inbound_connections(0),
    outbound_connections(8),
    manual_attempt_limit(0),
    connect_batch_size(5),
    connect_timeout_seconds(5),
    channel_handshake_seconds(30),
    channel_heartbeat_minutes(5),
    channel_inactivity_minutes(10),
    channel_expiration_minutes(1440),
    channel_germination_seconds(30),
    host_pool_capacity(0),
    hosts_file("hosts.cache"),
    self(unspecified_network_address),

    // [log]
    debug_file("debug.log"),
    error_file("error.log"),
    archive_directory("archive"),
    rotation_size(0),
    minimum_free_space(0),
    maximum_archive_size(0),
    maximum_archive_files(0),
    statistics_server(unspecified_network_address),
    verbose(false)
{
}

// Use push_back due to initializer_list bug:
// stackoverflow.com/a/20168627/1172329
settings::settings(config::settings context)
  : settings()
{
    // Handle deviations from common defaults.
    switch (context)
    {
        case config::settings::mainnet:
        {
#ifdef LITECOIN
            identifier = 0xdbb6c0fb; 
            inbound_port = 9333;
            seeds.reserve(5);
            seeds.push_back({ "seed-a.litecoin.loshan.co.uk", 9333 });
            seeds.push_back({ "dnsseed.thrasher.io", 9333 });
            seeds.push_back({ "dnsseed.litecointools.com", 9333 });
            seeds.push_back({ "dnsseed.litecoinpool.org", 9333 });
            seeds.push_back({ "dnsseed.koin-project.com", 9333 });        
#else
            identifier = 3652501241;
            inbound_port = 8333;

            // Seeds based on bitcoinstats.com/network/dns-servers
            seeds.reserve(6);
            seeds.push_back({ "seed.bitnodes.io", 8333 });
            seeds.push_back({ "seed.bitcoinstats.com", 8333 });
            seeds.push_back({ "seed.bitcoin.sipa.be", 8333 });
            seeds.push_back({ "dnsseed.bluematt.me", 8333 });
            seeds.push_back({ "seed.bitcoin.jonasschnelli.ch", 8333 });
            seeds.push_back({ "dnsseed.bitcoin.dashjr.org", 8333 });
#endif
          break;
        }

        case config::settings::testnet:
        {
#ifdef LITECOIN
            identifier = 4056470269;
            inbound_port = 19335;
            seeds.reserve(2);
            seeds.push_back({ "testnet-seed.litecointools.com", 19335 });
            seeds.push_back({ "seed-b.litecoin.loshan.co.uk", 19335 });
#else
#ifdef WITH_EXTBLK
            identifier = 3298929661;
            inbound_port = 58901;
            seeds.reserve(1);
            seeds.push_back({ "45.33.50.84", 58901 });
#else
            identifier = 118034699;
            inbound_port = 18333;

            seeds.reserve(3);
            seeds.push_back({ "testnet-seed.bitcoin.petertodd.org", 18333 });
            seeds.push_back({ "testnet-seed.bitcoin.schildbach.de", 18333 });
            seeds.push_back({ "testnet-seed.bluematt.me", 18333 });
#endif
#endif
          break;
        }

        default:
        case config::settings::none:
        {
        }
    }
}

duration settings::connect_timeout() const
{
    return seconds(connect_timeout_seconds);
}

duration settings::channel_handshake() const
{
    return seconds(channel_handshake_seconds);
}

duration settings::channel_heartbeat() const
{
    return minutes(channel_heartbeat_minutes);
}

duration settings::channel_inactivity() const
{
    return minutes(channel_inactivity_minutes);
}

duration settings::channel_expiration() const
{
    return minutes(channel_expiration_minutes);
}

duration settings::channel_germination() const
{
    return seconds(channel_germination_seconds);
}

} // namespace network
} // namespace libbitcoin
