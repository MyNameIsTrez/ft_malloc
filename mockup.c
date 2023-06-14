#include <stddef.h>

struct block
{
	void *address;
};

struct zone
{
	struct block *blocks;
	void *address; // From mmap()
	size_t available_block_index;
};

struct meta
{
	struct zone *tiny_zones;
	struct zone *small_zones;
	struct zone *large_zones;

	size_t available_tiny_zone_index;
	size_t available_small_zone_index;
	size_t available_large_zone_index;
};

#define TINY_BLOCK_LENGTH 10
#define BLOCKS_PER_ZONE 100
#define TINY_ZONE_SIZE TINY_BLOCK_LENGTH * BLOCKS_PER_ZONE

// Called by for example malloc(3)
fn allocate_tiny(size)
	zone = meta.tiny_zones[available_tiny_zone_index]

	if zone.available_block_index < BLOCKS_PER_ZONE
		block_address = zone.address + zone.available_block_index * TINY_BLOCK_LENGTH
		zone.available_block_index++
		push_block(zone.blocks, (block){block_address})
		return block_address
	else
		available_tiny_zone_index++

	push_new_zone(meta.tiny_zones)
	return allocate_tiny(size)

// Called by for example free(tiny_ptr)
fn free_tiny(block_address)
	zone_index = get_zone_index_from_block_address(block_address) // Can use hashmap for O(1)
	if zone == -1
		return
	zone = meta.tiny_zones[zone_index]

	block_index = get_block_index_from_block_address(block_address)
	if block_index == -1
		return
	// Overwrite this block with the last block that's not free
	zone.blocks[block_index] = zone.blocks[--zone.available_block_index]

	if zone.available_block_index == 0
		// Overwrite this zone with the last zone that's not free
		meta.tiny_zones[zone_index] = meta.tiny_zones[--meta.available_tiny_zone_index]