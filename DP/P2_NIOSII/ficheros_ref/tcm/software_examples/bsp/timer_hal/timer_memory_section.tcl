# Get region information for tightly_coupled_instruction_memory memory region.
# Returned as a list.
set region_info [get_memory_region tightly_coupled_instruction_memory]
# Extract fields from region information list.
set region_name [lindex $region_info 0]
set slave [lindex $region_info 1]
set offset [lindex $region_info 2]
set span [lindex $region_info 3]
# Remove the existing memory region.
delete_memory_region $region_name
# Compute memory ranges for replacement regions.
set split_span 2048
set new_span [expr $span-$split_span]
set split_offset [expr $offset+$new_span]
# Create tightly_couple_memory region.
add_memory_region tightly_coupled_instruction_memory $slave $offset $new_span
# Create a second region called timer_isr memory_region.
add_memory_region timer_isrs_region $slave $split_offset $split_span
# Create memory mapping to map .isrs to timer_isr_region.
add_section_mapping .isrs timer_isrs_region
# Locate the exception stack to tightly coupled data memory.
set_setting hal.linker.enable_exception_stack TRUE
set_setting hal.linker.exception_stack_memory_region_name tightly_coupled_data_memory
set_setting hal.linker.exception_stack_size 1024