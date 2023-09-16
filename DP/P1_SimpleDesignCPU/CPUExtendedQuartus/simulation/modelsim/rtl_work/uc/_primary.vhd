library verilog;
use verilog.vl_types.all;
entity uc is
    port(
        opcode          : in     vl_logic_vector(5 downto 0);
        z               : in     vl_logic;
        s               : in     vl_logic;
        o               : in     vl_logic;
        p               : in     vl_logic;
        s_inc           : out    vl_logic;
        s_inm           : out    vl_logic;
        s_rgj           : out    vl_logic;
        we3             : out    vl_logic;
        wez             : out    vl_logic;
        wes             : out    vl_logic;
        weo             : out    vl_logic;
        wep             : out    vl_logic;
        wed             : out    vl_logic;
        wext            : out    vl_logic;
        rws             : out    vl_logic;
        wsp             : out    vl_logic;
        wed_ext         : out    vl_logic;
        wess            : out    vl_logic;
        we_ram          : out    vl_logic;
        wro             : out    vl_logic_vector(1 downto 0)
    );
end uc;
