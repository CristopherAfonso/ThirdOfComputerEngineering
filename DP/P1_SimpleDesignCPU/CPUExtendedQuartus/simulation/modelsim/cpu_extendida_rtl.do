onerror {resume}
quietly WaveActivateNextPane {} 0
add wave -noupdate /pr1_cpu_extendida/clk
add wave -noupdate /pr1_cpu_extendida/reset
add wave -noupdate /pr1_cpu_extendida/write_ext
add wave -noupdate /pr1_cpu_extendida/read_ext
add wave -noupdate /pr1_cpu_extendida/we_ram
add wave -noupdate -label {Bus Dir.} -radix decimal /pr1_cpu_extendida/dir_mem_ex
add wave -noupdate -label {Bus Datos} -radix decimal /pr1_cpu_extendida/data_bus
add wave -noupdate -label {LED HEX3} -radix binary /pr1_cpu_extendida/out_d0
add wave -noupdate -label {LED HEX2} -radix binary /pr1_cpu_extendida/out_d1
add wave -noupdate -label {LED HEX1} -radix binary /pr1_cpu_extendida/out_d2
add wave -noupdate -label {LED HEX0} -radix binary /pr1_cpu_extendida/out_d3
add wave -noupdate -label R1 -radix decimal {/pr1_cpu_extendida/pr1_cpu_extendida/data_path/banc_reg/regb[1]}
add wave -noupdate -label R2 -radix decimal {/pr1_cpu_extendida/pr1_cpu_extendida/data_path/banc_reg/regb[2]}
add wave -noupdate -label R3 -radix decimal {/pr1_cpu_extendida/pr1_cpu_extendida/data_path/banc_reg/regb[3]}
add wave -noupdate -label R4 -radix decimal {/pr1_cpu_extendida/pr1_cpu_extendida/data_path/banc_reg/regb[4]}
add wave -noupdate -label R5 -radix decimal {/pr1_cpu_extendida/pr1_cpu_extendida/data_path/banc_reg/regb[5]}
add wave -noupdate -label R6 -radix decimal {/pr1_cpu_extendida/pr1_cpu_extendida/data_path/banc_reg/regb[6]}
add wave -noupdate -label R7 -radix decimal {/pr1_cpu_extendida/pr1_cpu_extendida/data_path/banc_reg/regb[7]}
add wave -noupdate -label R8 -radix decimal {/pr1_cpu_extendida/pr1_cpu_extendida/data_path/banc_reg/regb[8]}
add wave -noupdate -label R10 -radix decimal {/pr1_cpu_extendida/pr1_cpu_extendida/data_path/banc_reg/regb[9]}
add wave -noupdate -label PC -radix unsigned /pr1_cpu_extendida/pr1_cpu_extendida/data_path/pc/q
TreeUpdate [SetDefaultTree]
WaveRestoreCursors {{Cursor 1} {1280352240281 ps} 0}
quietly wave cursor active 1
configure wave -namecolwidth 207
configure wave -valuecolwidth 54
configure wave -justifyvalue left
configure wave -signalnamewidth 0
configure wave -snapdistance 10
configure wave -datasetprefix 0
configure wave -rowmargin 4
configure wave -childrowmargin 2
configure wave -gridoffset 0
configure wave -gridperiod 1
configure wave -griddelta 40
configure wave -timeline 0
configure wave -timelineunits ns
update
WaveRestoreZoom {1270730971060 ps} {1270733118954 ps}
