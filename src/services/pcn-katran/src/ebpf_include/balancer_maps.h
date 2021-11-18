/* Copyright (C) 2018-present, Facebook, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef __BALANCER_MAPS_H
#define __BALANCER_MAPS_H

/*
 * This file contains definition of maps used by the balancer typically
 * involving information pertaining to proper forwarding of packets
 */

#include "balancer_consts.h"
#include "balancer_structs.h"

// map, which contains all the vips for which we are doing load balancing
BPF_HASH(vip_map, struct vip_definition, struct vip_meta, MAX_VIPS);

// fallback lru. we should never hit this one outside of unittests
BPF_TABLE("lru_hash", struct flow_key, struct real_pos_lru, fallback_cache, FAKE_LRU_SIZE);

// This is a trick to make the BPF_ARRAY_OF_MAP use this table with the
// correct size that is pushed by the data plane before after having calculated
// the exact size of the LRU maps
BPF_TABLE("lru_hash", struct flow_key, struct real_pos_lru, fake_lru_map, FAKE_LRU_SIZE);
// map which contains cpu core to lru mapping
// I use the fallback_cache map to indicate the prototype of the innermap
// Then, when katran starts I will create the single LRU maps that will serve
// every core.
BPF_ARRAY_OF_MAPS(lru_mapping, "fake_lru_map", MAX_SUPPORTED_CPUS);

// map which contains all vip to real mappings
BPF_ARRAY(ch_rings, __u32, CH_RINGS_SIZE);

// map which contains opaque real's id to real mapping
BPF_ARRAY(reals, struct real_definition, MAX_REALS);

// map with per real pps/bps statistic
BPF_PERCPU_ARRAY(reals_stats, struct lb_stats, MAX_REALS);

// map w/ per vip statistics
BPF_PERCPU_ARRAY(stats, struct lb_stats, STATS_MAP_SIZE);

// map for quic connection-id to real's id mapping
BPF_ARRAY(quic_mapping , __u32, MAX_QUIC_REALS);

#ifdef LPM_SRC_LOOKUP
BPF_LPM_TRIE(lpm_src_v4, struct v4_lpm_key, __u32, MAX_LPM_SRC);
BPF_LPM_TRIE(lpm_src_v6, struct v6_lpm_key, __u32, MAX_LPM_SRC);
#endif

#endif // of _BALANCER_MAPS
