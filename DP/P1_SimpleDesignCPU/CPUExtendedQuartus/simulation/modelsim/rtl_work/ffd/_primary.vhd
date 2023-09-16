library verilog;
use verilog.vl_types.all;
entity ffd is
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        d               : in     vl_logic;
        carga           : in     vl_logic;
        q               : out    vl_logic
    );
end ffd;
