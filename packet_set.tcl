
# Upper layer agents/applications behavior
set null_(1) [new Agent/Null]$ns_ attach-agent $node_(45) $null_(1)

set udp_(1) [new Agent/UDP]
$ns_ attach-agent $node_(1) $udp_(1)

set cbr_(1) [new Application/Traffic/CBR]
$cbr_(1) set packetSize_ 16
$cbr_(1) set interval_ 2.0
$cbr_(1) set random_ 1
$cbr_(1) attach-agent $udp_(1)
$ns_ connect $udp_(1) $null_(1)
$ns_ at 30.0 "$cbr_(1) start"
$ns_ at 140.0 "$cbr_(1) stop" 