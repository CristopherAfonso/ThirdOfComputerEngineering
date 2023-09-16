library verilog;
use verilog.vl_types.all;
entity sevensegments is
    port(
        clk             : in     vl_logic;
        reset           : in     vl_logic;
        wess            : in     vl_logic;
        in_val          : in     vl_logic_vector(6 downto 0);
        in_dir          : in     vl_logic_vector(1 downto 0);
        out_d0          : out    vl_logic_vector(6 downto 0);
        out_d1          : out    vl_logic_vector(6 downto 0);
        out_d2          : out    vl_logic_vector(6 downto 0);
        out_d3          : out    vl_logic_vector(6 downto 0)
    );
end sevensegments;
