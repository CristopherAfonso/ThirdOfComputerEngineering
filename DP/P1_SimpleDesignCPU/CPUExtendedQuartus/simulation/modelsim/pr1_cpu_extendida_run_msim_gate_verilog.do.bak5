transcript on
if {[file exists gate_work]} {
	vdel -lib gate_work -all
}
vlib gate_work
vmap work gate_work

vlog -vlog01compat -work work +incdir+. {pr1_cpu_extendida.vo}

vlog -vlog01compat -work work +incdir+/home/alumno/Documentos/pr1_processor_design {/home/alumno/Documentos/pr1_processor_design/cpu_tb.v}

vsim -t 1ps +transport_int_delays +transport_path_delays -L cycloneii_ver -L gate_work -L work -voptargs="+acc"  pr1_cpu_extendida

add wave *
view structure
view signals
run -all
