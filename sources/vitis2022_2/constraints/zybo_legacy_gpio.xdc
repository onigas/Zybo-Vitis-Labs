# Zybo legacy Rev B ONLY. Physical mappings verified against Digilent
# digilent-xdc/Zybo-Master.xdc. Not a Zybo Z7 pinout.
# Use external scalar/vector ports switches[3:0] and buttons[3:0].
# Do not also apply board-automation constraints to these same ports.
set_property -dict {PACKAGE_PIN G15 IOSTANDARD LVCMOS33} [get_ports {switches[0]}]
set_property -dict {PACKAGE_PIN P15 IOSTANDARD LVCMOS33} [get_ports {switches[1]}]
set_property -dict {PACKAGE_PIN W13 IOSTANDARD LVCMOS33} [get_ports {switches[2]}]
set_property -dict {PACKAGE_PIN T16 IOSTANDARD LVCMOS33} [get_ports {switches[3]}]
set_property -dict {PACKAGE_PIN R18 IOSTANDARD LVCMOS33} [get_ports {buttons[0]}]
set_property -dict {PACKAGE_PIN P16 IOSTANDARD LVCMOS33} [get_ports {buttons[1]}]
set_property -dict {PACKAGE_PIN V16 IOSTANDARD LVCMOS33} [get_ports {buttons[2]}]
set_property -dict {PACKAGE_PIN Y16 IOSTANDARD LVCMOS33} [get_ports {buttons[3]}]
