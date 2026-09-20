# Run in Vivado 2022.2 Tcl Console with the relevant project open.
# Lab 1: export_lab_xsa C:/fpga/zybo_work/export/lab1.xsa 0
# Lab 2+: open the completed implemented design first, then use 1.
proc export_lab_xsa {output_xsa include_bitstream} {
    if {$include_bitstream ni {0 1}} { error "include_bitstream must be 0 or 1" }
    if {[llength [get_projects -quiet]] == 0} { error "Open the lab project first." }
    if {![string match 2022.2* [version -short]]} {
        error "These labs target Vivado 2022.2. Current version: [version -short]"
    }
    set out [file normalize $output_xsa]
    if {[string tolower [file extension $out]] ne ".xsa"} { error "Use an .xsa output file." }
    file mkdir [file dirname $out]
    if {[file exists $out]} {
        error "Refusing to overwrite $out. Choose a new versioned filename or remove the old export yourself."
    }
    if {$include_bitstream} {
        # Vivado validates whether a matching bitstream is available.
        write_hw_platform -fixed -include_bit $out
    } else {
        write_hw_platform -fixed $out
    }
    puts "Exported: $out"
}
