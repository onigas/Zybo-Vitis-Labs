# Test fixtures only: these commands DO NOT execute Vivado.
if {[llength $argv] != 2} { error "usage: tclsh test_tcl_helpers.tcl <repo_root> <temporary_directory>" }
set root [file normalize [lindex $argv 0]]
set tmp [file normalize [lindex $argv 1]]
set calls {}
set vivado_version "2022.2"
set projects {mock_project}
set repos {previous_board_repo}
set boards {digilentinc.com:zybo:part0:1.0}
proc get_projects {args} { return $::projects }
proc version {args} { return $::vivado_version }
proc get_param {name} { return $::repos }
proc set_param {name value} { set ::repos $value }
proc get_board_parts {args} { return $::boards }
proc write_hw_platform {args} {
    set options [lrange $args 0 end-1]
    foreach option $options {
        if {$option ni {-fixed -include_bit}} { error "Unexpected export option: $option" }
    }
    if {[file extension [lindex $args end]] ne ".xsa"} { error "Missing positional XSA output" }
    lappend ::calls $args
}
proc assert_true {condition message} {
    if {![uplevel 1 [list expr $condition]]} { error $message }
}
proc expect_error {command pattern} {
    set code [catch {uplevel 1 $command} result]
    if {!$code || ![string match $pattern $result]} {
        error "Expected error matching $pattern, got code=$code result=$result"
    }
}
source [file join $root scripts vitis2022_2 export_xsa.tcl]
source [file join $root scripts vitis2022_2 register_board_repo.tcl]
export_lab_xsa [file join $tmp export lab1.xsa] 0
assert_true {[lsearch -exact [lindex $calls 0] -include_bit] < 0} "PS-only export must not include a bitstream"
export_lab_xsa [file join $tmp export lab2.xsa] 1
assert_true {[lsearch -exact [lindex $calls 1] -include_bit] >= 0} "PL export must request a bitstream"
expect_error [list export_lab_xsa [file join $tmp bad.xsa] 2] "*must be 0 or 1*"
expect_error [list export_lab_xsa [file join $tmp bad.hdf] 0] "*Use an .xsa*"
set vivado_version "2024.1"
expect_error [list export_lab_xsa [file join $tmp bad.xsa] 0] "*target Vivado 2022.2*"
set vivado_version "2022.2"
set projects {}
expect_error [list export_lab_xsa [file join $tmp bad.xsa] 0] "*Open the lab project*"
set projects {mock_project}
set existing [file join $tmp existing.xsa]
close [open $existing w]
expect_error [list export_lab_xsa $existing 0] "*Refusing to overwrite*"
set boardroot [file join $tmp board_files]
file mkdir $boardroot
register_zybo_board_repo $boardroot
register_zybo_board_repo $boardroot
assert_true {[llength $repos] == 2} "Existing board paths must remain and duplicates must not be added"
assert_true {[lindex $repos 0] eq "previous_board_repo"} "Lost existing board repository"
set boards {}
expect_error [list register_zybo_board_repo $boardroot] "*No legacy Zybo board found*"
expect_error [list register_zybo_board_repo [file join $tmp nonexistent]] "*not found*"
puts "PASS: Tcl helper control flow, export options, no-overwrite and board-repo guards with MOCK Vivado commands."
