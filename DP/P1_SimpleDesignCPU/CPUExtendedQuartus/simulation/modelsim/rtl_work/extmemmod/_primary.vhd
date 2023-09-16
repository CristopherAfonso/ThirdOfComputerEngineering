library verilog;
use verilog.vl_types.all;
entity extmemmod is
    port(
        rw              : in     vl_logic;
        d_in            : in     vl_logic_vector(15 downto 0);
        d_out           : out    vl_logic_vector(15 downto 0);
        mem_cable       : inout  vl_logic_vector(15 downto 0)
    );
end extmemmod;
