transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+/home/alumno/pr1_processor_design {/home/alumno/pr1_processor_design/uc.v}
vlog -vlog01compat -work work +incdir+/home/alumno/pr1_processor_design {/home/alumno/pr1_processor_design/cpu.v}
vlog -vlog01compat -work work +incdir+/home/alumno/pr1_processor_design {/home/alumno/pr1_processor_design/alu.v}
vlog -vlog01compat -work work +incdir+/home/alumno/pr1_processor_design {/home/alumno/pr1_processor_design/cd.v}
vlog -vlog01compat -work work +incdir+/home/alumno/pr1_processor_design {/home/alumno/pr1_processor_design/memprog.v}
vlog -vlog01compat -work work +incdir+/home/alumno/pr1_processor_design {/home/alumno/pr1_processor_design/componentes.v}

vlog -vlog01compat -work work +incdir+/home/alumno/pr1_processor_design {/home/alumno/pr1_processor_design/cpu_tb.v}

vsim -t 1ps -L altera_ver -L lpm_ver -L sgate_ver -L altera_mf_ver -L altera_lnsim_ver -L cycloneii_ver -L rtl_work -L work -voptargs="+acc"  cpu_tb

add wave *
view structure
view signals
run -all
