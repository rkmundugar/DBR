

# ======================================================================
# Default Script Options
# ======================================================================
set opt(chan)		Channel/WirelessChannel
set opt(prop)		Propagation/TwoRayGround
set opt(netif)		Phy/WirelessPhy
set opt(mac)		Mac/802_11
set opt(ifq)		CMUPriQueue
set opt(ll)			LL
set opt(ant)        Antenna/OmniAntenna

set opt(x)		1100		;# X dimension of the topography
set opt(y)		1100		;# Y dimension of the topography

set opt(cp)		"packet_set.tcl"
set opt(sc)		"map2.tcl"

set opt(ifqlen)	1000		;# max packet in ifq
set opt(nn)		194		;# number of nodes
set opt(tn)		194		;# number of nodes
set opt(seed)	0.0
set opt(stop)	500.0		;# simulation time
set opt(tr)		trace.tr	;# trace file
set opt(nam)    nam.out.tr	;# NAM trace file
set opt(rp)     dbr			;# routing protocol script
set opt(lm)     "off"		;# log movement


# ======================================================================

LL set mindelay_		50us
LL set delay_			25us
LL set bandwidth_		0	;# not used

#Mac/802_11 set RTSThreshold_ 3000     ;# Setting the RTS threhold
Mac/802_11 set dataRate_    11mb       ;# Setting the data rate
Mac/802_11 set basicRate_   1mb        ;# Setting the basic data rate

Agent/Null set sport_		0
Agent/Null set dport_		0

Agent/CBR set sport_		0
Agent/CBR set dport_		0

Agent/TCPSink set sport_	0
Agent/TCPSink set dport_	0

Agent/TCP set sport_		0
Agent/TCP set dport_		0
Agent/TCP set packetSize_	1460

Queue/DropTail/PriQueue set Prefer_Routing_Protocols    1

# unity gain, omni-directional antennas
# set up the antennas to be centered in the node and 1.5 meters above it
Antenna/OmniAntenna set X_ 0
Antenna/OmniAntenna set Y_ 0
Antenna/OmniAntenna set Z_ 1.5
Antenna/OmniAntenna set Gt_ 1.0
Antenna/OmniAntenna set Gr_ 1.0

# Initialize the SharedMedia interface with parameters to make
# it work like the 914MHz Lucent WaveLAN DSSS radio interface
Phy/WirelessPhy set CPThresh_ 10.0
Phy/WirelessPhy set CSThresh_ 1.559e-11
Phy/WirelessPhy set RXThresh_ 6.652e-11
Phy/WirelessPhy set Rb_ 2*1e6
Phy/WirelessPhy set freq_ 914e+6 
Phy/WirelessPhy set L_ 1.0


# The transimssion radio range 
#Phy/WirelessPhy set Pt_ 6.9872e-4    ;# ?m
#Phy/WirelessPhy set Pt_ 8.5872e-4    ;# 40m
#Phy/WirelessPhy set Pt_ 1.33826e-3   ;# 50m
#Phy/WirelessPhy set Pt_ 7.214e-3     ;# 100m
Phy/WirelessPhy set Pt_ 0.2818    	  ;# 250m
# ======================================================================

# Agent/DBR setting
Agent/DBR set hello_period_ 1.0		 ;#Hello message period

# ======================================================================

proc usage { argv0 }  {
	puts "Usage: $argv0"
	puts "\tmandatory arguments:"
	puts "\t\t\[-x MAXX\] \[-y MAXY\]"
	puts "\toptional arguments:"
	puts "\t\t\[-cp conn pattern\] \[-sc scenario\] \[-nn nodes\]"
	puts "\t\t\[-seed seed\] \[-stop sec\] \[-tr tracefile\]\n"
}


#proc getopt {argc argv} {
#	global opt
#	lappend optlist cp nn seed sc stop tr x y
#
#	for {set i 0} {$i < $argc} {incr i} {
#		set arg [lindex $argv $i]
#		if {[string range $arg 0 0] != "-"} continue
#
#		set name [string range $arg 1 end]
#		set opt($name) [lindex $argv [expr $i+1]]
#	}
#}

proc log-movement {} {
    global logtimer ns_ ns

    set ns $ns_
    source ../tcl/mobility/timer.tcl
    Class LogTimer -superclass Timer
    LogTimer instproc timeout {} {
	global opt node_;
	for {set i 0} {$i < $opt(nn)} {incr i} {
	    $node_($i) log-movement
	}
	$self sched 0.1
    }

    set logtimer [new LogTimer]
    $logtimer sched 0.1
}

# ======================================================================
# Main Program
# ======================================================================
#
# Source External TCL Scripts
#
#source ../lib/ns-mobilenode.tcl

#if { $opt(rp) != "" } {
	#source ../mobility/$opt(rp).tcl
	#} elseif { [catch { set env(NS_PROTO_SCRIPT) } ] == 1 } {
	#puts "\nenvironment variable NS_PROTO_SCRIPT not set!\n"
	#exit
#} else {
	#puts "\n*** using script $env(NS_PROTO_SCRIPT)\n\n";
        #source $env(NS_PROTO_SCRIPT)
#}
#source ../tcl/lib/ns-cmutrace.tcl
#source ../tcl/lib/ns-bsnode.tcl----------
#source ../tcl/mobility/com.tcl----------

# do the get opt again incase the routing protocol file added some more
# options to look for
#getopt $argc $argv

#if { $opt(x) == 0 || $opt(y) == 0 } {
#	usage $argv0
#	exit 1
#}

#if {$opt(seed) > 0} {
#	puts "Seeding Random number generator with $opt(seed)\n"
#	ns-random $opt(seed)
#}

#
# Initialize Global Variables
#
set ns_		[new Simulator]
set chan	[new $opt(chan)]
set prop	[new $opt(prop)]
set topo	[new Topography]

set tracefd   [open $opt(tr) w]
$ns_ trace-all  $tracefd

set namfile [open $opt(nam) w]
#$ns_ namtrace-all $namfile
#$ns_ namtrace-all-wireless $namfile $opt(x) $opt(y)

$topo load_flatgrid $opt(x) $opt(y)

$prop topography $topo

#
# Create God
#
set god_ [create-god $opt(nn)]

set chan_1_ [new $opt(chan)]

#
#  Create the specified number of nodes $opt(nn) and "attach" them
#  the channel.
#  Each routing protocol script is expected to have defined a proc
#  create-mobile-node that builds a mobile node and inserts it into the
#  array global $node_($i)
#

$ns_ node-config -adhocRouting dbr \
                 -llType $opt(ll) \
                 -macType $opt(mac) \
                 -ifqType $opt(ifq) \
                 -ifqLen $opt(ifqlen) \
                 -antType $opt(ant) \
                 -propType $opt(prop) \
                 -phyType $opt(netif) \
                 -topoInstance $topo \
                 -agentTrace ON \
                 -routerTrace ON \
                 -macTrace ON \
                 -movementTrace ON \
                 -channel $chan_1_ 

source ./dbr.tcl

for {set i 0} {$i < $opt(nn) } {incr i} {
    dbr-create-mobile-node $i
    #set node_($i) [$ns_ node]
    #$ns_ initial_node_pos $node_($i) 20
    #$node_($i) random-motion 0
}
    #$node_(0) random-motion 1
    #$node_(1) random-motion 0
    #$node_(142) random-motion 0
    #$node_(143) random-motion 0


# Connection and Movement scripts
#
if { $opt(cp) == "" } {
	puts "*** NOTE: no connection pattern specified."
        set opt(cp) "none"
} else {
	puts "Loading connection pattern..."
	source $opt(cp)
}

if { $opt(sc) == "" } {
	puts "*** NOTE: no scenario file specified."
        set opt(sc) "none"
} else {
	puts "Loading scenario file..."
	source $opt(sc)
	puts "Load complete..."
}

puts $tracefd "M 0.0 nn $opt(nn) x $opt(x) y $opt(y) rp $opt(rp)"
puts $tracefd "M 0.0 sc $opt(sc) cp $opt(cp) seed $opt(seed)"
puts $tracefd "M 0.0 prop $opt(prop) ant $opt(ant)"

# Define node initial position in nam
#for {set i 0} {$i < $opt(nn)} {incr i} {
#    # 20 defines the node size in nam, must adjust it according to your scenario
#    # The function must be called after mobility model is defined
#   $ns_ initial_node_pos $node_($i) 20
#	}
#########################################################################
#
# Tell all the nodes when the simulation ends
#
for {set i 0} {$i < $opt(nn) } {incr i} {
    $ns_ at $opt(stop) "$node_($i) reset";
}


$ns_ at $opt(stop) "finish"
$ns_ at $opt(stop) "puts \"NS EXITING...\" ; $ns_ halt"

proc finish {} {
    global ns_ tracefd namfile
    $ns_ flush-trace
    close $tracefd
    close $namfile
   	#exit 0
}

# DBR routing agent settings
for {set i 0} {$i < $opt(nn)} {incr i} {
    $ns_ at 0.0002 "$ragent_($i) turnon"
    #puts "node $i started"
   
}

puts "Starting Simulation..."


$ns_ run

