#--------------------------------------------------------------------
#   update.tcl
#   ==========
#--------------------------------------------------------------------

source ../bin/makemake.tcl
set binPrefix "..\\..\\bin\\win32"

#--------------------------------------------------------------------
# read any extension packages, and create list of targets
#--------------------------------------------------------------------
set global_gendeps 0
set targets {}
foreach ext [glob -nocomplain *.epk] {
    lappend targets [string range $ext [expr [string last / $ext]+1] end-4]
    source $ext
}

#--------------------------------------------------------------------
puts ""
puts ":: GENERATING..."
puts "================"

# vc8 project files
gen_allsolutions8_win32
gen_allsolutions9_win32

if {[file exists foundation_xbox360.epk]} {
    gen_allsolutions8_xbox360
    gen_allsolutions9_xbox360
}
puts "done."

#--------------------------------------------------------------------
#   EOF
#--------------------------------------------------------------------

