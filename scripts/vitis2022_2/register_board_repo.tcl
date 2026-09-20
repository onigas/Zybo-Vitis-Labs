# Run in Vivado 2022.2 Tcl Console (not in XSCT).
# source C:/path/to/repo/scripts/vitis2022_2/register_board_repo.tcl
# register_zybo_board_repo C:/fpga/vivado-boards/new/board_files
proc register_zybo_board_repo {board_root} {
    set root [file normalize $board_root]
    if {![file isdirectory $root]} { error "Board repository not found: $root" }
    set repos [get_param board.repoPaths]
    if {[lsearch -exact $repos $root] < 0} { lappend repos $root }
    set_param board.repoPaths $repos
    set boards [get_board_parts -quiet *:zybo:part0:*]
    if {[llength $boards] == 0} {
        error "No legacy Zybo board found. Expected new/board_files/zybo/<revision>/board.xml. Do not select zybo-z7."
    }
    puts "Legacy Zybo board definitions in this Vivado session:"
    foreach b $boards { puts "  $b" }
    return $boards
}
