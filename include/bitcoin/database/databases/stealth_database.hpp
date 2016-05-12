/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef LIBBITCOIN_DATABASE_STEALTH_DATABASE_HPP
#define LIBBITCOIN_DATABASE_STEALTH_DATABASE_HPP

#include <cstdint>
#include <memory>
#include <boost/filesystem.hpp>
#include <bitcoin/database/define.hpp>
#include <bitcoin/database/memory/memory.hpp>
#include <bitcoin/database/memory/memory_map.hpp>
#include <bitcoin/database/primitives/record_manager.hpp>

namespace libbitcoin {
namespace database {

class BCD_API stealth_database
{
public:
    typedef std::function<void(memory_ptr)> write_function;

    stealth_database(const boost::filesystem::path& rows_filename,
        std::shared_ptr<shared_mutex> mutex=nullptr);

    /// Initialize a new stealth database.
    void create();

    /// Call before using the database.
    void start();

    /// Call stop to unload the memory map.
    bool stop();

    /// Linearly scan all entries, discarding those after from_height.
    chain::stealth_compact::list scan(const binary& filter,
        size_t from_height) const;

    /// Add a stealth row to the database.
    void store(uint32_t prefix, uint32_t height,
        const chain::stealth_compact& row);

    /// Delete all rows after and including from_height (no implemented).
    void unlink(size_t from_height);

    /// Synchronise storage with disk so things are consistent.
    /// Should be done at the end of every block write.
    void sync();

private:
    void write_index();
    array_index read_index(size_t from_height) const;

    // Row entries containing stealth tx data.
    memory_map rows_file_;
    record_manager rows_manager_;
};

} // namespace database
} // namespace libbitcoin

#endif