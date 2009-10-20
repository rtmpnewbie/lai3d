#--------------------------------------------------------------------
#   update_wii.tcl
#   ==============
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

if {[file exists foundation_wii.epk]} {
    gen_solutionxml_wii foundation_wii  
    gen_solutionxml_wii render_wii  
    gen_solutionxml_wii application_wii
    gen_solutionxml_wii addons_wii
    gen_solutionxml_wii extlibs_wii
    gen_solutionxml_wii wiitest_wii
    gen_solutionxml_wii wiigfxtest_wii
    gen_solutionxml_wii wiimotetest_wii
    gen_solutionxml_wii wiichartest_wii
    
#    gen_solutionxml_wii testviewer_wii
#    gen_solutionxml_wii tools_wii
}

puts "done."

#--------------------------------------------------------------------
#   EOF
#--------------------------------------------------------------------

