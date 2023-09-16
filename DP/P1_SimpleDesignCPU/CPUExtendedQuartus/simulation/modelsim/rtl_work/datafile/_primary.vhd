library verilog;
use verilog.vl_types.all;
entity datafile is
    port(
        clk             : in     vl_logic;
        wed             : in     vl_logic;
        fa              : in     vl_logic_vector(10 downto 0);
        sw              : in     vl_logic_vector(15 downto 0);
        d_mem           : out    vl_logic_vector(15 downto 0)
    );
end datafile;
