/* Copyright (c) Facebook, Inc. and its affiliates. All Rights Reserved.
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

#ifndef __HEALTHCHECKING_MAPS_H
#define __HEALTHCHECKING_MAPS_H

#include "balancer_consts.h"
#include "healthchecking_consts.h"
#include "healthchecking_structs.h"

BPF_ARRAY(hc_ctrl_map, __u32, CTRL_MAP_SIZE);

BPF_HASH(hc_reals_map, __u32, struct hc_real_definition, MAX_REALS);

BPF_ARRAY(hc_pckt_srcs_map, struct hc_real_definition, 2);

BPF_ARRAY(hc_pckt_macs, struct hc_mac, 2);

// map which contains counters for monitoring
BPF_PERCPU_ARRAY(hc_stats_map, struct hc_stats, STATS_SIZE);

#endif // of __HEALTHCHECKING_MAPS_H
